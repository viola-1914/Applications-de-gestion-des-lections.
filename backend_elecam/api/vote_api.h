#ifndef VOTE_API_H
#define VOTE_API_H

#include <mysql.h>
#include <winsock2.h>

// ✅ Structure pour représenter un vote
typedef struct {
    int id_electeur;
    int id_candidat;
} Vote;

// --- Prototypes des fonctions API ---
void handle_vote(SOCKET client_socket, const char *body);
void handle_results(SOCKET client_socket);
void handle_refunded_candidates(SOCKET client_socket);

#endif
