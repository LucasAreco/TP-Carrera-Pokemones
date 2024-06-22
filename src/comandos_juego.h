#ifndef COMANDOS_JUEGO_H
#define COMANDOS_JUEGO_H

#include "fase.h"
#include <stdbool.h>
#include "tp.h"

typedef struct estado_juego {
	TP *juego;
	bool continuar;
	int dificultad;
	fase_t *fase_actual;
	char *entrada;
	int intentos;
} estado_t;

typedef enum { FACIL, NORMAL, DIFICIL, IMPOSIBLE } DIFICULTAD;

bool inicializar_dificultad(void *e);
bool seleccion_pokemon_usuario(void *e);
bool modificar_pista(void *e);
bool volver_a_modificar_pista(void *e);

bool salir(void *e);

bool pasar_a_seleccionar_pokemon(void *e);
bool pasar_a_armar_pista(void *e);
bool pasar_a_correr_carrera(void *e);
bool pasar_a_fin_juego(void *e);

const struct pokemon_info *asignar_pokemon_jugador(TP *tp,
						   enum TP_JUGADOR jugador);
unsigned int construir_pista_oponente(TP *tp, enum TP_JUGADOR jugador,
				      int dificultad);

void liberar_memoria_split(char **nombres, int cantidad);
#endif // COMANDOS_JUEGO_H
