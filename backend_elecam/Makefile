CC = gcc
CFLAGS = -I/mingw64/include/mariadb
LDFLAGS = -L/mingw64/lib -lmariadb -lws2_32

SRC = server.c db/db_utils.c api/candidates_api.c api/voters_api.c api/vote_api.c utils/json_utils.c utils/http_utils.c
OBJ = $(SRC:.c=.o)

server: $(OBJ)
	$(CC) $(OBJ) -o server $(LDFLAGS) -mconsole

clean:
	rm -f $(OBJ) server
