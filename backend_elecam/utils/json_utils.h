#ifndef JSON_UTILS_H
#define JSON_UTILS_H

#include <mysql.h>

char *result_to_json(MYSQL_RES *res);

#endif
