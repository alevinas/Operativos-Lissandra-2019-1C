/*
 * Globales.h
 *
 *  Created on: 14 jul. 2019
 *      Author: utnso
 */

#ifndef GLOBALES_H_
#define GLOBALES_H_

// Semaforos
t_dictionary* mutexsConexiones;
pthread_mutex_t usoConfig;

//las puse aca para que no me rompa, buscar un lugar mejor
t_log* g_logger;
t_config* g_config;

int memoriaSocket;			// Almaceno el socket de la memoria para usarlo posteriormente
t_list* socketsPool;

typedef struct{
	char* ipMemoria;
	char* puerto;
	uint32_t id;
	t_dictionary* conexiones;
	int totalOperaciones;
	int selectsTotales;
	int insertsTotales;
	pthread_mutex_t mutex;
	uint16_t estado;
	uint64_t timestamp;
}Memoria;

typedef enum{SC, SHC, EC} t_consist;

typedef struct{
	t_consist tipo;
	t_list *memorias;
	uint64_t timeTotalReads;
	uint64_t timeTotalWrites;
	int amountReads;
	int amountWrites;
	int amountTotales;
}Criterio;

Criterio sc,shc,ec;

// Pool de Memorias
t_list *pool;
Memoria* MemDescribe;

char* idGossiping;

t_list* idsEjecutadores;

t_list* criterios;

#endif /* GLOBALES_H_ */
