#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "db/db_utils.h"
#include "api/candidates_api.h"
#include "api/voters_api.h"
#include "api/vote_api.h"
#include "utils/http_utils.h"

#pragma comment(lib, "ws2_32.lib") // Liaison à la librairie réseau Windowsuj

// 🔹 Fonction utilitaire pour envoyer une réponse CORS générique
void send_cors_response(SOCKET client_socket) {
    const char *response =
        "HTTP/1.1 204 No Content\r\n"
        "Access-Control-Allow-Origin: http://localhost:5173\r\n"
        "Access-Control-Allow-Methods: GET, POST, OPTIONS\r\n"
        "Access-Control-Allow-Headers: Content-Type\r\n"
        "Content-Length: 0\r\n"
        "\r\n";
    send(client_socket, response, strlen(response), 0);
}

// 🔹 Fonction utilitaire pour envoyer une réponse 404
void send_not_found(SOCKET client_socket) {
    const char *response =
        "HTTP/1.1 404 Not Found\r\n"
        "Access-Control-Allow-Origin: http://localhost:5173\r\n"
        "Content-Type: text/plain\r\n"
        "\r\n"
        "❌ Endpoint non trouvé !";
    send(client_socket, response, strlen(response), 0);
}

// ✅ Fonction principale
int main() {
    WSADATA wsa;
    SOCKET server_fd, client_socket;
    struct sockaddr_in server_addr, client_addr;
    int addr_len = sizeof(client_addr);
    char request[4096];

    printf("🚀 Initialisation du serveur...\n");

    // --- 1️⃣ Initialiser Winsock ---
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("❌ Erreur d'initialisation Winsock : %d\n", WSAGetLastError());
        return 1;
    }

    // --- 2️⃣ Créer le socket serveur ---
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("❌ Erreur socket : %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // --- 3️⃣ Lier le socket au port 8080 ---
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("❌ Erreur bind : %d\n", WSAGetLastError());
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    // --- 4️⃣ Écouter les connexions entrantes ---
    if (listen(server_fd, 10) == SOCKET_ERROR) {
        printf("❌ Erreur listen : %d\n", WSAGetLastError());
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    printf("🌍 Serveur HTTP en ligne sur le port 8080 !\n");

    // --- 5️⃣ Boucle principale ---
    while (1) {
        client_socket = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);
        if (client_socket == INVALID_SOCKET) {
            printf("❌ Erreur accept : %d\n", WSAGetLastError());
            continue;
        }

        memset(request, 0, sizeof(request));
        recv(client_socket, request, sizeof(request) - 1, 0);
        printf("\n➡️  Requête reçue :\n%s\n", request);

        // --- 🔹 6️⃣ Gestion du CORS (OPTIONS) ---
        if (strncmp(request, "OPTIONS", 7) == 0) {
            send_cors_response(client_socket);

        // --- 🔹 Routage normal ---
        } else if (strncmp(request, "GET /candidates", 15) == 0) {
            handle_get_candidates(client_socket);

        } else if (strncmp(request, "POST /candidates", 16) == 0) {
            char *body = strstr(request, "\r\n\r\n");
            if (body) handle_add_candidate(client_socket, body + 4);

        } else if (strncmp(request, "GET /voters", 11) == 0) {
            handle_get_voters(client_socket);

        } else if (strncmp(request, "POST /voters", 12) == 0) {
            char *body = strstr(request, "\r\n\r\n");
            if (body) handle_add_voter(client_socket, body + 4);

        } else if (strncmp(request, "GET /voted?", 11) == 0) {
            int id;
            sscanf(request, "GET /voted?id=%d", &id);
            handle_has_voted(client_socket, id);

        } else if (strncmp(request, "POST /vote", 10) == 0) {
            char *body = strstr(request, "\r\n\r\n");
            if (body) handle_vote(client_socket, body + 4);

        } else if (strncmp(request, "GET /results", 12) == 0) {
            handle_results(client_socket);

        } else if (strncmp(request, "GET /refunded", 13) == 0) {
            handle_refunded_candidates(client_socket);

        } else if (strncmp(request, "GET /rejected", 13) == 0) {
    handle_rejected_candidates(client_socket);
       }

        else {
            send_not_found(client_socket);
        }

        closesocket(client_socket);
    }

    closesocket(server_fd);
    WSACleanup();
    return 0;
}
