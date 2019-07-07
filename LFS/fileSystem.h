/*
 * fileSystem.h
 *
 *  Created on: 28 abr. 2019
 *      Author: utnso
 */

#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

#include "fileSystemPropio.h"
#include "LFS.h"
#include <dirent.h>
#include <sys/types.h>
#include <semaphore.h>

t_list* listaBloqueos;

typedef struct
{
	char* tabla;
	int bloqueo;
} nodo_bloqueo;


metadataTabla obtenerMetadata(char* nombreTabla);
int	existeMetadata(char* nombreTabla);
char* obtenerMetadataPath(char* nombreTabla);
char* obtenerTablePath();
registro* fs_select(char* nombreTabla, int key, int partition);
registro* fs_select_partition(char* nombreTabla, int key, int partition);
registro* fs_select_temporal(char* nombreTabla, int key);
registro* obtenerRegistroDeArchivo(FILE* file, int key);
t_list* obtenerTodasMetadata();
int crear_tabla(char* tabla,char* t_cons,int cant_part,int tiempo_comp);
int crearArchivoMetadata(char* tablePath,metadataTabla metadata);
int crearArchivosBinarios(char* tablesPath,metadataTabla metadata);
int dropTableFS(char * tabla);
int fs_create_tmp(char* tabla,t_list* regList);
void compactarTabla(char* tabla);
void compactar();

void setearBloqueo(char* tabla);
void liberarBloqueo(char* tabla);
int consultarBloqueo(char* tabla);
void esperarBloqueo(char* tabla);
int consultarAlgunBloqueo();
void esperarAlgunBloqueo();

#endif /* FILESYSTEM_H_ */
