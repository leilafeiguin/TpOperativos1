/*
 * primitivas.c
 *
 *  Created on: 30/5/2017
 *      Author: utnso
 */

#ifndef PRIMITIVAS_C_
#define PRIMITIVAS_C_

#include "primitivas.h"

static const int CONTENIDO_VARIABLE = 20;
static const int POSICION_MEMORIA = 0x10;
bool termino = false;

t_puntero definirVariable(t_nombre_variable variable) {
	printf("definir la variable %c\n", variable);
	return POSICION_MEMORIA;
}

t_puntero obtenerPosicionVariable(t_nombre_variable variable) {
	printf("Obtener posicion de %c\n", variable);
	return POSICION_MEMORIA;
}

void finalizar(void){
	termino = true;
	printf("Finalizar\n");
}

bool terminoElPrograma(void){
	return termino;
}

t_valor_variable dereferenciar(t_puntero puntero) {
	printf("Dereferenciar %d y su valor es: %d\n", puntero, CONTENIDO_VARIABLE);
	return CONTENIDO_VARIABLE;
}

void asignar(t_puntero puntero, t_valor_variable variable) {
	printf("Asignando en %d el valor %d\n", puntero, variable);
}

#endif /* PRIMITIVAS_C_ */
