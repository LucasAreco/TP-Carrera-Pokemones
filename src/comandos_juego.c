#include "comandos_juego.h"
#include "split.h"
#include "interfaces_menues.h"
#include "fases_juego.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

int seleccionar_numero_aleatorio(int max)
{
	return rand() % max;
}

unsigned int construir_pista_oponente(TP *tp, enum TP_JUGADOR jugador,
				      int dificultad)
{
	int cantidad_obstaculos = (dificultad * 2 + 1) + 3;

	enum TP_OBSTACULO obstaculos[] = { OBSTACULO_FUERZA, OBSTACULO_DESTREZA,
					   OBSTACULO_INTELIGENCIA };

	int tipo_obtaculo = -1;

	for (unsigned int i = 0; i < cantidad_obstaculos; i++) {
		tipo_obtaculo = seleccionar_numero_aleatorio(3);
		tp_agregar_obstaculo(tp, jugador, obstaculos[tipo_obtaculo], i);
	}

	return (unsigned int)cantidad_obstaculos;
}

const struct pokemon_info *obtener_pokemon_aleatorio(TP *tp)
{
	char *pokemones_disponibles = tp_nombres_disponibles(tp);
	if (!pokemones_disponibles) {
		return NULL;
	}

	char separador = ',';
	char **nombres_disponibles = split(pokemones_disponibles, separador);

	int cantidad_pokemones = tp_cantidad_pokemon(tp);

	srand((unsigned int)time(NULL));

	int indice_random = seleccionar_numero_aleatorio(cantidad_pokemones);
	char *nombre_random = nombres_disponibles[indice_random];

	const struct pokemon_info *pokemon_oponente =
		tp_buscar_pokemon(tp, nombre_random);

	liberar_memoria_split(nombres_disponibles, cantidad_pokemones);
	free(pokemones_disponibles);

	return pokemon_oponente;
}

const struct pokemon_info *asignar_pokemon_jugador(TP *tp,
						   enum TP_JUGADOR jugador)
{
	const struct pokemon_info *pokemon_oponente =
		obtener_pokemon_aleatorio(tp);
	if (!pokemon_oponente) {
		return NULL;
	}

	if (tp_seleccionar_pokemon(tp, jugador, pokemon_oponente->nombre)) {
		return pokemon_oponente;
	}

	return NULL;
}

enum TP_OBSTACULO conversor_obstaculo_numero(char letra_obstaculo)
{
	switch (letra_obstaculo) {
	case 'F':
		return OBSTACULO_FUERZA;
		break;
	case 'D':
		return OBSTACULO_DESTREZA;
		break;
	case 'I':
		return OBSTACULO_INTELIGENCIA;
		break;

	default:
		break;
	}

	return -1;
}

bool volver(void *e)
{
	estado_t *estado = (estado_t *)e;
	estado->fase_actual->contenido(estado);
	return true;
}

bool volver_a_menu_armado_pista(char *linea, estado_t *estado,
				char *pista_usuario)
{
	if (strcmp(linea, "b") == 0) {
		printf("Volviendo al menú de armado de pista.\n");
		free(pista_usuario); // Liberar memoria de pista_usuario
		return volver(estado);
	}
	return false; // El comando no fue "b"
}

bool datos_de_modificacion_pista_validos(char **datos_modificacion,
					 bool hay_tipo_modificacion)
{
	bool son_validos = false;
	if (datos_modificacion && datos_modificacion[0] &&
	    datos_modificacion[1] && datos_modificacion[2]) {
		son_validos = true;
	}

	if (hay_tipo_modificacion) {
		if ((strcmp(datos_modificacion[0], "+") == 0 ||
		     strcmp(datos_modificacion[0], "-") == 0) &&
		    conversor_obstaculo_numero(datos_modificacion[1][0]) !=
			    -1 &&
		    atoi(datos_modificacion[2]) >= 0) {
			son_validos = true;
		} else {
			son_validos = false;
		}
	}

	return son_validos;
}

bool ejecutar_tipo_de_ejecucion(char **datos_modificacion, char **pista_usuario,
				estado_t *estado)
{
	char *tipo_modificacion = datos_modificacion[0];
	enum TP_OBSTACULO tipo_obstaculo =
		conversor_obstaculo_numero(datos_modificacion[1][0]);
	int posicion = atoi(datos_modificacion[2]);

	if (strcmp(tipo_modificacion, "+") == 0) {
		tp_agregar_obstaculo(estado->juego, JUGADOR_1, tipo_obstaculo,
				     (unsigned)posicion);
		free(*pista_usuario); // Liberar memoria antigua
		*pista_usuario =
			NULL; // Reiniciar pista_usuario para obtener nueva pista
	} else if (strcmp(tipo_modificacion, "-") == 0) {
		tp_quitar_obstaculo(estado->juego, JUGADOR_1,
				    (unsigned)posicion);
		free(*pista_usuario); // Liberar memoria antigua
		*pista_usuario =
			NULL; // Reiniciar pista_usuario para obtener nueva pista
	} else {
		printf("Comando inválido. Formato esperado: + TIPO_OBSTACULO POSICION (ej. + F 3)\n");
	}

	return true;
}

bool agregar_o_quitar_obstaculo(char *linea, estado_t *estado,
				char **pista_usuario)
{
	char separador = ' ';
	char **datos_modificacion = split(linea, separador);

	if (!datos_modificacion) {
		return false;
	}

	if (datos_de_modificacion_pista_validos(datos_modificacion, true)) {
		ejecutar_tipo_de_ejecucion(datos_modificacion, pista_usuario,
					   estado);
	} else {
		printf("Comando inválido. Formato esperado: + TIPO_OBSTACULO POSICION (ej. + F 3)\n");
		liberar_memoria_split(datos_modificacion, 3);
		return false;
	}

	liberar_memoria_split(datos_modificacion, 3);
	return true;
}

int intentos_totales(int dificultad)
{
	switch (dificultad - 1) {
	case FACIL:
		return 5;
		break;
	case NORMAL:
		return 3;
		break;
	case DIFICIL:
		return 2;
		break;
	case IMPOSIBLE:
		return 1;
		break;
	default:
		return -1;
		break;
	}
}

bool inicializar_dificultad(void *e)
{
	estado_t *estado = (estado_t *)e;
	int dificultad_elegida = atoi(estado->fase_actual->entrada);
	if (dificultad_elegida >= 1 && dificultad_elegida <= 4) {
		estado->dificultad = dificultad_elegida;
		destruir_fase(estado->fase_actual);
		estado->fase_actual = armado_de_atributos_oponente();
		estado->fase_actual->contenido(estado);
		estado->intentos = intentos_totales(estado->dificultad);
		estado->continuar = true;
		return true;
	}

	return false;
}

bool salir(void *e)
{
	estado_t *estado = (estado_t *)e;
	destruir_fase(estado->fase_actual);
	estado->continuar = false;
	return true;
}

bool pasar_a_seleccionar_pokemon(void *e)
{
	estado_t *estado = (estado_t *)e;
	destruir_fase(estado->fase_actual);
	estado->fase_actual = inicializar_pokemon_usuario(estado);
	estado->fase_actual->contenido(estado);
	estado->continuar = true;
	return true;
}

bool seleccion_pokemon_usuario(void *e)
{
	estado_t *estado = (estado_t *)e;
	if (tp_seleccionar_pokemon(estado->juego, JUGADOR_1,
				   estado->fase_actual->entrada)) {
		const struct pokemon_info *pokemon_usuario =
			tp_pokemon_seleccionado(estado->juego, JUGADOR_1);
		mostrar_info_pokemon(pokemon_usuario);
		printf("\nSi el pokemon seleccionado es correcto, oprime 'c', para continuar o ingresa otro nombre:\n");
		return true;
	} else {
		printf("El nombre del pokemon ya ha sido seleccionado.");
	}

	return false;
}

bool pasar_a_armar_pista(void *e)
{
	estado_t *estado = (estado_t *)e;
	if (tp_pokemon_seleccionado(estado->juego, JUGADOR_1)) {
		destruir_fase(estado->fase_actual);
		estado->fase_actual = armar_pista_del_usuario();
		estado->fase_actual->contenido(estado);
		estado->continuar = true;
		return true;
	} else {
		printf("Por favor selecciona un pokemon.");
	}

	return true;
}

bool pasar_a_correr_carrera(void *e)
{
	estado_t *estado = (estado_t *)e;
	destruir_fase(estado->fase_actual);
	estado->fase_actual = correr_carrera();
	estado->fase_actual->contenido(estado);
	estado->continuar = true;
	return true;
}

bool pasar_a_fin_juego(void *e)
{
	estado_t *estado = (estado_t *)e;
	destruir_fase(estado->fase_actual);
	estado->fase_actual = fin_del_juego();
	estado->fase_actual->contenido(estado);
	estado->continuar = false;
	return true;
}

bool volver_a_modificar_pista(void *e)
{
	estado_t *estado = (estado_t *)e;
	estado->intentos--;
	if (estado->intentos >= 0) {
		destruir_fase(estado->fase_actual);
		estado->fase_actual = armar_pista_del_usuario();
		estado->fase_actual->contenido(estado);
		estado->continuar = true;
		return true;
	} else {
		return pasar_a_fin_juego(estado);
	}

	return false;
}

bool modificar_pista(void *e)
{
	estado_t *estado = (estado_t *)e;
	char *pista_usuario = NULL;

	while (true) {
		pista_usuario = mostrar_pista_usuario(pista_usuario, estado);

		printf("\n> ");
		char linea[200];
		if (fgets(linea, sizeof(linea), stdin)) {
			linea[strlen(linea) - 1] = '\0';
			if (volver_a_menu_armado_pista(linea, estado,
						       pista_usuario)) {
				return true;
			}

			if (strlen(linea) == 5) {
				if (!agregar_o_quitar_obstaculo(
					    linea, estado, &pista_usuario)) {
					free(pista_usuario);
					return false;
				}
			} else {
				printf("Comando inválido. Formato esperado: + TIPO_OBSTACULO POSICION (ej. + F 3)\n");
			}
		}
	}

	free(pista_usuario);
	return true;
}
