#include <socketConfig.h>
#include <commons/config.h>


typedef struct fileSystem_configuracion {
	int PUERTO;
	char* IP_KERNEL;
	char* PUNTO_MONTAJE;
} fileSystem_configuracion;


	fileSystem_configuracion get_configuracion() {
	puts("Inicializando proceso File System\n");
	fileSystem_configuracion configuracion;
	// Obtiene el archivo de configuracion
	char* path = "/home/utnso/workspace/tp-2017-1c-AsadoClash/FileSystem/config-fileSystem.cfg";
	t_config* archivo_configuracion = config_create(path);

	configuracion.PUERTO = get_campo_config_int(archivo_configuracion, "PUERTO");
	configuracion.IP_KERNEL = get_campo_config_int(archivo_configuracion, "IP_KERNEL");
	configuracion.PUNTO_MONTAJE = get_campo_config_string(archivo_configuracion, "PUNTO_MONTAJE");
	return configuracion;
}


int main(void) {
	fileSystem_configuracion configuracion = get_configuracion();

	un_socket kernel = conectar_a(configuracion.IP_KERNEL,configuracion.PUERTO);
	realizar_handshake(kernel, cop_handshake_fileSystem);

	return 0;
}
