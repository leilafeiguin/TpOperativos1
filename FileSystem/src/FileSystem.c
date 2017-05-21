#include <socketConfig.h>
#include <commons/config.h>


typedef struct fileSystem_configuracion {
	int PUERTO;
	char* PUNTO_MONTAJE;
} fileSystem_configuracion;

int MAXIMO_TAMANO_DATOS = 100;

	fileSystem_configuracion get_configuracion() {
	puts("Inicializando proceso File System\n");
	fileSystem_configuracion configuracion;
	// Obtiene el archivo de configuracion
	char* path = "/home/utnso/workspace/tp-2017-1c-AsadoClash/FileSystem/config-fileSystem.cfg";
	t_config* archivo_configuracion = config_create(path);

	configuracion.PUERTO = get_campo_config_int(archivo_configuracion, "PUERTO");
	configuracion.PUNTO_MONTAJE = get_campo_config_string(archivo_configuracion, "PUNTO_MONTAJE");
	return configuracion;
}


int main(void) {
	fileSystem_configuracion configuracion = get_configuracion();

	struct sockaddr_in direccionServidor;

	direccionServidor.sin_family = AF_INET;
	direccionServidor.sin_addr.s_addr = inet_addr("127.0.0.1");
	direccionServidor.sin_port = htons(configuracion.PUERTO);


	un_socket cliente = socket(AF_INET, SOCK_STREAM, 0);
	bind(cliente, (struct sockaddr *)&direccionServidor, sizeof direccionServidor);
	if (connect(cliente, (void*) &direccionServidor, sizeof(direccionServidor)) != 0) {
		perror("No se pudo conectar");
		return 1;
	}
	printf("conecte\n");
	char* saludo = "Consola";
	send(cliente, saludo, strlen(saludo)+1, 0);
	sleep(1000000000);

	return 0;
}
