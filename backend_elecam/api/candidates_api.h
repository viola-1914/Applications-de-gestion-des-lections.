#ifndef CANDIDATES_API_H
#define CANDIDATES_API_H

#include <mysql.h>
#include <winsock2.h>

// ✅ Structure représentant un candidat
typedef struct {
    int id;
    char nom[100];
    char parti[100];
    int age;
    int votes;
    char nationalite[100];
    char url_image[255];
} Candidat;

// --- Prototypes des fonctions API ---
void handle_get_candidates(SOCKET client_socket);
void handle_add_candidate(SOCKET client_socket, const char *body);

#endif
