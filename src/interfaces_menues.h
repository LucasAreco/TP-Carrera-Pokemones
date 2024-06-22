#ifndef INTERFACES_MENUES_H
#define INTERFACES_MENUES_H

#include "fase.h"
#include <stdbool.h>

void mostrar_interfaz_inicio(void* contexto);
void mostrar_interfaz_dificultad(void *contexto);
void mostrar_interfaz_info_oponente(void *contexto);
void mostrar_info_pokemon_usuario(void *contexto);
void mostrar_info_pokemon(const struct pokemon_info *pokemon);
void mostrar_info_armado_pista(void *contexto);
char *mostrar_pista_usuario(char *pista_usuario, estado_t *estado);
void mostrar_resultado_carrera(void *contexto);
void mostrar_interfaz_fin_del_juego(void *contexto);
bool mostrar_comando_y_descripcion(void *contexto);
bool mostrar_acerca_del_juego(void *contexto);
void liberar_memoria_split(char **nombres, int cantidad);

#endif // INTERFACES_MENUES_H
