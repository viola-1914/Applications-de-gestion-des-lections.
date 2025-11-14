#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../db/db_utils.h"
#include "../utils/http_utils.h"
#include "../utils/json_utils.h"
#include "candidates_api.h"

// 🔧 Fonction utilitaire pour nettoyer le JSON (supprime espaces, retours à la ligne, tabulations)
static void clean_json(char *str) {
    char *src = str, *dst = str;
    while (*src) {
        if (*src != '\n' && *src != '\r' && *src != '\t' && *src != ' ')
            *dst++ = *src;
        src++;
    }
    *dst = '\0';
}

// 🔹 Fonction GET /candidates → renvoie tous les candidats au format JSON
void handle_get_candidates(SOCKET client_socket) {
    MYSQL *conn = connect_to_database();
    if (mysql_query(conn, "SELECT id, nom, parti, age, votes, nationalite, url_image FROM candidates")) {
        send_http_response(client_socket, 500, "Erreur lors de la requête MySQL");
        mysql_close(conn);
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row;

    // ✅ Buffer sécurisé et plus grand
    char json[16384];
    size_t offset = 0;
    offset += snprintf(json + offset, sizeof(json) - offset, "[");

    int first = 1;
    while ((row = mysql_fetch_row(res))) {
        if (!first) {
            offset += snprintf(json + offset, sizeof(json) - offset, ",");
        }
        first = 0;

        // ✅ Sécuriser chaque champ contre les NULL
        const char *nom = row[1] ? row[1] : "";
        const char *parti = row[2] ? row[2] : "";
        const char *age_str = row[3] ? row[3] : "0";
        const char *votes_str = row[4] ? row[4] : "0";
        const char *nationalite = row[5] ? row[5] : "";
        const char *url_image = row[6] ? row[6] : "";

        int id = row[0] ? atoi(row[0]) : 0;
        int age = atoi(age_str);
        int votes = atoi(votes_str);

        // ✅ Concaténation protégée
        offset += snprintf(json + offset, sizeof(json) - offset,
            "{\"id\": %d, \"nom\": \"%s\", \"parti\": \"%s\", \"age\": %d, \"votes\": %d, \"nationalite\": \"%s\", \"url_image\": \"%s\"}",
            id, nom, parti, age, votes, nationalite, url_image);

        // ✅ Stopper avant de dépasser le buffer
        if (offset >= sizeof(json) - 512) {
            printf("⚠️  Trop de données, JSON tronqué.\n");
            break;
        }
    }

    offset += snprintf(json + offset, sizeof(json) - offset, "]");
    mysql_free_result(res);
    mysql_close(conn);

    send_json_response(client_socket, 200, json);
}

// 🔹 Fonction POST /candidates → ajoute un nouveau candidat dans la base
void handle_add_candidate(SOCKET client_socket, const char *body) {
    char cleaned[512];
    strncpy(cleaned, body, sizeof(cleaned) - 1);
    cleaned[sizeof(cleaned) - 1] = '\0';
    clean_json(cleaned);  // Nettoyage du JSON

    Candidat c;
    memset(&c, 0, sizeof(Candidat));

    // ✅ Parser tous les champs attendus
    int parsed = sscanf(cleaned,
        "{\"nom\":\"%[^\"]\",\"parti\":\"%[^\"]\",\"age\":%d,\"nationalite\":\"%[^\"]\",\"url_image\":\"%[^\"]\"}",
        c.nom, c.parti, &c.age, c.nationalite, c.url_image);

    if (parsed != 5) {
        send_http_response(client_socket, 400,
            "Format JSON invalide. Attendu : {\"nom\":\"...\",\"parti\":\"...\",\"age\":...,\"nationalite\":\"...\",\"url_image\":\"...\"}");
        printf("⚠️  JSON invalide reçu : %s\n", body);
        return;
    }

    printf("✅ Candidat reçu → nom='%s', parti='%s', age=%d, nationalite='%s', url_image='%s'\n",
           c.nom, c.parti, c.age, c.nationalite, c.url_image);

    MYSQL *conn = connect_to_database();
    char query[512];
    snprintf(query, sizeof(query),
        "INSERT INTO candidates (nom, parti, age, votes, nationalite, url_image) "
        "VALUES ('%s', '%s', %d, 0, '%s', '%s')",
        c.nom, c.parti, c.age, c.nationalite, c.url_image);

    if (mysql_query(conn, query)) {
        printf("❌ Erreur MySQL : %s\n", mysql_error(conn));
        send_http_response(client_socket, 500, "Erreur lors de l’insertion du candidat");
    } else {
        send_http_response(client_socket, 200, "Candidat ajouté avec succès !");
    }

    mysql_close(conn);
}
// 🔹 GET /rejected → compte les candidats rejetés (accepted = 0)
void handle_rejected_candidates(SOCKET client_socket) {
    MYSQL *conn = connect_to_database();

    if (mysql_query(conn, "SELECT COUNT(*) FROM candidates WHERE accepted = 0")) {
        send_http_response(client_socket, 500, "Erreur lors du comptage des candidats rejetés !");
        mysql_close(conn);
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(res);
    int rejected_count = row && row[0] ? atoi(row[0]) : 0;

    char json[128];
    snprintf(json, sizeof(json), "{\"rejected\": %d}", rejected_count);

    send_json_response(client_socket, 200, json);

    mysql_free_result(res);
    mysql_close(conn);
}
