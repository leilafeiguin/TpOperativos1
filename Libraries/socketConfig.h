/*
 * socketConfig.h
 *
 *  Created on: 6/5/2017
 *      Author: utnso
 */

#ifndef SOCKETCONFIG_H_
#define SOCKETCONFIG_H_

#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdint.h>
#include <stdio.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>
#include <commons/string.h>
#include <commons/error.h>
#include <commons/config.h>
#include <unistd.h>

int MAXIMO_TAMANO_DATOS = 100;

enum codigos_de_operacion {
	cop_generico = 0,
	cop_handshake_consola = 1,
	cop_handshake_cpu = 2,
	cop_handshake_memoria = 3,
	cop_handshake_fileSystem = 4,
	cop_handshake_kernel = 5,
	cop_archivo_programa = 6,
	cop_imprimi = 7,
	cop_terminar_proceso = 8,
	cop_handshake_consolaInterfazUsuario = 9,
	cop_obtener_direccion = 10
//cop_envio_pid = , es necesario o podemos asumir con seguridad que el kernel nos va a mandar el pid?
};

typedef int un_socket;
typedef struct {
	int codigo_operacion;
	int tamanio;
	void * data;
} t_paquete;

enum estados_proceso {
	estado_new = 0,
	estado_ready = 1,
	estado_exit = 2,
	estado_exec = 3,
	estado_block = 4
};

//------------ESTRUCTURAS PROCESOS----------------
typedef struct proceso_consola {
	bool habilitado;
	int socket;
	void* siguiente;
} proceso_consola;

typedef struct proceso_cpu {
	bool habilitado;
	int socket;
	void* siguiente;
} proceso_cpu;

typedef struct proceso_memoria {
	int socket;
} proceso_memoria;

typedef struct proceso_fileSystem {
	int socket;
} proceso_fileSystem;

typedef struct proceso_kernel {
	int socket;
} proceso_kernel;

typedef struct procesos {
	proceso_consola* consolas;
	proceso_cpu* cpus;
	proceso_memoria memoria;
	proceso_fileSystem fileSystem;
	proceso_kernel kernel;
} procesos;
//------------FIN ESTRUCTURAS PROCESOS----------------

/**	@NAME: conectar_a
 * 	@DESC: Intenta conectarse.
 * 	@RETURN: Devuelve el socket o te avisa si hubo un error al conectarse.
 *
 */

un_socket conectar_a(char *IP, char* Port);

/**	@NAME: socket_escucha
 * 	@DESC: Configura un socket que solo le falta hacer listen.
 * 	@RETURN: Devuelve el socket listo para escuchar o te avisa si hubo un error al conectarse.
 *
 */

un_socket socket_escucha(char* IP, char* Port);

/**	@NAME: enviar
 * 	@DESC: Hace el envio de la data que le pasamos. No hay que hacer más nada.
 *
 */

void enviar(un_socket socket_envio, int codigo_operacion, int tamanio,
		void * data);

/**	@NAME: recibir
 * 	@DESC: devuelve un paquete que está en el socket pedido
 *
 */
t_paquete* recibir(un_socket socket_para_recibir);

/**	@NAME: aceptar_conexion
 * 	@DESC: devuelve el socket nuevo que se quería conectar
 *
 */
un_socket aceptar_conexion(un_socket socket_escuchador);

/**	@NAME: liberar_paquete
 * 	@DESC: libera el paquete y su data.
 *
 */
void liberar_paquete(t_paquete * paquete);

/**	@NAME: realizar_handshake
 *
 */
bool realizar_handshake(un_socket socket_del_servidor, int cop_handshake);

/**	@NAME: esperar_handshake
 *
 */
bool esperar_handshake(un_socket socket_del_cliente,
		t_paquete* inicio_del_handshake, int cop_handshake);

char get_campo_config_char(t_config* archivo_configuracion, char* nombre_campo);

int get_campo_config_int(t_config* archivo_configuracion, char* nombre_campo);

char** get_campo_config_array(t_config* archivo_configuracion,
		char* nombre_campo);

char* get_campo_config_string(t_config* archivo_configuracion,
		char* nombre_campo);

/**	@NAME: enviar_archivo
 *
 */
void enviar_archivo(un_socket socket, char* path);

bool comprobar_archivo(char* path);

#endif /* SOCKETCONFIG_H_ */
