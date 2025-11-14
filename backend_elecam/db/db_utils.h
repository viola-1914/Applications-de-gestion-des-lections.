#ifndef DB_UTILS_H
#define DB_UTILS_H

#include <mysql.h>

// ✅ Structure globale pour gérer la connexion à la base
MYSQL* connect_to_database();
void close_connection(MYSQL *conn);
int execute_query(MYSQL *conn, const char *query);
MYSQL_RES* execute_select(MYSQL *conn, const char *query);

#endif
