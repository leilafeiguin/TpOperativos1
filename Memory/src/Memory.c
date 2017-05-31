#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <signal.h>
#include <commons/log.h>
#include <commons/collections/list.h>
#include <commons/config.h>
#include <socketConfig.h>

#define TRUE   1
#define FALSE  0

typedef struct memory_configuracion {
	int PUERTO;
	int MARCOS;
	int MARCOS_SIZE;
	int ENTRADAS_CACHE;
	int CACHE_X_PROC;
	int RETARDO_MEMORIA;
} memory_configuracion;

void* bloqueMemoria;

memory_configuracion get_configuracion() {
	puts("Inicializando proceso Memory\n");
	memory_configuracion configuracion;
	// Obtiene el archivo de configuracion
	char* path = "/home/utnso/workspace/tp-2017-1c-AsadoClash/Memory/config-memory.cfg";
	t_config* archivo_configuracion = config_create(path);
	configuracion.PUERTO = get_campo_config_int(archivo_configuracion, "PUERTO");
	configuracion.MARCOS = get_campo_config_int(archivo_configuracion, "MARCOS");
	configuracion.MARCOS_SIZE = get_campo_config_int(archivo_configuracion, "MARCOS_SIZE");
	configuracion.ENTRADAS_CACHE = get_campo_config_int(archivo_configuracion, "ENTRADAS_CACHE");
	configuracion.CACHE_X_PROC = get_campo_config_int(archivo_configuracion, "CHACHE_X_PROC");
	configuracion.RETARDO_MEMORIA = get_campo_config_int(archivo_configuracion, "RETARDO_MEMORIA");

	bloqueMemoria = malloc(configuracion.MARCOS * configuracion.MARCOS_SIZE);
	return configuracion;
}

int main(void) {
	memory_configuracion configuracion = get_configuracion();
	procesos procesos;

	struct sockaddr_storage remoteaddr; // client address
	socklen_t addrlen;
	int nbytes;
	char remoteIP[INET6_ADDRSTRLEN];
	int yes=1;        // for setsockopt() SO_REUSEADDR, below
	int socketActual, j, rv;
	struct addrinfo hints, *ai, *p;
	struct sockaddr_in direccionServidor;
	fd_set listaOriginal;
	fd_set listaAux;
	FD_ZERO(&listaOriginal);
	FD_ZERO(&listaAux);
	int fd_max = 1;
	int newfd;

	direccionServidor.sin_family = AF_INET;
	direccionServidor.sin_addr.s_addr = inet_addr("127.0.0.1");
	direccionServidor.sin_port = htons(configuracion.PUERTO);

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
		for(socketActual = 0; socketActual <= fd_max; socketActual++) {
			if (FD_ISSET(socketActual, &listaAux)) {
				if (socketActual == socketServer) { //es una conexion nueva
					newfd = aceptar_conexion(socketActual);
					FD_SET(newfd, &listaOriginal); //Agregar al master SET
					if (newfd > fd_max) {    //Update el Maximo
						fd_max = newfd;
					}
		            printf("Memoria recibio una nueva conexion\n");

				} else {
					t_paquete* paqueteRecibido = recibir(socketActual);
					switch(paqueteRecibido->codigo_operacion){ //revisar validaciones de habilitados
						case cop_handshake_kernel:
							esperar_handshake(socketActual, paqueteRecibido, cop_handshake_memoria);
							proceso_kernel nuevo_nodo_kernel;
							nuevo_nodo_kernel.socket = socketActual;
							procesos.kernel = nuevo_nodo_kernel;
					    break;

						case cop_handshake_cpu:
							esperar_handshake(socketActual, paqueteRecibido, cop_handshake_memoria);
							proceso_cpu* nuevo_nodo_cpu = malloc(sizeof(proceso_cpu));
							proceso_cpu* auxiliar_cpu;
							proceso_cpu* ultimo_nodo_cpu;
							auxiliar_cpu = procesos.cpus;
							while(auxiliar_cpu != NULL){
								ultimo_nodo_cpu = auxiliar_cpu;
								auxiliar_cpu = (proceso_cpu*)auxiliar_cpu->siguiente;
							}
							if(ultimo_nodo_cpu == NULL){
								procesos.cpus = nuevo_nodo_cpu;
							}else{
								ultimo_nodo_cpu->siguiente = nuevo_nodo_cpu;
							}
							nuevo_nodo_cpu->habilitado = true;
							nuevo_nodo_cpu->socket = socketActual;
						break;
					}
				}
			}
			listaAux = listaOriginal;
		}// for
	}//while


	/*chau
	socketKernel = aceptar_conexion(socketServer);
	t_paquete* paqueteRecibido = recibir(socketKernel);
	esperar_handshake(socketKernel, paqueteRecibido, cop_handshake_memoria);
	*/

	return 0;
}
