#include "lista.h"
#include <stddef.h>
#include <stdlib.h>

typedef struct nodo {
	void *elemento;
	struct nodo *siguiente;
} nodo_t;

struct lista {
	nodo_t *nodo_inicial;
	nodo_t *nodo_final;
	size_t tamanio;
};

struct lista_iterador {
	nodo_t *corriente;
	lista_t *lista;
};

lista_t *lista_crear()
{
	return calloc(1, sizeof(lista_t));
}

lista_t *lista_insertar(lista_t *lista, void *elemento)
{
	if (!lista) {
		return NULL;
	}

	nodo_t *nuevo_nodo = malloc(sizeof(nodo_t));
	if (!nuevo_nodo) {
		return NULL;
	}

	if (lista_vacia(lista)) {
		lista->nodo_inicial = nuevo_nodo;
		lista->nodo_final = nuevo_nodo;
	} else {
		lista->nodo_final->siguiente = nuevo_nodo;
		lista->nodo_final = nuevo_nodo;
	}

	nuevo_nodo->elemento = elemento;
	nuevo_nodo->siguiente = NULL;
	(lista->tamanio)++;

	return lista;
}

lista_t *lista_insertar_en_posicion(lista_t *lista, void *elemento,
				    size_t posicion)
{
	if (!lista) {
		return NULL;
	}

	if (posicion >= lista_tamanio(lista)) {
		return lista_insertar(lista, elemento);
	}

	nodo_t *nuevo_nodo = malloc(sizeof(nodo_t));
	if (!nuevo_nodo) {
		return NULL;
	}
	nuevo_nodo->elemento = elemento;
	nuevo_nodo->siguiente = NULL;

	if (posicion == 0) {
		nuevo_nodo->siguiente = lista->nodo_inicial;
		lista->nodo_inicial = nuevo_nodo;
	} else {
		nodo_t *nodo_anterior = lista->nodo_inicial;
		for (size_t i = 1; i < posicion; i++) {
			nodo_anterior = nodo_anterior->siguiente;
		}
		nuevo_nodo->siguiente = nodo_anterior->siguiente;
		nodo_anterior->siguiente = nuevo_nodo;
	}

	(lista->tamanio)++;
	return lista;
}

void *lista_quitar(lista_t *lista)
{
	if (!lista || lista_vacia(lista))
		return NULL;

	void *elemento_quitado = NULL;

	if (lista->nodo_inicial->siguiente == NULL) {
		elemento_quitado = lista->nodo_inicial->elemento;
		free(lista->nodo_inicial);
		lista->nodo_inicial = lista->nodo_final = NULL;
	} else {
		nodo_t *nodo_actual = lista->nodo_inicial;

		while (nodo_actual->siguiente->siguiente != NULL) {
			nodo_actual = nodo_actual->siguiente;
		}

		elemento_quitado = nodo_actual->siguiente->elemento;
		free(nodo_actual->siguiente);
		nodo_actual->siguiente = NULL;
		lista->nodo_final = nodo_actual;
	}

	(lista->tamanio)--;

	return elemento_quitado;
}

void *lista_quitar_de_posicion(lista_t *lista, size_t posicion)
{
	if (!lista || lista_vacia(lista)) {
		return NULL;
	}

	if (posicion >= lista->tamanio) {
		return lista_quitar(lista);
	}

	void *elemento_quitado = NULL;
	nodo_t *nodo_anterior = NULL;
	nodo_t *nodo_actual = lista->nodo_inicial;

	if (posicion == 0) {
		lista->nodo_inicial = nodo_actual->siguiente;
	} else {
		size_t posicion_actual = 0;
		while (posicion_actual < posicion) {
			nodo_anterior = nodo_actual;
			nodo_actual = nodo_actual->siguiente;
			posicion_actual++;
		}
		nodo_anterior->siguiente = nodo_actual->siguiente;
	}

	elemento_quitado = nodo_actual->elemento;
	free(nodo_actual);
	lista->tamanio--;

	return elemento_quitado;
}

void *lista_elemento_en_posicion(lista_t *lista, size_t posicion)
{
	if (!lista || posicion > lista_tamanio(lista)) {
		return NULL;
	}

	nodo_t *nodo_actual = lista->nodo_inicial;
	size_t posicion_actual = 0;

	while (posicion_actual < posicion) {
		nodo_actual = nodo_actual->siguiente;
		posicion_actual++;
	}

	if (!nodo_actual) {
		return NULL;
	}

	return nodo_actual->elemento;
}

void *lista_buscar_elemento(lista_t *lista, int (*comparador)(void *, void *),
			    void *contexto)
{
	if (!lista || !comparador || lista_vacia(lista)) {
		return NULL;
	}

	nodo_t *nodo_actual = lista->nodo_inicial;
	int posicion_actual = 0;

	while (nodo_actual != NULL && posicion_actual <= lista_tamanio(lista)) {
		if ((*comparador)(nodo_actual->elemento, contexto) == 0) {
			return nodo_actual->elemento;
		}
		nodo_actual = nodo_actual->siguiente;
		posicion_actual++;
	}

	return NULL;
}

void *lista_primero(lista_t *lista)
{
	if (!lista || lista_vacia(lista)) {
		return NULL;
	}

	return lista->nodo_inicial->elemento;
}

void *lista_ultimo(lista_t *lista)
{
	if (!lista || lista_vacia(lista)) {
		return NULL;
	}

	return lista->nodo_final->elemento;
}

bool lista_vacia(lista_t *lista)
{
	if (!lista || lista->tamanio == 0) {
		return true;
	}

	return false;
}

size_t lista_tamanio(lista_t *lista)
{
	if (!lista) {
		return 0;
	}

	return lista->tamanio;
}

void lista_destruir_todo(lista_t *lista, void (*funcion)(void *))
{
	if (!lista) {
		return;
	}

	if (!lista_vacia(lista)) {
		nodo_t *nodo_actual = lista->nodo_inicial;
		nodo_t *nodo_aux = NULL;
		size_t contador = 0;

		while (contador < lista_tamanio(lista)) {
			nodo_aux = nodo_actual->siguiente;
			if (funcion) {
				funcion(nodo_actual->elemento);
			}
			free(nodo_actual);
			nodo_actual = nodo_aux;
			contador++;
		}
	}

	free(lista);
}

void lista_destruir(lista_t *lista)
{
	lista_destruir_todo(lista, NULL);
}

lista_iterador_t *lista_iterador_crear(lista_t *lista)
{
	if (!lista) {
		return NULL;
	}

	lista_iterador_t *nuevo_iterador = malloc(sizeof(lista_iterador_t));
	if (!nuevo_iterador) {
		return NULL;
	}
	nuevo_iterador->corriente = lista->nodo_inicial;
	nuevo_iterador->lista = lista;

	return nuevo_iterador;
}

bool lista_iterador_tiene_siguiente(lista_iterador_t *iterador)
{
	if (!iterador) {
		return false;
	}
	return iterador->corriente != NULL;
}

bool lista_iterador_avanzar(lista_iterador_t *iterador)
{
	if (!lista_iterador_tiene_siguiente(iterador)) {
		return false;
	}

	iterador->corriente = iterador->corriente->siguiente;

	return iterador->corriente != NULL;
}

void *lista_iterador_elemento_actual(lista_iterador_t *iterador)
{
	if (!iterador || !iterador->corriente) {
		return NULL;
	}
	return iterador->corriente->elemento;
}

void lista_iterador_destruir(lista_iterador_t *iterador)
{
	free(iterador);
}

size_t lista_con_cada_elemento(lista_t *lista, bool (*funcion)(void *, void *),
			       void *contexto)
{
	if (!lista || !funcion || lista_vacia(lista)) {
		return 0;
	}

	size_t cantidad_lista = lista_tamanio(lista);
	nodo_t *nodo_actual = lista->nodo_inicial;
	size_t posicion_actual = 0;
	bool continuar = true;

	for (size_t i = 0; i < cantidad_lista && continuar; i++) {
		continuar = funcion(nodo_actual->elemento, contexto);
		nodo_actual = nodo_actual->siguiente;
		posicion_actual++;
	}

	return posicion_actual;
}
