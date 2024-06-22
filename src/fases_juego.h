#ifndef FASES_JUEGO_H
#define FASES_JUEGO_H

#include "fase.h"
#include "comandos_juego.h"
#include "fases_juego.h"
#include <stdbool.h>

fase_t* inicio_juego();
fase_t *seleccionar_dificultad();
fase_t *armado_de_atributos_oponente();
fase_t *inicializar_pokemon_usuario(estado_t *estado);
fase_t *armar_pista_del_usuario();
fase_t *correr_carrera();
fase_t *fin_del_juego();

#endif // FASES_JUEGO_H
