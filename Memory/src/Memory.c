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

typedef struct memory_configuracion {
	int PUERTO;
	int MARCOS;
	int MARCOS_SIZE;
	int ENTRADAS_CACHE;
	int CACHE_X_PROC;
	int RETARDO_MEMORIA;
} memory_configuracion;

int MAXIMO_TAMANO_DATOS = 100;

char get_campo_config_char(t_config* archivo_configuracion, char* nombre_campo) {
	char* valor;
	if(config_has_property(archivo_configuracion, nombre_campo)){
		valor = config_get_string_value(archivo_configuracion, nombre_campo);
		printf("El %s es: %s\n", nombre_campo, valor);
		return valor;
	}
	return NULL;
}

int get_campo_config_int(t_config* archivo_configuracion, char* nombre_campo) {
	int valor;
	if(config_has_property(archivo_configuracion, nombre_campo)){
		valor = config_get_int_value(archivo_configuracion, nombre_campo);
		printf("El %s es: %i\n", nombre_campo, valor);
		return valor;
	}
	return NULL;
}

int get_campo_config_array(t_config* archivo_configuracion, char* nombre_campo) {
	char** valor;
	if(config_has_property(archivo_configuracion, nombre_campo)){
		valor = config_get_array_value(archivo_configuracion, nombre_campo);
		printf("El %s es: %s\n", nombre_campo, valor);
		return valor;
	}
	return NULL;
}

char get_campo_config_string(t_config* archivo_configuracion, char* nombre_campo) {
	char* valor;
	if(config_has_property(archivo_configuracion, nombre_campo)){
		valor = config_get_string_value(archivo_configuracion, nombre_campo);
		printf("El %s es: %s\n", nombre_campo, valor);
		return valor;
	}
	return NULL;
}

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
	return configuracion;
}

int main(void) {

	memory_configuracion configuracion = get_configuracion();
	/*
	struct sockaddr_in direccionServidor;

	char ip[20];
	char port[15];
	int host;

	FILE *archivoConfig;
	archivoConfig= fopen("/home/utnso/workspace/tp-2017-1c-AsadoClash/Memory/src/configMemory", "r");
	fgets(&ip, sizeof(ip)+1, archivoConfig);
	fgets(&port, sizeof(port)+1, archivoConfig);


	//printf(conf);
	//printf(ip);
	//printf(port);

	fclose(archivoConfig);

	direccionServidor.sin_family = AF_INET;
	direccionServidor.sin_addr.s_addr = inet_addr("127.0.0.1");
	direccionServidor.sin_port = htons(8888);

	int cliente = socket(AF_INET, SOCK_STREAM, 0);
	if (connect(cliente, (void*) &direccionServidor, sizeof(direccionServidor)) != 0) {
		perror("No se pudo conectar");
		return 1;
	}

	char *bienvenida = "hola soy [inserte nombre]";
	send(cliente, bienvenida, strlen(bienvenida), 0);
	//free(bienvenida);

	while (1) {
		char mensaje[1000];

		recv(cliente, mensaje, 100, 0);
		printf("%c /n", mensaje);

		scanf("%s", mensaje);
		send(cliente, mensaje, strlen(mensaje), 0);
	}

	*/
	return 0;
}
