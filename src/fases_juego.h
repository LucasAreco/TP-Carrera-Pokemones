#ifndef FASES_JUEGO_H
#define FASES_JUEGO_H

#include "fase.h"
#include "funciones_juego.h"
#include "fases_juego.h"
#include <stdbool.h>

/**
 * Crea y devuelve la fase inicial del juego.
 * En caso de error devuelve NULL.
 */
fase_t* inicio_juego();

/**
 * Crea y devuelve la fase de selección de dificultad del juego.
 * En caso de error devuelve NULL.
 */
fase_t *seleccionar_dificultad();

/**
 * Crea y devuelve la fase de armado de atributos para el oponente.
 * En caso de error devuelve NULL.
 */
fase_t *armado_de_atributos_oponente();

/**
 * Inicializa el pokémon del usuario en el estado proporcionado.
 * En caso de error devuelve NULL.
 */
fase_t *inicializar_pokemon_usuario(estado_t *estado);

/**
 * Crea y devuelve la fase de armado de la pista del usuario.
 * En caso de error devuelve NULL.
 */
fase_t *armar_pista_del_usuario();

/**
 * Crea y devuelve la fase de carrera entre los Pokémon.
 * En caso de error devuelve NULL.
 */
fase_t *correr_carrera();

/**
 * Crea y devuelve la fase de fin de juego.
 * En caso de error devuelve NULL.
 */
fase_t *fin_del_juego();


#endif // FASES_JUEGO_H
