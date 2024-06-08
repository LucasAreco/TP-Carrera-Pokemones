#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "hash.h"

#define FACTOR_CARGA_MAXIMA 0.7
#define CAPACIDAD_MIN 3

typedef struct par {
	char *clave;
	void *valor;
} par_t;

struct hash {
	size_t capacidad;
	par_t *pares;
	size_t cantidad_pares;
};

/*
 * Calcula un hash para una cadena de caracteres utilizando
 * el algoritmo DJB2. Toma un puntero a los datos y devuelve un hash 
 * único para esos datos.
 */
unsigned long funcion_hash_djb2(void *data)
{
	const unsigned char *str = data;
	unsigned long hash = 5381;
	int c;

	for (int i = 0; (c = str[i]) != '\0'; i++) {
		hash = hash * 33 + (unsigned long)c;
	}

	return hash;
}

/*
* Calcula y devuelve el número hash para una clave.
*
* Utiliza una función hash dada para calcular el hash de la clave,
* ajustándolo al rango de capacidad del hash proporcionado.
*/
size_t numero_hash(hash_t *hash, size_t (*funcion_hash)(void *data),
		   const char *clave)
{
	return (size_t)funcion_hash((void *)clave) % hash->capacidad;
}

/*
 * Busca una clave en la tabla hash y devuelve un puntero al par clave-valor si se encuentra.
 * En caso contrario, devuelve NULL.
 */
par_t *obtener_par(hash_t *hash, const char *clave)
{
	size_t posicion = numero_hash(hash, funcion_hash_djb2, clave);
	int posiciones_visitadas = 0;

	while (hash->pares[posicion].clave != NULL &&
	       posiciones_visitadas < hash->capacidad) {
		if (strcmp(hash->pares[posicion].clave, clave) == 0) {
			return &(hash->pares[posicion]);
		}
		posicion = (posicion + 1) % hash->capacidad;
		posiciones_visitadas++;
	}

	return NULL;
}

/*
 * Copia la cadena de caracteres en una nueva ubicación de memoria 
 * y devuelve un puntero a la copia. El usuario es responsable de liberar la memoria
 * asignada utilizando la función `free`.
 */
char *copiar_clave(const char *clave)
{
	size_t largo = strlen(clave) + 1;
	char *copia = malloc(largo);
	if (!copia) {
		return NULL;
	}

	for (size_t i = 0; i < largo; i++) {
		copia[i] = clave[i];
	}

	return copia;
}

hash_t *hash_crear(size_t capacidad)
{
	hash_t *hash = calloc(1, sizeof(hash_t));
	if (!hash) {
		return NULL;
	}

	if (capacidad < CAPACIDAD_MIN) {
		hash->capacidad = CAPACIDAD_MIN;
	}

	hash->capacidad = capacidad;

	hash->pares = calloc(hash->capacidad, sizeof(par_t));
	if (!hash->pares) {
		free(hash);
		return NULL;
	}

	return hash;
}

/*
 * Realiza un rehashing del hash proporcionado, duplicando su capacidad.
 * Si es exitoso, devuelve un puntero al hash sino devuelve NULL.
 */
hash_t *rehash(hash_t *hash)
{
	if (!hash) {
		return NULL;
	}

	size_t capacidad_anterior = hash->capacidad;
	size_t nueva_capacidad = capacidad_anterior * 2;

	hash_t *hash_aux = hash_crear(nueva_capacidad);
	if (!hash_aux) {
		return NULL;
	}

	for (size_t i = 0; i < capacidad_anterior; i++) {
		if (hash->pares[i].clave != NULL) {
			if (!hash_insertar(hash_aux, hash->pares[i].clave,
					   hash->pares[i].valor, NULL)) {
				hash_destruir(hash_aux);
				return NULL;
			}
			free((void *)hash->pares[i].clave);
		}
	}

	free(hash->pares);

	hash->capacidad = nueva_capacidad;
	hash->pares = hash_aux->pares;
	hash->cantidad_pares = hash_aux->cantidad_pares;

	free(hash_aux);

	return hash;
}

hash_t *hash_insertar(hash_t *hash, const char *clave, void *elemento,
		      void **anterior)
{
	if (!hash || !clave) {
		return NULL;
	}

	float factor_de_carga =
		(float)hash->cantidad_pares / (float)hash->capacidad;
	if (factor_de_carga >= FACTOR_CARGA_MAXIMA) {
		if (!rehash(hash)) {
			return NULL;
		}
	}

	par_t *par = obtener_par(hash, clave);
	if (par) {
		if (anterior) {
			*anterior = par->valor;
		}
		par->valor = elemento;
		return hash;
	}

	size_t posicion = numero_hash(hash, funcion_hash_djb2, clave);

	while (hash->pares[posicion].clave != NULL) {
		posicion = (posicion + 1) % hash->capacidad;
	}

	hash->pares[posicion].clave = copiar_clave(clave);
	if (!hash->pares[posicion].clave) {
		return NULL;
	}
	hash->pares[posicion].valor = elemento;

	hash->cantidad_pares++;

	if (anterior) {
		*anterior = NULL;
	}

	return hash;
}

/*
* Reemplaza pares clave-valor al quitar un elemento del hash.
* Ajusta la posición de los pares colisionantes para mantener la integridad del hash.
*/
void reemplazar_al_quitar(hash_t *hash, size_t posicion)
{
	size_t actual = (posicion + 1) % hash->capacidad;
	int posiciones_visitadas = 0;

	while (hash->pares[actual].clave != NULL &&
	       posiciones_visitadas < hash->capacidad) {
		size_t nueva_posicion = numero_hash(hash, funcion_hash_djb2,
						    hash->pares[actual].clave);
		if (nueva_posicion <= posicion) {
			hash->pares[posicion].clave =
				copiar_clave(hash->pares[actual].clave);
			if (!hash->pares[posicion].clave) {
				return;
			}
			hash->pares[posicion].valor = hash->pares[actual].valor;

			free((void *)hash->pares[actual].clave);
			hash->pares[actual].clave = NULL;
			hash->pares[actual].valor = NULL;

			posicion = actual;
		}

		actual = (actual + 1) % hash->capacidad;
		posiciones_visitadas++;
	}
}

void *hash_quitar(hash_t *hash, const char *clave)
{
	if (!hash || !clave) {
		return NULL;
	}

	size_t posicion_quitar = numero_hash(hash, funcion_hash_djb2, clave);

	while (hash->pares[posicion_quitar].clave != NULL) {
		if (strcmp(hash->pares[posicion_quitar].clave, clave) == 0) {
			void *valor_eliminado =
				hash->pares[posicion_quitar].valor;

			char *clave_eliminada =
				hash->pares[posicion_quitar].clave;

			hash->pares[posicion_quitar].clave = NULL;
			hash->pares[posicion_quitar].valor = NULL;

			reemplazar_al_quitar(hash, posicion_quitar);

			hash->cantidad_pares--;

			free(clave_eliminada);

			return valor_eliminado;
		}
		posicion_quitar = (posicion_quitar + 1) % hash->capacidad;
	}

	return NULL;
}

void *hash_obtener(hash_t *hash, const char *clave)
{
	if (!hash || !clave) {
		return NULL;
	}

	size_t posicion = numero_hash(hash, funcion_hash_djb2, clave);
	int posiciones_visitadas = 0;

	while (hash->pares[posicion].clave != NULL &&
	       posiciones_visitadas < hash->capacidad) {
		if (strcmp(hash->pares[posicion].clave, clave) == 0) {
			return hash->pares[posicion].valor;
		}
		posicion = (posicion + 1) % hash->capacidad;
		posiciones_visitadas++;
	}

	return NULL;
}

bool hash_contiene(hash_t *hash, const char *clave)
{
	if (!hash || !clave) {
		return false;
	}

	if (hash_obtener(hash, clave)) {
		return true;
	}

	return false;
}

size_t hash_cantidad(hash_t *hash)
{
	if (!hash) {
		return 0;
	}
	return hash->cantidad_pares;
}

void hash_destruir_todo(hash_t *hash, void (*destructor)(void *))
{
	if (!hash) {
		return;
	}
	for (size_t i = 0; i < hash->capacidad; i++) {
		if (hash->pares[i].clave != NULL) {
			free(hash->pares[i].clave);
			if (destructor) {
				destructor(hash->pares[i].valor);
			}
		}
	}
	free(hash->pares);
	free(hash);
}

void hash_destruir(hash_t *hash)
{
	hash_destruir_todo(hash, NULL);
}

size_t hash_con_cada_clave(hash_t *hash,
			   bool (*f)(const char *clave, void *valor, void *aux),
			   void *aux)
{
	if (!hash || !f) {
		return 0;
	}

	size_t iteraciones = 0;

	for (size_t i = 0; i < hash->capacidad; i++) {
		if (hash->pares[i].clave) {
			bool funcion = f(hash->pares[i].clave,
					 hash->pares[i].valor, aux);
			iteraciones++;

			if (!funcion) {
				return iteraciones;
			}
		}
	}

	return iteraciones;
}