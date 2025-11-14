#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../db/db_utils.h"
#include "../utils/http_utils.h"
#include "../utils/json_utils.h"
#include "voters_api.h"

// 🔹 GET /voters → renvoie la liste de tous les électeurs
void handle_get_voters(SOCKET client_socket) {
    MYSQL *conn = connect_to_database();

    if (mysql_query(conn, "SELECT id, nom, quartier, age, has_voted, voted_candidate_id FROM voters")) {
        send_http_response(client_socket, 500, "Erreur lors de la requête MySQL");
        mysql_close(conn);
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row;

    char json[8192] = "[";
    int first = 1;

    while ((row = mysql_fetch_row(res))) {
        if (!first) strcat(json, ",");
        first = 0;

        Electeur e;
        e.id = atoi(row[0]);
        strcpy(e.nom, row[1]);
        strcpy(e.quartier, row[2]);
        e.age = atoi(row[3]);
        e.has_voted = atoi(row[4]);
        e.voted_candidate_id = row[5] ? atoi(row[5]) : 0;

        char record[512];
        snprintf(record, sizeof(record),
                 "{\"id\": %d, \"nom\": \"%s\", \"quartier\": \"%s\", \"age\": %d, \"has_voted\": %d, \"voted_candidate_id\": %d}",
                 e.id, e.nom, e.quartier, e.age, e.has_voted, e.voted_candidate_id);

        strcat(json, record);
    }

    strcat(json, "]");
    mysql_free_result(res);
    mysql_close(conn);
    send_json_response(client_socket, 200, json);
}

// 🔧 Nettoie les espaces et retours à la ligne du JSON
static void clean_json(char *str) {
    char *src = str, *dst = str;
    while (*src) {
        if (*src != '\n' && *src != '\r' && *src != ' ' && *src != '\t')
            *dst++ = *src;
        src++;
    }
    *dst = '\0';
}

// 🔹 POST /voters → ajoute un électeur s’il n’existe pas déjà
void handle_add_voter(SOCKET client_socket, const char *body) {
    Electeur e;
    memset(&e, 0, sizeof(Electeur));


    // ✅ Nettoyer le JSON
    clean_json((char *)body);

    // ✅ Vérifie si le JSON contient bien tous les champs nécessaires
    if (sscanf(body, "{\"nom\":\"%[^\"]\",\"quartier\":\"%[^\"]\",\"age\":%d}", e.nom, e.quartier, &e.age) != 3) {
        send_http_response(client_socket, 400, "Format JSON invalide. Champs attendus : {nom, quartier, age}");
        return;
    }

    MYSQL *conn = connect_to_database();

    // ✅ Vérifie si l’électeur existe déjà (nom + quartier)
    char check_query[256];
    snprintf(check_query, sizeof(check_query),
             "SELECT id FROM voters WHERE nom='%s' AND quartier='%s'", e.nom, e.quartier);

    if (mysql_query(conn, check_query)) {
        send_http_response(client_socket, 500, "Erreur lors de la vérification d'existence");
        mysql_close(conn);
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    if (mysql_num_rows(res) > 0) {
        send_http_response(client_socket, 400, "Électeur déjà enregistré !");
        mysql_free_result(res);
        mysql_close(conn);
        return;
    }
    mysql_free_result(res);

    // ✅ Insertion du nouvel électeur
    char insert_query[512];
    snprintf(insert_query, sizeof(insert_query),
             "INSERT INTO voters (nom, quartier, age, has_voted, voted_candidate_id) "
             "VALUES ('%s', '%s', %d, 0, NULL)",
             e.nom, e.quartier, e.age);

    if (mysql_query(conn, insert_query)) {
        char error_msg[256];
        snprintf(error_msg, sizeof(error_msg), "Erreur MySQL : %s", mysql_error(conn));
        send_http_response(client_socket, 500, error_msg);
    } else {
        send_http_response(client_socket, 200, "Électeur ajouté avec succès !");
    }

    mysql_close(conn);
}


// 🔹 GET /voters/{id}/voted → vérifie si un électeur a voté
void handle_has_voted(SOCKET client_socket, int voter_id) {
    MYSQL *conn = connect_to_database();
    char query[256];
    snprintf(query, sizeof(query),
             "SELECT has_voted FROM voters WHERE id=%d", voter_id);

    if (mysql_query(conn, query)) {
        send_http_response(client_socket, 500, "Erreur de requête MySQL");
        mysql_close(conn);
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(res);

    if (row) {
        int has_voted = atoi(row[0]);
        if (has_voted)
            send_http_response(client_socket, 200, "L’électeur a déjà voté ✅");
        else
            send_http_response(client_socket, 200, "L’électeur n’a pas encore voté ❌");
    } else {
        send_http_response(client_socket, 404, "Électeur non trouvé");
    }

    mysql_free_result(res);
    mysql_close(conn);
}
