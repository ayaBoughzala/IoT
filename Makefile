all: SM.c MDMS.c 
	gcc -o SMconnect SM.c  -I. -lcurl -lpaho-mqtt3c -lpthread;
	gcc -o MDMSconnect MDMS.c -I. -lcurl -lpaho-mqtt3c -lpthread;

