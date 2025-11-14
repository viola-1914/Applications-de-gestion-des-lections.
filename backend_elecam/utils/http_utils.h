#ifndef HTTP_UTILS_H
#define HTTP_UTILS_H

#include <winsock2.h>

void send_http_response(SOCKET client_socket, int status, const char *message);
void send_json_response(SOCKET client_socket, int status, const char *json);

#endif
