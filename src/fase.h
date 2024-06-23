#ifndef FASE_H
#define FASE_H

#include "lista.h"
#include "menu.h"

typedef struct fase {
	menu_t *menu;
	char *entrada;
	void (*contenido)(void *);
} fase_t;


/**
 * Crea una nueva fase.
 * Devuelve un puntero a la fase creada o NULL en caso de error.
 */
fase_t *inicializar_fase();

/**
 * Libera toda la memoria reservada por la fase.
 */
void destruir_fase(fase_t *fase);
#endif // FASE_H
