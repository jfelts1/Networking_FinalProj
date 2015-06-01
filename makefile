all:
	gcc -ggdb -Wall -Wextra -o "server" server.c fileUtil.c -lpthread
	gcc -ggdb -Wall -Wextra -o "client" client.c -lpthread
