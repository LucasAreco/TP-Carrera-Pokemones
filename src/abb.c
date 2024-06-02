#include "abb.h"
#include "abb_estructura_privada.h"
#include <stddef.h>
#include <stdlib.h>

typedef struct s_vector {
	void **array;
	size_t tamanio_max;
	size_t cantidad;
} vector_t;

abb_t *abb_crear(abb_comparador comparador)
{
	if (!comparador) {
		return NULL;
	}

	abb_t *nuevo_abb = calloc(1, sizeof(abb_t));
	if (!nuevo_abb) {
		return NULL;
	}

	nuevo_abb->comparador = comparador;

	return nuevo_abb;
}

nodo_abb_t *nodo_crear(void *elemento)
{
	nodo_abb_t *nuevo_nodo_abb = calloc(1, sizeof(nodo_abb_t));
	if (!nuevo_nodo_abb) {
		return NULL;
	}

	nuevo_nodo_abb->elemento = elemento;

	return nuevo_nodo_abb;
}

nodo_abb_t *abb_insertar_aux(nodo_abb_t *raiz_actual, void *elemento,
			     abb_comparador comparador)
{
	if (!raiz_actual) {
		return nodo_crear(elemento);
	}

	int comparacion = comparador(elemento, raiz_actual->elemento);
	if (comparacion <= 0) {
		raiz_actual->izquierda = abb_insertar_aux(
			raiz_actual->izquierda, elemento, comparador);
	} else {
		raiz_actual->derecha = abb_insertar_aux(raiz_actual->derecha,
							elemento, comparador);
	}

	return raiz_actual;
}

abb_t *abb_insertar(abb_t *arbol, void *elemento)
{
	if (!arbol) {
		return NULL;
	}

	arbol->nodo_raiz =
		abb_insertar_aux(arbol->nodo_raiz, elemento, arbol->comparador);
	(arbol->tamanio)++;

	return arbol;
}

nodo_abb_t *obtener_predecesor_inorder(nodo_abb_t *raiz_actual,
				       void **elemento_quitado)
{
	if (!raiz_actual->derecha) {
		*elemento_quitado = raiz_actual->elemento;
		nodo_abb_t *hijo_izquierdo = raiz_actual->izquierda;
		free(raiz_actual);
		return hijo_izquierdo;
	}

	raiz_actual->derecha = obtener_predecesor_inorder(raiz_actual->derecha,
							  elemento_quitado);

	return raiz_actual;
}

void *abb_quitar_aux(nodo_abb_t *raiz_actual, void *elemento,
		     abb_comparador comparador, void **elemento_quitado)
{
	if (!raiz_actual || !comparador) {
		return NULL;
	}

	int comparacion = comparador(elemento, raiz_actual->elemento);

	if (comparacion == 0) {
		*elemento_quitado = raiz_actual->elemento;
		nodo_abb_t *izquierda = raiz_actual->izquierda;
		nodo_abb_t *derecha = raiz_actual->derecha;

		if (raiz_actual->izquierda && raiz_actual->derecha) {
			void *elemento_predecesor_inorder = NULL;

			raiz_actual->izquierda = obtener_predecesor_inorder(
				raiz_actual->izquierda,
				&elemento_predecesor_inorder);

			raiz_actual->elemento = elemento_predecesor_inorder;

			return raiz_actual;

		} else {
			free(raiz_actual);

			if (!izquierda) {
				return derecha;
			}

			return izquierda;
		}
	} else if (comparacion < 0) {
		raiz_actual->izquierda = abb_quitar_aux(raiz_actual->izquierda,
							elemento, comparador,
							elemento_quitado);
	}

	raiz_actual->derecha = abb_quitar_aux(raiz_actual->derecha, elemento,
					      comparador, elemento_quitado);

	return raiz_actual;
}

void *abb_quitar(abb_t *arbol, void *elemento)
{
	if (!arbol) {
		return NULL;
	}

	void *elemento_quitado = NULL;

	arbol->nodo_raiz = abb_quitar_aux(arbol->nodo_raiz, elemento,
					  arbol->comparador, &elemento_quitado);
	if (elemento_quitado) {
		(arbol->tamanio)--;
	}

	return elemento_quitado;
}

nodo_abb_t *abb_buscar_aux(nodo_abb_t *raiz_actual, void *elemento_buscado,
			   abb_comparador comparador)
{
	if (!raiz_actual) {
		return NULL;
	}

	int comparacion = comparador(elemento_buscado, raiz_actual->elemento);
	if (comparacion == 0) {
		return raiz_actual;
	} else if (comparacion < 0) {
		return abb_buscar_aux(raiz_actual->izquierda, elemento_buscado,
				      comparador);
	} else {
		return abb_buscar_aux(raiz_actual->derecha, elemento_buscado,
				      comparador);
	}

	return raiz_actual;
}

void *abb_buscar(abb_t *arbol, void *elemento)
{
	if (!arbol) {
		return NULL;
	}

	nodo_abb_t *nodo_encontrado =
		abb_buscar_aux(arbol->nodo_raiz, elemento, arbol->comparador);
	if (nodo_encontrado) {
		return nodo_encontrado->elemento;
	}

	return NULL;
}

bool abb_vacio(abb_t *arbol)
{
	return (!arbol || !arbol->nodo_raiz || (abb_tamanio(arbol) == 0));
}

size_t abb_tamanio(abb_t *arbol)
{
	if (!arbol) {
		return 0;
	}

	return arbol->tamanio;
}

void abb_destruir_todo_aux(nodo_abb_t *raiz_actual, void (*destructor)(void *))
{
	if (!raiz_actual) {
		return;
	}

	abb_destruir_todo_aux(raiz_actual->izquierda, destructor);
	abb_destruir_todo_aux(raiz_actual->derecha, destructor);
	if (destructor) {
		destructor(raiz_actual->elemento);
	}

	free(raiz_actual);
}

void abb_destruir_todo(abb_t *arbol, void (*destructor)(void *))
{
	if (!arbol) {
		return;
	}

	abb_destruir_todo_aux(arbol->nodo_raiz, destructor);

	free(arbol);
}

void abb_destruir(abb_t *arbol)
{
	abb_destruir_todo(arbol, NULL);
}

bool abb_recorrer_preorder(nodo_abb_t *raiz_actual,
			   bool (*funcion)(void *, void *), void *aux,
			   size_t *veces_recorrido)
{
	if (!raiz_actual) {
		return true;
	}

	(*veces_recorrido)++;

	if (!funcion(raiz_actual->elemento, aux)) {
		return false;
	}

	bool continua_recorrido = abb_recorrer_preorder(
		raiz_actual->izquierda, funcion, aux, veces_recorrido);

	if (!continua_recorrido) {
		return false;
	}

	return abb_recorrer_preorder(raiz_actual->derecha, funcion, aux,
				     veces_recorrido);
}

bool abb_recorrer_inorder(nodo_abb_t *raiz_actual,
			  bool (*funcion)(void *, void *), void *aux,
			  size_t *veces_recorrido)
{
	if (!raiz_actual) {
		return true;
	}

	bool continua_recorrido = abb_recorrer_inorder(
		raiz_actual->izquierda, funcion, aux, veces_recorrido);

	if (!continua_recorrido) {
		return false;
	}

	(*veces_recorrido)++;

	if (!funcion(raiz_actual->elemento, aux)) {
		return false;
	}
	return abb_recorrer_inorder(raiz_actual->derecha, funcion, aux,
				    veces_recorrido);
}

bool abb_recorrer_postorder(nodo_abb_t *raiz_actual,
			    bool (*funcion)(void *, void *), void *aux,
			    size_t *veces_recorrido)
{
	if (!raiz_actual) {
		return true;
	}

	bool continua_recorrido = abb_recorrer_postorder(
		raiz_actual->izquierda, funcion, aux, veces_recorrido);

	if (!continua_recorrido) {
		return false;
	}

	continua_recorrido = abb_recorrer_postorder(
		raiz_actual->derecha, funcion, aux, veces_recorrido);

	if (!continua_recorrido) {
		return false;
	}

	(*veces_recorrido)++;

	if (!funcion(raiz_actual->elemento, aux)) {
		return false;
	}
	return continua_recorrido;
}

size_t abb_con_cada_elemento(abb_t *arbol, abb_recorrido recorrido,
			     bool (*funcion)(void *, void *), void *aux)
{
	if (!arbol || !funcion) {
		return 0;
	}

	size_t veces = 0;

	if (recorrido == INORDEN) {
		abb_recorrer_inorder(arbol->nodo_raiz, funcion, aux, &veces);
	} else if (recorrido == PREORDEN) {
		abb_recorrer_preorder(arbol->nodo_raiz, funcion, aux, &veces);
	} else if (recorrido == POSTORDEN) {
		abb_recorrer_postorder(arbol->nodo_raiz, funcion, aux, &veces);
	}

	return veces;
}

bool completar_vector(void *elemento, void *vector)
{
	vector_t *vector_actual = vector;
	if (vector_actual->cantidad == vector_actual->tamanio_max) {
		return false;
	}

	vector_actual->array[vector_actual->cantidad] = elemento;
	(vector_actual->cantidad)++;

	return true;
}

size_t abb_recorrer(abb_t *arbol, abb_recorrido recorrido, void **array,
		    size_t tamanio_array)
{
	if (!arbol || !array || tamanio_array == 0) {
		return 0;
	}

	vector_t nuevo_vector;
	nuevo_vector.array = array;
	nuevo_vector.tamanio_max = tamanio_array;
	nuevo_vector.cantidad = 0;

	abb_con_cada_elemento(arbol, recorrido, completar_vector,
			      &nuevo_vector);

	return nuevo_vector.cantidad;
}
