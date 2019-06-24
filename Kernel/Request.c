/*
 * Request.c
 *
 *  Created on: 23 jun. 2019
 *      Author: utnso
 */

#include "Request.h"

Script* run(char* path){
	printf("Ruta: %s\n", path);
	FILE* arch = fopen(path, "r+b");
	if(arch == NULL)
		perror("\nError:");
	else
		printf("Abro el archivo\n");

	Script* script = parsearScript(arch);

	fclose(arch);
	return script;
}

resultadoParser leerRequest(FILE* fd){
	char* linea=malloc(1); //VER BIEN ESTO IMPORTANTE
	size_t tamanioLeido;

	resultadoParser r;
	getline(&linea,&tamanioLeido,fd);//realoca linea y pone el tamaño leido
	printf("Tamaño linea Rq: %d\n",tamanioLeido);

	r = parseConsole(linea);

	free(linea);
	return r;
}

///////////////// Linea por consola /////////////////
resultadoParser leerLineaSQL(char* mensaje)
{
	resultadoParser r;
	r = parseConsole(mensaje);
	return r;
}

Script* parsearScript(FILE* fd){
	Script* script = malloc(sizeof(Script));
	script->instrucciones = list_create();
	script->pc=0;

	while(!feof(fd)){
		resultadoParser* req = malloc(sizeof(resultadoParser));

		resultadoParser aux = leerRequest(fd);

		req->accionEjecutar = aux.accionEjecutar;
		req->contenido = aux.contenido;

		list_add(script->instrucciones,req);
	}
	printf("Script preparado, cantidad instrucciones: %d\n",script->instrucciones->elements_count);
	return script;
}


Script* crearScript(resultadoParser* r){
	Script* s;
	if(r->accionEjecutar==RUN)
	{
		char* path;
		path = ((contenidoRun*) r->contenido)->path;
		s = run(path);
	}
	else
	{
		s = malloc(sizeof(Script));

		s->instrucciones = list_create();
		s->pc = 0;
		list_add(s->instrucciones,r);
		printf("Arme el script de una linea\n");
	}
	return s;
}


bool terminoScript(Script *s){
	return s->pc == list_size(s->instrucciones);
}

status ejecutar(Criterio* criterio, resultadoParser* request){
	Memoria* mem = masApropiada(criterio);
	printf("Elegi mem: \n",mem->id);
	status resultado = enviarRequest(mem, request); 		// Seguramente se cambie status por una estructura Resultado dependiendo lo que devuelva
	return resultado;										// la memoria. enviarRequest está sin implementar, usa sockets.
}

status enviarRequest(Memoria* mem, resultadoParser* request)
{
	status result;
	resultado res;
	int size;

	printf("Hola\n");
	char* msg = serializarPaquete(request,&size);
	send(memoriaSocket, msg, size, 0);
	int resultadoMemoria = recibirYDeserializarRespuesta(memoriaSocket,&res);

	if(resultadoMemoria == -2 || resultadoMemoria == -1)
		result = REQUEST_ERROR;
	else
		result = REQUEST_OK;

	return result;
}

status ejecutarScript(Script *s){
	printf("Entro a ejecutarScript\n");

	resultadoParser *r = list_get(s->instrucciones,s->pc);
	status estado = ejecutarRequest(r);

	(s->pc)++;
	return estado;
}

status ejecutarRequest(resultadoParser *r){
	printf("Entro a ejecutarRequest\n");
	printf("Accion:%i\n",r->accionEjecutar);

	if(usaTabla(r)){
		Tabla* tabla = obtenerTabla(r);
		printf("UsoTabla %s\n",tabla->nombre);

		if(tabla != NULL){
			printf("Voy a ejecutar\n");
			printf("Criterio:%d\n",(tabla->criterio)->tipo);
			return ejecutar(tabla->criterio,r);
		}
		else
			return REQUEST_ERROR; 											// HACER UN ENUM
	}
	else{
		switch (r->accionEjecutar){
			case JOURNAL:
				//journal();
				break;
			case METRICS:
				//metrics();
				break;
			case ADD:
			{
				contenidoAdd* contenido = (contenidoAdd *)(r->contenido);

				Memoria *mem = buscarMemoria(contenido->numMem);

				if(mem==NULL)
					return REQUEST_ERROR;

				Criterio cons = toConsistencia(contenido->criterio);
				add(mem,cons);

				break;
			}
			default:
				break;
		}
	return REQUEST_OK;
	}
}

void finalizarScript()	// Debe hacer un free y sacarlo de la cola
{
	/*
	resultadoParser* res;
	res = queue_pop(exi);
	free(res);
	*/
	free(queue_pop(exi));
}

bool usaTabla(resultadoParser* r){
	return r->accionEjecutar == SELECT || r->accionEjecutar == INSERT || r->accionEjecutar == DROP || r->accionEjecutar == DESCRIBE || r->accionEjecutar == CREATE;
}
Tabla* obtenerTabla(resultadoParser* r){
	switch(r->accionEjecutar)
	{
		case SELECT:
		{
			contenidoSelect* c = (contenidoSelect*)r->contenido;
			return buscarTabla(c->nombreTabla);
		}
		case INSERT:
		{
			contenidoInsert* c = (contenidoInsert*)r->contenido;
			return buscarTabla(c->nombreTabla);
		}
		case DROP:
		{
			contenidoDrop* c = (contenidoDrop*)r->contenido;
			return buscarTabla(c->nombreTabla);
		}
		case CREATE:
		{
			contenidoCreate* c = (contenidoCreate*)r->contenido;
			return buscarTabla(c->nombreTabla);
		}
		case DESCRIBE:
		{
			contenidoDescribe* c = (contenidoDescribe*)r->contenido;
			return buscarTabla(c->nombreTabla);
		}
		default:
			return NULL;
	}
}

Tabla* buscarTabla(char* nom)
{	printf("Entre a buscarTabla\n");
	bool coincideNombre(void* element)					//Subfunción de busqueda
	{
		return strcmp(nom,((Tabla*)element)->nombre) == 0;
	}

	return (Tabla*)list_find(tablas,coincideNombre);
}
