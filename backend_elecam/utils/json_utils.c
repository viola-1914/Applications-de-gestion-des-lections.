#include "json_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *result_to_json(MYSQL_RES *res) {
    MYSQL_ROW row;
    int num_fields = mysql_num_fields(res);
    MYSQL_FIELD *fields = mysql_fetch_fields(res);

    char *json = malloc(8192);
    strcpy(json, "[");

    int first = 1;
    while ((row = mysql_fetch_row(res))) {
        if (!first) strcat(json, ",");
        strcat(json, "{");
        for (int i = 0; i < num_fields; i++) {
            char field[256];
            snprintf(field, sizeof(field),
                     "\"%s\":\"%s\"%s",
                     fields[i].name, row[i] ? row[i] : "",
                     (i < num_fields - 1) ? "," : "");
            strcat(json, field);
        }
        strcat(json, "}");
        first = 0;
    }
    strcat(json, "]");
    return json;
}
