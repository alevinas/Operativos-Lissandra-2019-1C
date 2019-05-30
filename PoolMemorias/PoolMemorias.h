/*
 * PoolMemorias.h
 *
 *  Created on: 11 abr. 2019
 *      Author: utnso
 */

#ifndef POOLMEMORIAS_H_
#define POOLMEMORIAS_H_


#include <commons/sockets.h>
#include<commons/log.h>
#include<commons/config.h>
#include<commons/collections/list.h>
#include<commons/parser.h>

t_log* g_logger;
t_config* g_config;

t_list* tabla_segmentos;
t_list* tabla_paginas;

int posLibres;


#define TAM_VALUE 20
#define TAM_MEMORIA_PRINCIPAL 100
#define NOMBRE_TABLA 7

typedef struct
{
	char value[TAM_VALUE];
	int key;
	long timestamp;
} Registro;


Registro* memoria;
bool* bitmap;
int cantidadFrames;

typedef struct
{
	int numero_pagina;
	int indice_registro;
	int flag_modificado;

} Pagina;

typedef struct
{
	int numero_segmento;
	char *nombre_tabla;//[NOMBRE_TABLA];
	t_list* puntero_tpaginas;

} Segmento;

typedef enum
{
	OK,
	SALIR,
	MENSAJE_MAL_FORMATEADO,
	ERROR
}estado;

typedef struct
{
	estado resultado;
	char* mensaje;
} resultado;

void iniciar_programa(void);
void terminar_programa(void);
void gestionarConexion(void);
void destroy_nodo_pagina(void *);
void destroy_nodo_segmento(void *);
void iniciar_tablas();

resultado select_t(char *nombre_tabla,int key);
int contieneRegistro(char *nombre_tabla,int key, Pagina* pagina);
bool encuentraSegmento(char *ntabla,Segmento *segmento);
bool encuentraPagina(Segmento segmento,int key, Pagina* pagina);
Registro pedirAlLFS(char* nombre_tabla, int key);
int espacioLibre();
void almacenarRegistro(char *nombre_tabla,Registro registro, int posLibre);
Segmento *agregarSegmento(char *nombre_tabla);
void agregarPagina(Registro registro, Segmento *segmento, int posLibre);
void iniciarReemplazo(char *nombre_tabla,Registro registro);
void guardarEnMemoria(Registro registro, int posLibre);
Pagina* paginaMenosUsada(Segmento** segmento);
void cambiarNumerosPaginas(t_list* listaPaginas);
bool memoriaFull();
void journal();
resultado insert(char *nombre_tabla,int key,char *value);
void actualizarRegistro(Pagina *pagina,char *value);
resultado parsear_mensaje(char *);




char* PUERTO_M;
char* PUERTO;
char* IP_M;

/*
void gestionarConexionEntrante();
int iniciarServidor();
int esperarCliente(int listenningSocket);
void* recibir_mensaje(int socketCliente);
int iniciarCliente();
int gestionarConexionSaliente();
int enviar_mensaje(int socket_cliente, char *buffer);
*/

#endif /* POOLMEMORIAS_H_ */
