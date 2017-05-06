
#include <socketConfig.h>
#include <commons/config.h>


typedef struct console_configuracion {
	char* IP_KERNEL;
	int PUERTO_KERNEL;
} console_configuracion;

signed int kernel;

console_configuracion get_configuracion() {
	puts("Inicializando proceso Console\n");
	console_configuracion configuracion;
	// Obtiene el archivo de configuracion
	char* path = "/home/utnso/workspace/tp-2017-1c-AsadoClash/Console001/config-console.cfg";
	t_config* archivo_configuracion = config_create(path);

	configuracion.IP_KERNEL = get_campo_config_char(archivo_configuracion, "IP_KERNEL");
	configuracion.PUERTO_KERNEL = get_campo_config_int(archivo_configuracion, "PUERTO_KERNEL");
	return configuracion;
}



int main(void) {
	console_configuracion configuracion = get_configuracion();

	struct sockaddr_in direccionServidor;

	direccionServidor.sin_family = AF_INET;
	direccionServidor.sin_addr.s_addr = inet_addr("127.0.0.1");
	direccionServidor.sin_port = htons(configuracion.PUERTO_KERNEL);

	un_socket cliente = socket(AF_INET, SOCK_STREAM, 0);
	if (connect(cliente, (void*) &direccionServidor, sizeof(direccionServidor)) != 0) {
		perror("No se pudo conectar");
		return 1;
	}
	printf("conecte\n");
	char* saludo = "hola";
	send(cliente, saludo, strlen(saludo)+1, 0);

/*
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



