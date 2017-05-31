
#include <socketConfig.h>
#include <commons/config.h>

#define TRUE   1
#define FALSE  0

//ESTRUCTURA ARCHIVO CONFIGURACION
typedef struct kernell_configuracion {
	char* PUERTO_PROG;
	char* PUERTO_CPU;
	char* IP_MEMORIA;
	char* PUERTO_MEMORIA;
	char* IP_FS;
	char* PUERTO_FS;
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
	configuracion.PUERTO_PROG = get_campo_config_string(archivo_configuracion, "PUERTO_PROG");
	configuracion.PUERTO_CPU = get_campo_config_string(archivo_configuracion, "PUERTO_CPU");
	configuracion.IP_MEMORIA = get_campo_config_string(archivo_configuracion, "IP_MEMORIA");
	configuracion.PUERTO_MEMORIA = get_campo_config_string(archivo_configuracion, "PUERTO_MEMORIA");
	configuracion.IP_FS = get_campo_config_string(archivo_configuracion, "IP_FS");
	configuracion.PUERTO_FS = get_campo_config_string(archivo_configuracion, "PUERTO_FS");
	configuracion.QUANTUM = get_campo_config_int(archivo_configuracion, "QUANTUM");
	configuracion.QUANTUM_SLEEP = get_campo_config_int(archivo_configuracion, "QUANTUM_SLEEP");
	configuracion.ALGORITMO = get_campo_config_string(archivo_configuracion, "ALGORITMO");
	configuracion.GRADO_MULTIPROG = get_campo_config_int(archivo_configuracion, "GRADO_MULTIPROG");
	configuracion.SEM_IDS = get_campo_config_array(archivo_configuracion, "SEM_IDS");
	configuracion.SEM_INIT = get_campo_config_array(archivo_configuracion, "SEM_INIT");
	configuracion.SHARED_VARS = get_campo_config_array(archivo_configuracion, "SHARED_VARS");
	configuracion.STACK_SIZE = get_campo_config_int(archivo_configuracion, "STACK_SIZE");
	return configuracion;
}

typedef struct tablaArchivosAbiertos{
	char* nombreArchivo;
	int descriptor;
	void* siguiente;
}tablaArchivosAbiertos;

typedef struct pcb{
	int PID;// Identificador del proceso
	int PC;// Program Counter
	tablaArchivosAbiertos* tablaDeArchivos;// Referencia a la tabla de Archivos del Proceso
	int SP;// Posicion del Stack
	int EC;// Exit Code
}pcb;

typedef struct listaDePCBs{
	pcb* pcb;
	void* siguiente;
}listaDePCBs;

int main(void){
	kernell_configuracion configuracion = get_configuracion();
	procesos procesos;

//--------------------CONEXION CON MEMORIA-----------------------------
	un_socket socketMemoria = conectar_a(configuracion.IP_MEMORIA,configuracion.PUERTO_MEMORIA);
	realizar_handshake(socketMemoria, cop_handshake_kernel);
	proceso_memoria nodo_memoria;
	procesos.memoria = nodo_memoria;

//---------------------------------------------------------------------


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
	direccionServidor.sin_port = htons(configuracion.PUERTO_PROG);

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
						esperar_handshake(socketActual, paqueteRecibido, cop_handshake_kernel);
						proceso_consola* nuevo_nodo_consola = malloc(sizeof(proceso_consola));
						proceso_consola* auxiliar_consola;
						proceso_consola* ultimo_nodo_consola;
						auxiliar_consola = procesos.consolas;
						while(auxiliar_consola != NULL){
							ultimo_nodo_consola = auxiliar_consola;
							auxiliar_consola = (proceso_consola*)auxiliar_consola->siguiente;
						}
						if(ultimo_nodo_consola == NULL){
							procesos.consolas = nuevo_nodo_consola;
						}else{
							ultimo_nodo_consola->siguiente = nuevo_nodo_consola;
						}
						nuevo_nodo_consola->habilitado = true;
						nuevo_nodo_consola->socket = socketActual;
				    break;

					case cop_handshake_cpu:
						esperar_handshake(socketActual, paqueteRecibido, cop_handshake_kernel);
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




void *get_in_addr(struct sockaddr *sa){
	if (sa->sa_family == AF_INET) {return &(((struct sockaddr_in*)sa)->sin_addr);}
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}


	return 0;
}
