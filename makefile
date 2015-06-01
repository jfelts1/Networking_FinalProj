all: 
	gcc -ggdb -Wall -Wextra -o "server" server.c fileUtil.c commonFunctions.c -lpthread
	gcc -ggdb -Wall -Wextra -o "client" client.c commonFunctions.c -lpthread
	
server:
	gcc -ggdb -Wall -Wextra -o "server" server.c fileUtil.c commonFunctions.c -lpthread
client:
	gcc -ggdb -Wall -Wextra -o "client" client.c commonFunctions.c -lpthread
