#ifndef INTERFACES_MENUES_H
#define INTERFACES_MENUES_H

#include "fase.h"
#include <stdbool.h>

/**
 * Muestra la interfaz de inicio del juego, incluyendo el logo y las opciones iniciales.
 * En caso de error, no realiza ninguna acción visible.
 */
void mostrar_interfaz_inicio(void *contexto);

/**
 * Muestra la interfaz de selección de dificultad del juego.
 * En caso de error, no realiza ninguna acción visible.
 */
void mostrar_interfaz_dificultad(void *contexto);

/**
 * Muestra la información del oponente, incluyendo su pokémon y la pista de obstáculos.
 * En caso de error al asignar el pokémon o construir la pista, informa al usuario.
 */
void mostrar_interfaz_info_oponente(void *contexto);

/**
 * Muestra la información para que el usuario elija su pokémon.
 * En caso de error al obtener los nombres de los pokémon disponibles, informa al usuario.
 */
void mostrar_info_pokemon_usuario(void *contexto);

/**
 * Muestra la información detallada de un Pokémon.
 * En caso de error al obtener la información del Pokémon, no muestra nada.
 */
void mostrar_info_pokemon(const struct pokemon_info *pokemon);

/**
 * Muestra la interfaz para el armado de la pista de obstáculos por parte del usuario.
 * En caso de error al obtener la pista actual, no muestra ninguna pista.
 */
void mostrar_info_armado_pista(void *contexto);

/**
 * Muestra la pista de obstáculos actual del usuario.
 * En caso de error al obtener la pista actual, retorna NULL.
 */
char *mostrar_pista_usuario(char *pista_usuario, estado_t *estado);

/**
 * Muestra el resultado de la carrera entre los pokémon.
 * Calcula el puntaje final y muestra si los pokémon han sincronizado sus tiempos.
 * En caso de error al calcular los tiempos o al mostrar el resultado, no muestra ningún resultado.
 */
void mostrar_resultado_carrera(void *contexto);

/**
 * Muestra la interfaz de fin del juego, mostrando el resultado final y finalizando la fase actual.
 * En caso de error, no realiza ninguna acción visible.
 */
void mostrar_interfaz_fin_del_juego(void *contexto);

/**
 * Función auxiliar que imprime el comando y su descripción para cada comando del menú.
 * Utilizada para mostrar todos los comandos disponibles en la interfaz.
 * Siempre retorna true.
 */
bool imprimir_comando_descripcion(const char *clave, const char *descripcion,
				  bool (*funcion)(void *), void *aux);

/**
 * Muestra todos los comandos disponibles en la interfaz actual.
 * Utiliza la función auxiliar imprimir_comando_descripcion para imprimir cada comando.
 * Siempre retorna true.
 */
bool mostrar_comando_y_descripcion(void *contexto);

/**
 * Muestra información acerca del juego, describiendo el objetivo y la mecánica principal.
 * En caso de error, no muestra ninguna información adicional.
 * Siempre retorna true.
 */
bool mostrar_acerca_del_juego(void *contexto);

#endif // INTERFACES_MENUES_H
