#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>
#include "../db/db_utils.h"

// ✅ Paramètres de connexion à la base
#define DB_HOST "127.0.0.1"
#define DB_USER "root"
#define DB_PASS ""
#define DB_NAME "election_db"
#define DB_PORT 3306

// ✅ Fonction pour établir la connexion
MYSQL* connect_to_database() {
    MYSQL *conn = mysql_init(NULL);

    if (conn == NULL) {
        fprintf(stderr, "Erreur : impossible d'initialiser MySQL.\n");
        exit(EXIT_FAILURE);
    }

    // Désactive SSL
    mysql_ssl_set(conn, NULL, NULL, NULL, NULL, NULL);
    my_bool verify_cert = 0;
    mysql_options(conn, MYSQL_OPT_SSL_VERIFY_SERVER_CERT, &verify_cert);


    if (!mysql_real_connect(conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, DB_PORT, NULL, 0)) {
        fprintf(stderr, "Erreur de connexion : %s\n", mysql_error(conn));
        mysql_close(conn);
        exit(EXIT_FAILURE);
    }

    printf("✅ Connecté à la base de données '%s' avec succès !\n", DB_NAME);
    return conn;
}

// ✅ Fermer la connexion proprement
void close_connection(MYSQL *conn) {
    if (conn != NULL) {
        mysql_close(conn);
        printf("🔒 Connexion MySQL fermée.\n");
    }
}

// ✅ Exécuter une requête (INSERT, UPDATE, DELETE)
int execute_query(MYSQL *conn, const char *query) {
    if (mysql_query(conn, query)) {
        fprintf(stderr, "Erreur SQL : %s\n", mysql_error(conn));
        return 0;
    }
    return 1;
}

// ✅ Exécuter un SELECT (renvoie un résultat)
MYSQL_RES* execute_select(MYSQL *conn, const char *query) {
    if (mysql_query(conn, query)) {
        fprintf(stderr, "Erreur SQL (SELECT) : %s\n", mysql_error(conn));
        return NULL;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    if (!res) {
        fprintf(stderr, "Erreur de récupération du résultat : %s\n", mysql_error(conn));
        return NULL;
    }

    return res;
}
