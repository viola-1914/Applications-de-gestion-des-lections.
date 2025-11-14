#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../db/db_utils.h"
#include "../utils/http_utils.h"
#include "../utils/json_utils.h"
#include "vote_api.h"

// 🔹 POST /vote → un électeur vote pour un candidat
void handle_vote(SOCKET client_socket, const char *body) {
    Vote v;
    // Lecture du corps JSON reçu
    sscanf(body, "{\"id_electeur\":%d,\"id_candidat\":%d}", &v.id_electeur, &v.id_candidat);

    MYSQL *conn = connect_to_database();

    // Vérifie si l’électeur existe et n’a pas encore voté
    char check_query[256];
    snprintf(check_query, sizeof(check_query),
             "SELECT has_voted FROM voters WHERE id=%d", v.id_electeur);

    if (mysql_query(conn, check_query)) {
        send_http_response(client_socket, 500, "Erreur lors de la vérification de l’électeur !");
        mysql_close(conn);
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(res);

    if (!row) {
        send_http_response(client_socket, 404, "Électeur non trouvé !");
        mysql_free_result(res);
        mysql_close(conn);
        return;
    }

    int has_voted = atoi(row[0]);
    mysql_free_result(res);

    if (has_voted == 1) {
        send_http_response(client_socket, 400, "Cet électeur a déjà voté !");
        mysql_close(conn);
        return;
    }

    // Met à jour le nombre de votes du candidat
    char update_candidate[256];
    snprintf(update_candidate, sizeof(update_candidate),
             "UPDATE candidates SET votes = votes + 1 WHERE id = %d", v.id_candidat);

    if (mysql_query(conn, update_candidate)) {
        send_http_response(client_socket, 500, "Erreur lors de la mise à jour des votes du candidat !");
        mysql_close(conn);
        return;
    }

    // Marque l’électeur comme ayant voté et enregistre son choix
    char update_voter[256];
    snprintf(update_voter, sizeof(update_voter),
             "UPDATE voters SET has_voted = 1, voted_candidate_id = %d WHERE id = %d",
             v.id_candidat, v.id_electeur);

    if (mysql_query(conn, update_voter)) {
        send_http_response(client_socket, 500, "Erreur lors de la mise à jour de l’électeur !");
        mysql_close(conn);
        return;
    }

    send_http_response(client_socket, 200, "✅ Vote enregistré avec succès !");
    mysql_close(conn);
}

// 🔹 GET /results → classe les candidats par nombre de votes décroissant
void handle_results(SOCKET client_socket) {
    MYSQL *conn = connect_to_database();

    if (mysql_query(conn, "SELECT id, nom, parti, votes FROM candidates ORDER BY votes DESC")) {
        send_http_response(client_socket, 500, "Erreur de récupération des résultats !");
        mysql_close(conn);
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    char *json = result_to_json(res);
    send_json_response(client_socket, 200, json);

    free(json);
    mysql_free_result(res);
    mysql_close(conn);
}

// 🔹 GET /refunded → candidats avec ≥ 12% des voix (caution remboursée)
void handle_refunded_candidates(SOCKET client_socket) {
    MYSQL *conn = connect_to_database();

    // Calcule le total des votes
    if (mysql_query(conn, "SELECT SUM(votes) FROM candidates")) {
        send_http_response(client_socket, 500, "Erreur lors du calcul du total des votes !");
        mysql_close(conn);
        return;
    }

    MYSQL_RES *res_total = mysql_store_result(conn);
    MYSQL_ROW row_total = mysql_fetch_row(res_total);
    int total_votes = row_total && row_total[0] ? atoi(row_total[0]) : 0;
    mysql_free_result(res_total);

    if (total_votes == 0) {
        send_http_response(client_socket, 200, "Aucun vote enregistré pour le moment !");
        mysql_close(conn);
        return;
    }

    // Récupère les candidats avec pourcentage >= 12%
    char query[512];
    snprintf(query, sizeof(query),
             "SELECT id, nom, parti, votes, (votes * 100 / %d) AS pourcentage "
             "FROM candidates WHERE (votes * 100 / %d) >= 12",
             total_votes, total_votes);

    if (mysql_query(conn, query)) {
        send_http_response(client_socket, 500, "Erreur dans le calcul des remboursements !");
        mysql_close(conn);
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    char *json = result_to_json(res);
    send_json_response(client_socket, 200, json);

    free(json);
    mysql_free_result(res);
    mysql_close(conn);
}
