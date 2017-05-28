#include <socketConfig.h>
#include <commons/config.h>


typedef struct fileSystem_configuracion {
	int PUERTO;
	char* PUNTO_MONTAJE;
} fileSystem_configuracion;


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

	un_socket kernel = socket_escucha("127.0.0.1", configuracion.PUERTO);
	aceptar_conexion(kernel);
	t_paquete* handshake = recibir(kernel);
	esperar_handshake(kernel, handshake,cop_handshake_fileSystem);
	free(handshake);
	return 0;
}
