
#include <socketConfig.h>
#include <commons/config.h>

#define TRUE   1
#define FALSE  0
#define PORT 8888

typedef struct kernell_configuracion {
	int PUERTO_PROG;
	int PUERTO_CPU;
	char* IP_MEMORIA;
	int PUERTO_MEMORIA;
	char* IP_FS;
	int PUERTO_FS;
	int QUANTUM;
	int QUANTUM_SLEEP;
	char* ALGORITMO;
	int GRADO_MULTIPROG;
	int STACK_SIZE;
	char** SEM_IDS;
	char** SEM_INIT;
	char** SHARED_VARS;
} kernell_configuracion;

kernell_configuracion get_configuracion() {
	printf("Inicializando proceso Kernel\n");
	kernell_configuracion configuracion;
	// Obtiene el archivo de configuracion
	char* path = "/home/utnso/workspace/tp-2017-1c-AsadoClash/Kernel/config-kernell.cfg";
	t_config* archivo_configuracion = config_create(path);

	configuracion.PUERTO_PROG = get_campo_config_int(archivo_configuracion, "PUERTO_PROG");
	configuracion.PUERTO_CPU = get_campo_config_int(archivo_configuracion, "PUERTO_CPU");
	configuracion.IP_MEMORIA = get_campo_config_char(archivo_configuracion, "IP_MEMORIA");
	configuracion.PUERTO_MEMORIA = get_campo_config_int(archivo_configuracion, "PUERTO_MEMORIA");
	configuracion.IP_FS = get_campo_config_char(archivo_configuracion, "IP_FS");
	configuracion.PUERTO_FS = get_campo_config_int(archivo_configuracion, "PUERTO_FS");
	configuracion.QUANTUM = get_campo_config_int(archivo_configuracion, "QUANTUM");
	configuracion.QUANTUM_SLEEP = get_campo_config_int(archivo_configuracion, "QUANTUM_SLEEP");
	configuracion.ALGORITMO = get_campo_config_char(archivo_configuracion, "ALGORITMO");
	configuracion.GRADO_MULTIPROG = get_campo_config_int(archivo_configuracion, "GRADO_MULTIPROG");
	configuracion.SEM_IDS = get_campo_config_array(archivo_configuracion, "SEM_IDS");
	configuracion.SEM_INIT = get_campo_config_array(archivo_configuracion, "SEM_INIT");
	configuracion.SHARED_VARS = get_campo_config_array(archivo_configuracion, "SHARED_VARS");
	configuracion.STACK_SIZE = get_campo_config_int(archivo_configuracion, "STACK_SIZE");
	return configuracion;
}


int main(void){
	kernell_configuracion configuracion = get_configuracion();

	fd_set listaOriginal;
	fd_set listaAux;
	FD_ZERO(&listaOriginal);
	FD_ZERO(&listaAux);

	int fd_max = 1;

	int newfd;        // newly accept()ed socket descriptor
	struct sockaddr_storage remoteaddr; // client address
	socklen_t addrlen;

	int nbytes;

	char remoteIP[INET6_ADDRSTRLEN];

	int yes=1;        // for setsockopt() SO_REUSEADDR, below
	int i, j, rv;

	struct addrinfo hints, *ai, *p;

	struct sockaddr_in direccionServidor;

	direccionServidor.sin_family = AF_INET;
	direccionServidor.sin_addr.s_addr = inet_addr("127.0.0.1");
	direccionServidor.sin_port = htons(PORT);

	un_socket socketServer = socket(AF_INET, SOCK_STREAM, 0);
	setsockopt(socketServer, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
	int error=0;
	error=bind(socketServer, (struct sockaddr *) &direccionServidor, sizeof(direccionServidor));
	if(error <0)
		perror('error en bind');
	error=listen(socketServer, 256);
	if(error <0)
			perror('error en listen');

	FD_SET(socketServer,&listaOriginal);
	fd_max=socketServer;


while(1){
	listaAux = listaOriginal;
	select(fd_max + 1, &listaAux, NULL, NULL, NULL);
	for(i = 0; i <= fd_max; i++) {
		if (FD_ISSET(i, &listaAux)) {
			if (i == socketServer) { //es una conexion nueva

				newfd = aceptar_conexion(i);

				//addrlen = sizeof remoteaddr;
	            //newfd = accept(socketServer,(struct sockaddr *)&remoteaddr,&addrlen);
	            //if (newfd == -1) {
	            //	perror("accept");

				FD_SET(newfd, &listaOriginal); //Agregar al master SET
				if (newfd > fd_max) {    //Update el Maximo
					fd_max = newfd;
				}
	            printf("Kernel recibio una nueva conexion\n");

			} else {
				/*
				// manejamos informacion recibida
				if ((nbytes = recv(i, buf, 5, 0)) <= 0) {
	            // got error or connection closed by client
				if (nbytes == 0) {
					// connection closed
					printf("selectserver: socket %d hung up\n", i);
				} else {
					perror("recv");
				}
				close(i); // bye!
				FD_CLR(i, &listaOriginal); // remove from master set
			 	} else {
				*/
				// tenemos informacion del cliente
				char* bufer1[1024];
				bufer1[1024] = "";
				recv(i, bufer1[1024], strlen(bufer1[1024]), 0); //usar recibir
				printf("%c\n", bufer1); //porque no imprime correctamente
				bufer1[1024] = "";
			}
		}
		listaAux = listaOriginal;
	}// for
}//while




void *get_in_addr(struct sockaddr *sa){
	if (sa->sa_family == AF_INET) {return &(((struct sockaddr_in*)sa)->sin_addr);}
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

	return 0;
}
