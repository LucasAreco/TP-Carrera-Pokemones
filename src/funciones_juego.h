#ifndef FUNCIONES_JUEGO_H
#define FUNCIONES_JUEGO_H

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

/**
 * Asigna al estado la dificultad elegida por el usuario.
 * Si la dificultad es válida, devuelve true y avanza al siguiente estado del juego.
 * Si la dificultad no es válida, devuelve false.
 */
bool inicializar_dificultad(void *e);

/**
 * Asigna al usuario el pokémon elegido de la lista de pokémon disponibles.
 * Si la elección es válida, devuelve true.
 * Si la elección es inválida (por ejemplo, pokémon no disponible), devuelve false.
 */
bool seleccion_pokemon_usuario(void *e);

/**
 * Cambia al modo de modificar la pista para que el usuario agregue o quite elementos en su pista.
 * Si los comandos de modificación son válidos, devuelve true; en caso contrario, devuelve false.
 */
bool modificar_pista(void *e);

/**
 * Regresa al modo de modificar la pista del usuario, siempre que tenga intentos suficientes para hacerlo.
 * Si tiene intentos suficientes, regresa a la fase de armado de pista.
 * Si no tiene intentos suficientes, termina el juego.
 */
bool volver_a_modificar_pista(void *e);

/**
 * Avanza a la fase de selección de dificultad del juego.
 */
bool pasar_a_seleccionar_dificultad(void *e);

/**
 * Avanza a la fase de selección de Pokémon por parte del usuario.
 */
bool pasar_a_seleccionar_pokemon(void *e);

/**
 * Avanza a la fase de armado de la pista por parte del usuario.
 */
bool pasar_a_armar_pista(void *e);

/**
 * Avanza a la fase de corrida de la carrera entre los Pokémon de ambos jugadores.
 */
bool pasar_a_correr_carrera(void *e);

/**
 * Finaliza el juego, pasando a la fase de fin de juego.
 */
bool pasar_a_fin_juego(void *e);

/**
 * Asigna un Pokémon al jugador basado en la elección realizada.
 * Esta función es utilizada internamente para asignar el Pokémon al jugador después de la selección.
 */
const struct pokemon_info *asignar_pokemon_jugador(TP *tp,
						   enum TP_JUGADOR jugador);

/**
 * Construye la pista del oponente del jugador según la dificultad elegida.
 * Devuelve la cantidad de obstáculos construidos para la pista del oponente.
 */
unsigned int construir_pista_oponente(TP *tp, enum TP_JUGADOR jugador,
				      int dificultad);

/**
 * Termina el juego y libera los recursos utilizados.
 */
bool salir(void *e);

/**
 * Libera la memoria asignada para un array de strings (usado por ejemplo en el manejo de nombres de Pokémon).
 * Esta función es útil para liberar la memoria después de haber usado funciones de split.
 */
void liberar_memoria_split(char **nombres);

#endif // FUNCIONES_JUEGO_H
