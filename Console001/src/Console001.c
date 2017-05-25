
#include <socketConfig.h>
#include <commons/config.h>


typedef struct console_configuracion {
	char* IP_KERNEL;
	char* PUERTO_KERNEL;
} console_configuracion;

signed int kernel;

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



int main(void) {
	console_configuracion configuracion = get_configuracion();

	struct sockaddr_in direccionServidor;

		direccionServidor.sin_family = AF_INET;
		direccionServidor.sin_addr.s_addr = inet_addr("127.0.0.1");
		direccionServidor.sin_port = htons(configuracion.PUERTO_KERNEL);

		un_socket cliente = conectar_a("127.0.0.1",configuracion.PUERTO_KERNEL);
		//realizar_handshake(cliente);
		esperar_handshake(cliente);

		while(1){
			t_paquete* paquete;
			paquete->data = "";
			scanf("%c\n",(char*)paquete->data);//no anda
			enviar(cliente,1,sizeof(paquete->data),paquete->data);
		}
return 0;
}



