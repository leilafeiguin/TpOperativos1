
#include <socketConfig.h>
#include <pthread.h>

typedef struct console_configuracion {
	char* IP_KERNEL;
	char* PUERTO_KERNEL;
} console_configuracion;

signed int kernel;
console_configuracion configuracion;

#define iniciarPrograma 1
#define finalizarPrograma 2
#define desconectarConsola 3
#define limpiarConsola 4

void programa();

console_configuracion get_configuracion() {
	puts("Inicializando proceso Console\n");
	console_configuracion configuracion;
	// Obtiene el archivo de configuracion
	char* path = "/home/utnso/workspace/tp-2017-1c-AsadoClash/Console001/config-console.cfg";
	t_config* archivo_configuracion = config_create(path);

	configuracion.IP_KERNEL = get_campo_config_string(archivo_configuracion, "IP_KERNEL");
	configuracion.PUERTO_KERNEL = get_campo_config_string(archivo_configuracion, "PUERTO_KERNEL");
	//configuracion.PUERTO_INTERFAZ = get_campo_config_string(archivo_configuracion, "PUERTO_INTERFAZ");
	return configuracion;
}



int main(void) {
	configuracion = get_configuracion();
	while(1){
		printf("Ingrese la opcion deseada:\n1) Iniciar Programa\n2) Finalizar Programa\n3) Desconectar Consola\n4) Limpiar Mensajes\n");
		int seleccion;
		scanf("%i\n",seleccion);
		switch(seleccion){
			case iniciarPrograma:
				//pthread_create();
				//un_socket programa = conectar_a("127.0.0.1", configuracion.PUERTO_INTERFAZ);
				break;
			case finalizarPrograma:
				break;
			case desconectarConsola:
				break;
			case limpiarConsola:
				break;
		}
	}


	return 0;
}

void programa(){
	struct sockaddr_in direccionServidor;

	direccionServidor.sin_family = AF_INET;
	direccionServidor.sin_addr.s_addr = inet_addr("127.0.0.1");
	//direccionServidor.sin_port = htons(configuracion.PUERTO_INTERFAZ); //agregar al archivo config

	int fd_max = 1;
	int yes=1;

	un_socket socketServer = socket(AF_INET, SOCK_STREAM, 0);
	setsockopt(socketServer, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
	int error=0;
	error=bind(socketServer, (struct sockaddr *) &direccionServidor, sizeof(direccionServidor));
	if(error <0){perror('error en bind');}
	error=listen(socketServer, 256);
	if(error <0){perror('error en listen');}

	fd_set listaOriginal;
	FD_ZERO(&listaOriginal);
	FD_SET(socketServer,&listaOriginal);
	fd_max=socketServer;

	un_socket kernel = conectar_a(configuracion.IP_KERNEL,configuracion.PUERTO_KERNEL);
	realizar_handshake(kernel, cop_handshake_consola);

	//select();

}


