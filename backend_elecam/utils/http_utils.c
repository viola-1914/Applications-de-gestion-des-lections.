#include "http_utils.h"
#include <stdio.h>
#include <string.h>

void send_http_response(SOCKET client_socket, int status, const char *message) {
    char response[1024];
    snprintf(response, sizeof(response),
             "HTTP/1.1 %d OK\r\n"
             "Content-Type: text/plain\r\n"
             "Access-Control-Allow-Origin: *\r\n"
             "\r\n"
             "%s", status, message);
    send(client_socket, response, strlen(response), 0);//passé en parametre un tableau se comporte comme un pointeur
}

void send_json_response(SOCKET client_socket, int status, const char *json) {
    char header[256];
    snprintf(header, sizeof(header),
             "HTTP/1.1 %d OK\r\n"
             "Content-Type: application/json\r\n"
             "Access-Control-Allow-Origin: *\r\n"
             "\r\n", status);
    send(client_socket, header, strlen(header), 0);
    send(client_socket, json, strlen(json), 0);
}
