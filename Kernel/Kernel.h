/*
 * Kernel.h
 *
 *  Created on: 11 abr. 2019
 *      Author: utnso
 */

#ifndef KERNEL_H_
#define KERNEL_H_

#include <commons/collections/queue.h>
#include <commons/parser.h>
#include <commons/serializacion.h>
#include <commons/log.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <semaphore.h>
#include <pthread.h>
#include "Estados.h"
#include "Request.h"

char* IP;
char* PUERTO;

int quantum; 					// Cantidad de Scripts en el estado EXEC
int nivelMultiprocesamiento;    // Cuantos procesos (hilos) voy a ejecutar
int nivelActual;
int metadataRefresh;
int retardoGossiping;

pthread_t plp; // Planificador a largo plazo
pthread_t describeGlobal; // Describe global
pthread_t gossipingAutomatico;
pthread_t* executer;

resultado finalizar; // Variable de corte

//////////// SOCKETS ////////////
void iniciar_programa(void);
void terminar_programa(void);
int enviar_mensaje(int socket_cliente);
int iniciarCliente();
void gestionarConexionAMemoria(Memoria *mem);
void establecerConexionPool();

//////////// CONSOLA Y PLANIFICADORES ////////////
void leerConsola();
void planificadorLargoPlazo();
void ejecutador();
void mandarAready(Script *s);
void mandarAexit(Script *s);
bool deboSalir(Script *s);

// DESCRIBE GLOBAL
void realizarDescribeGlobal();

void realizarGossipingAutomatico();

#endif /* KERNEL_H_ */
