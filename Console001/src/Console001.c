#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>

int main(void) {
	struct sockaddr_in direccionServidor;
	char ip[20];
	int host;

	FILE *archivoConfig;
	archivoConfig= fopen("configuracionConsole001", "r");
	fgets(&ip, sizeof(ip)+1, archivoConfig);
	printf(ip);
	//fgets(direccionServidor.sin_family, sizeof(direccionServidor.sin_family), archivoConfig);
	//fgets(ip, sizeof(ip),archivoConfig);
	//direccionServidor.sin_addr.s_addr = inet_addr(ip);
	//fgets(host, sizeof(host),archivoConfig);
	//direccionServidor.sin_port = htons(host);

	fclose(archivoConfig);

	//direccionServidor.sin_family = AF_INET;
	//direccionServidor.sin_addr.s_addr = inet_addr("127.0.0.1");
	//direccionServidor.sin_port = htons(1234);

	int cliente = socket(AF_INET, SOCK_STREAM, 0);
	if (connect(cliente, (void*) &direccionServidor, sizeof(direccionServidor)) != 0) {
		perror("No se pudo conectar");
		return 1;
	}

	while (1) {
		char mensaje[1000];
		scanf("%s", mensaje);

		send(cliente, mensaje, strlen(mensaje), 0);
	}

	return 0;
}
