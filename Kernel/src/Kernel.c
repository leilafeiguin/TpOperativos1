
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

typedef struct proceso_consola{
	bool habilitado;
	int socket;
	void* siguiente;
}proceso_consola;
//replicar para el resto
typedef struct procesos{
	proceso_consola* consolas;

}procesos;

int main(void){
	kernell_configuracion configuracion = get_configuracion();

	procesos procesos;

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
	int socketActual, j, rv;

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
	for(socketActual = 0; socketActual <= fd_max; socketActual++) {
		if (FD_ISSET(socketActual, &listaAux)) {
			if (socketActual == socketServer) { //es una conexion nueva
				newfd = aceptar_conexion(socketActual);
				FD_SET(newfd, &listaOriginal); //Agregar al master SET
				if (newfd > fd_max) {    //Update el Maximo
					fd_max = newfd;
				}
	            printf("Kernel recibio una nueva conexion\n");

			} else {
				t_paquete* paqueteRecibido = recibir(socketActual);
				switch(paqueteRecibido->codigo_operacion){ //revisar validaciones de habilitados
					case cop_handshake_consola:
						esperar_handshake(socketActual, paqueteRecibido);
						proceso_consola* nuevo_nodo = malloc(sizeof(proceso_consola));
						proceso_consola* auxiliar;
						proceso_consola* ultimo_nodo;
						auxiliar = procesos.consolas;
						while(auxiliar != NULL){
							ultimo_nodo = auxiliar;
							auxiliar = (proceso_consola*)auxiliar->siguiente;
						}
						if(ultimo_nodo == NULL){
							procesos.consolas = nuevo_nodo;
						}else{
							ultimo_nodo->siguiente = nuevo_nodo;
						}
						nuevo_nodo->habilitado = true;
						nuevo_nodo->socket = socketActual;
				    break;
				}
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
