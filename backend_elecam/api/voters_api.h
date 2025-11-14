#ifndef VOTERS_API_H
#define VOTERS_API_H

#include <mysql.h>
#include <winsock2.h>

// ✅ Structure représentant un électeur
typedef struct {
    int id;
    char nom[100];
    char quartier[100];
    int age;
    int has_voted;
    int voted_candidate_id;
} Electeur;

// --- Prototypes des fonctions API ---
void handle_get_voters(SOCKET client_socket);
void handle_add_voter(SOCKET client_socket, const char *body);
void handle_has_voted(SOCKET client_socket, int id);

#endif
