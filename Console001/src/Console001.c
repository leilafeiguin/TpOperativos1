
#include <socketConfig.h>
#include <pthread.h>

//ESTRUCTURA ARCHIVO CONFIGURACION
typedef struct console_configuracion {
	char* IP_KERNEL;
	char* PUERTO_KERNEL;
} console_configuracion;

console_configuracion get_configuracion() {
	puts("Inicializando proceso Console\n");
	console_configuracion configuracion;
	// Obtiene el archivo de configuracion
	char* path = "/home/utnso/workspace/tp-2017-1c-AsadoClash/Console001/config-console.cfg";
	t_config* archivo_configuracion = config_create(path);

	configuracion.IP_KERNEL = get_campo_config_string(archivo_configuracion, "IP_KERNEL");
	configuracion.PUERTO_KERNEL = get_campo_config_string(archivo_configuracion, "PUERTO_KERNEL");
	return configuracion;
}

signed int kernel;
console_configuracion configuracion;

enum instrucciones{
	iniciarPrograma = 1,
	finalizarPrograma = 2,
	desconectarConsola = 3,
	limpiarConsola = 4
};

typedef struct t_programa{
	int processID;
	int threadID;
	void* siguiente;
}t_programa;

typedef struct programas{
	t_programa* programa;
}programas;

programas programasActuales;

void hiloPrograma(char* path);


int main(void) {
	configuracion = get_configuracion();
	while(1){
		printf("Ingrese la opcion deseada:\n1) Iniciar Programa\n2) Finalizar Programa\n3) Desconectar Consola\n4) Limpiar Mensajes\n");
		int seleccion;
		scanf("%i\n",seleccion);
		switch(seleccion){
			case iniciarPrograma:
				printf("Ingrese el path del archivo a ejecutar:");
				char* path; //hace falta malloc?
				scanf("%s\n",path);
				if(comprobar_archivo(path)){
					//como logro que identifique a cada hilo de forma distinta
					//pthread_create();
				}
				break;
			case finalizarPrograma:
				printf("Ingrese el path del archivo a ejecutar:");
				char* path;
				scanf("%s\n",path);
				if(comprobar_archivo(path)){
					//exit();
				}
				break;
			case desconectarConsola:
				pthread_exit();
				//finalizar todos los hilos
				break;
			case limpiarConsola:
				System("clear");
				break;
		}
	}


	return 0;
}

//-------------------------HILO PROGRAMA--------------------------------------
void hiloPrograma(char* path){
	un_socket kernel = conectar_a(configuracion.IP_KERNEL,configuracion.PUERTO_KERNEL);
	realizar_handshake(kernel, cop_handshake_consola);
	enviar_archivo(kernel, path);

	t_paquete* pid = recibir(kernel);
	t_programa* nuevo_nodo = malloc(sizeof(t_programa));
	t_programa* auxiliar;
	t_programa* ultimo_nodo;

	auxiliar = programasActuales.programa;
	while(auxiliar != NULL){
		ultimo_nodo = auxiliar;
		auxiliar = (t_programa*)auxiliar->siguiente;
	}
	if(ultimo_nodo == NULL){
		programasActuales.programa = nuevo_nodo;
	}else{
		ultimo_nodo->siguiente = nuevo_nodo;
	}
	nuevo_nodo->processID = (int)pid->data;
	nuevo_nodo->threadID = (int)pthread_self();

	while(1){
		t_paquete* paqueteRecibido = recibir(kernel);
		switch(paqueteRecibido->codigo_operacion){
			case cop_imprimi:
				printf("%s\n",(char*)paqueteRecibido->data);
				break;
			case cop_terminar_proceso:
				//proceso que implica limpiarse de la lista de programas
				//instruccion para terminarse a el mismo? esto liberaria el paquete en el proceso?
				break;
		}
		liberar_paquete(paqueteRecibido);
	}


}


