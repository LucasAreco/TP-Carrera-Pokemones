#include "pa2m.h"
#include "src/tp.h"
#include "src/menu.h"
#include <string.h>
#include <stdlib.h>

#define ARCHIVO_PRUEBA "pokemones.txt"
#define ARCHIVO_VACIO "sinpokemones.txt"
#define POKEMONES_TOTALES 25

void cuando_se_crea_tp_nulo_se_devuelve_null()
{
	TP *tp = tp_crear(NULL);
	pa2m_afirmar(!tp, "Crear un tp con archivo NULL, devuelve NULL.");
}

void cuando_se_crea_tp_nulo_se_esperan_cero_pokemones()
{
	TP *tp = tp_crear(NULL);
	pa2m_afirmar(tp_cantidad_pokemon(tp) == 0,
		     "Cantidad de pokemones en tp con archivo NULL es 0.");
}

void cuando_se_crea_tp_válido_se_devuelve_el_tp()
{
	TP *tp = tp_crear(ARCHIVO_PRUEBA);
	pa2m_afirmar(tp != NULL, "Crear un tp con archivo válido, no es NULL.");
	tp_destruir(tp);
}

void cuando_se_crea_tp_válido_se_espera_que_tenga_pokemones()
{
	TP *nuevo_tp = tp_crear(ARCHIVO_PRUEBA);
	pa2m_afirmar(nuevo_tp != NULL, "El TP se crea correctamente.");
	pa2m_afirmar(
		tp_cantidad_pokemon(nuevo_tp) == POKEMONES_TOTALES,
		"Al crear un tp válido la cantidad de pokemones es correcta.");
	tp_destruir(nuevo_tp);
}

void cuando_se_busca_un_nombre_existente_se_devuelve_el_pokemon()
{
	TP *tp = tp_crear(ARCHIVO_PRUEBA);
	const char *nombre_existente = "Pikachu";
	const char *nombre_existente_2 = "Rattata";

	const struct pokemon_info *poke_buscado_1 =
		tp_buscar_pokemon(tp, nombre_existente);
	pa2m_afirmar(
		poke_buscado_1 != NULL,
		"Al buscar con nombre de pokemon existente, devuelve el pokemon.");

	const struct pokemon_info *poke_buscado_2 =
		tp_buscar_pokemon(tp, nombre_existente_2);
	pa2m_afirmar(
		poke_buscado_2 != NULL,
		"Al buscar con nombre de pokemon existente, devuelve el pokemon.");

	tp_destruir(tp);
}

void cuando_se_busca_un_nombre_en_minuscula_se_devuelve_el_pokemon()
{
	TP *tp = tp_crear(ARCHIVO_PRUEBA);

	const char *nombre_minuscula = "dratini";
	const char *nombre_minuscula_2 = "vulpix";

	const struct pokemon_info *poke_buscado_3 =
		tp_buscar_pokemon(tp, nombre_minuscula);
	pa2m_afirmar(
		poke_buscado_3 != NULL,
		"Al buscar con nombre de pokemon en minuscula, devuelve el pokemon.");

	const struct pokemon_info *poke_buscado_4 =
		tp_buscar_pokemon(tp, nombre_minuscula_2);
	pa2m_afirmar(
		poke_buscado_4 != NULL,
		"Al buscar con nombre de pokemon en minuscula, devuelve el pokemon.");

	tp_destruir(tp);
}

void cuando_se_busca_un_nombre_irregular_se_devuelve_el_pokemon()
{
	TP *tp = tp_crear(ARCHIVO_PRUEBA);
	const char *nombre_irregular = "SaNdShReW";
	const char *nombre_irregular_2 = "PikachU";

	const struct pokemon_info *poke_buscado_5 =
		tp_buscar_pokemon(tp, nombre_irregular);
	pa2m_afirmar(
		poke_buscado_5 != NULL,
		"Al buscar con un nombre de pokemon combinando minúsculas y mayúsculas, devuelve el pokemon.");

	const struct pokemon_info *poke_buscado_6 =
		tp_buscar_pokemon(tp, nombre_irregular_2);
	pa2m_afirmar(
		poke_buscado_6 != NULL,
		"Al buscar con un nombre de pokemon combinando minúsculas y mayúsculas, devuelve el pokemon.");

	tp_destruir(tp);
}

void cuando_se_busca_un_nombre_inexistente_se_devuelve_null()
{
	TP *tp = tp_crear(ARCHIVO_PRUEBA);

	const char *nombre_inexistente = "Pikodulce";
	const char *nombre_inexistente_2 = "mi_pokemon";

	const struct pokemon_info *poke_buscado_7 =
		tp_buscar_pokemon(tp, nombre_inexistente);
	pa2m_afirmar(poke_buscado_7 == NULL,
		     "Al buscar con un nombre inválido, devuelve NULL.");

	const struct pokemon_info *poke_buscado_8 =
		tp_buscar_pokemon(tp, nombre_inexistente_2);
	pa2m_afirmar(poke_buscado_8 == NULL,
		     "Al buscar con un nombre inválido, devuelve NULL.");

	const struct pokemon_info *poke_buscado_9 = tp_buscar_pokemon(tp, NULL);
	pa2m_afirmar(poke_buscado_9 == NULL,
		     "Al buscar con un nombre inválido, devuelve NULL.");

	tp_destruir(tp);
}

void cuando_se_busca_un_pokemon_con_argumentos_nulos_se_devuelve_null()
{
	const char *nombre_minuscula = "dratini";
	const struct pokemon_info *poke_buscado_10 =
		tp_buscar_pokemon(NULL, nombre_minuscula);
	pa2m_afirmar(poke_buscado_10 == NULL,
		     "Al buscar en un tp inválido, devuelve NULL.");
}

void cuando_se_pide_cantidad_de_pokemones_se_devuelve_la_cantidad_correcta()
{
	TP *tp = tp_crear(ARCHIVO_PRUEBA);

	pa2m_afirmar(tp_cantidad_pokemon(tp) == 25,
		     "La cantidad de pokemones es correcta.");

	tp_destruir(tp);
}

void cuando_se_solicita_pokemones_disponibles_se_devuelve_un_string_con_los_pokemones()
{
	TP *tp = tp_crear(ARCHIVO_PRUEBA);

	char *pokemones_disponibles = tp_nombres_disponibles(tp);

	pa2m_afirmar(pokemones_disponibles != NULL,
		     "Los pokemones disponibles son correctos.");

	size_t cantidad_pokemones = 0;
	for (size_t i = 0; pokemones_disponibles[i] != '\0'; i++) {
		if (pokemones_disponibles[i] == ',') {
			cantidad_pokemones++;
		}
	}
	cantidad_pokemones++;

	pa2m_afirmar(cantidad_pokemones == tp_cantidad_pokemon(tp),
		     "La cantidad de pokemones disponibles es correcta.");

	printf("%s", pokemones_disponibles);

	free(pokemones_disponibles);

	tp_destruir(tp);
}

void cuando_se_solicita_pokemones_disponibles_no_se_muestran_los_pokemones_ya_seleccionados()
{
	TP *tp = tp_crear(ARCHIVO_PRUEBA);

	tp_seleccionar_pokemon(tp, JUGADOR_1, "pikachu");
	tp_seleccionar_pokemon(tp, JUGADOR_2, "charizard");

	char *pokemones_disponibles = tp_nombres_disponibles(tp);

	pa2m_afirmar(pokemones_disponibles != NULL,
		     "Los pokemones disponibles son correctos.");

	size_t cantidad_pokemones = 0;
	for (size_t i = 0; pokemones_disponibles[i] != '\0'; i++) {
		if (pokemones_disponibles[i] == ',') {
			cantidad_pokemones++;
		}
	}
	cantidad_pokemones++;

	pa2m_afirmar(cantidad_pokemones == tp_cantidad_pokemon(tp) - 2,
		     "La cantidad de pokemones disponibles es correcta.");

	printf("%s", pokemones_disponibles);

	free(pokemones_disponibles);

	tp_destruir(tp);
}

void cuando_se_selecciona_un_pokemon_para_jugador_sin_pokemon_se_devuelve_true()
{
	TP *tp = tp_crear(ARCHIVO_PRUEBA);
	const char *nombre_existente = "Pikachu";
	const char *nombre_existente_2 = "Raichu";

	bool seleccion_jugador_2 =
		tp_seleccionar_pokemon(tp, JUGADOR_2, nombre_existente_2);
	pa2m_afirmar(seleccion_jugador_2 == true,
		     "Jugador 2 puede seleccionar pokemon correctamente.");

	const struct pokemon_info *poke_elegido_jugador_2 =
		tp_pokemon_seleccionado(tp, JUGADOR_2);
	pa2m_afirmar(poke_elegido_jugador_2 != NULL &&
			     strcmp(poke_elegido_jugador_2->nombre,
				    nombre_existente_2) == 0,
		     "Jugador 2 tiene el pokemon correcto seleccionado.");

	bool seleccion_jugador_1 =
		tp_seleccionar_pokemon(tp, JUGADOR_1, nombre_existente);
	pa2m_afirmar(seleccion_jugador_1 == true,
		     "Jugador 1 puede seleccionar pokemon correctamente.");

	const struct pokemon_info *poke_elegido_jugador_1 =
		tp_pokemon_seleccionado(tp, JUGADOR_1);
	pa2m_afirmar(poke_elegido_jugador_1 != NULL &&
			     strcmp(poke_elegido_jugador_1->nombre,
				    nombre_existente) == 0,
		     "Jugador 1 tiene el pokemon correcto seleccionado.");

	tp_destruir(tp);
}

void cuando_se_selecciona_un_pokemon_utilizado_por_otro_jugador_se_devuelve_false()
{
	TP *tp = tp_crear(ARCHIVO_PRUEBA);
	const char *nombre_existente = "Pikachu";
	const char *nombre_existente_2 = "Ekans";

	bool seleccion_jugador_2 =
		tp_seleccionar_pokemon(tp, JUGADOR_2, nombre_existente);
	pa2m_afirmar(seleccion_jugador_2 == true,
		     "Jugador 2 selecciona pokemon correctamente.");

	const struct pokemon_info *pokemon_seleccionado_jugador_2 =
		tp_pokemon_seleccionado(tp, JUGADOR_2);
	pa2m_afirmar(pokemon_seleccionado_jugador_2 != NULL &&
			     strcmp(pokemon_seleccionado_jugador_2->nombre,
				    nombre_existente) == 0,
		     "El pokemon seleccionado por Jugador 2 es correcto.");

	bool seleccion_jugador_1 =
		tp_seleccionar_pokemon(tp, JUGADOR_1, nombre_existente);
	pa2m_afirmar(
		seleccion_jugador_1 == false,
		"Jugador 1 no puede seleccionar el mismo pokemon que Jugador 2.");

	const struct pokemon_info *pokemon_seleccionado_jugador_1 =
		tp_pokemon_seleccionado(tp, JUGADOR_1);
	pa2m_afirmar(pokemon_seleccionado_jugador_1 == NULL,
		     "El pokemon seleccionado por Jugador 1 es NULL.");

	bool seleccion_jugador_1_de_nuevo =
		tp_seleccionar_pokemon(tp, JUGADOR_1, nombre_existente_2);
	pa2m_afirmar(
		seleccion_jugador_1_de_nuevo == true,
		"Jugador 1 puede seleccionar un pokemon distinto al de Jugador 2.");

	const struct pokemon_info *pokemon_seleccionado_jugador_1_de_nuevo =
		tp_pokemon_seleccionado(tp, JUGADOR_1);
	pa2m_afirmar(
		pokemon_seleccionado_jugador_1_de_nuevo != NULL &&
			strcmp(pokemon_seleccionado_jugador_1_de_nuevo->nombre,
			       nombre_existente_2) == 0,
		"El pokemon seleccionado por Jugador 1 es correcto.");

	bool seleccion_jugador_2_de_nuevo =
		tp_seleccionar_pokemon(tp, JUGADOR_2, nombre_existente_2);
	pa2m_afirmar(
		seleccion_jugador_2_de_nuevo == false,
		"Jugador 2 no puede seleccionar el mismo pokemon que Jugador 1.");

	const struct pokemon_info *pokemon_seleccionado_jugador_2_de_nuevo =
		tp_pokemon_seleccionado(tp, JUGADOR_2);
	pa2m_afirmar(
		pokemon_seleccionado_jugador_2_de_nuevo != NULL &&
			strcmp(pokemon_seleccionado_jugador_2_de_nuevo->nombre,
			       nombre_existente) == 0,
		"El pokemon seleccionado por Jugador 2 es correcto.");

	tp_destruir(tp);
}

void cuando_se_selecciona_un_pokemon_teniendo_uno_se_actualiza_y_se_devuelve_true()
{
	TP *tp = tp_crear(ARCHIVO_PRUEBA);
	const char *nombre_existente = "Pikachu";
	const char *nombre_existente_2 = "Moltres";

	bool seleccion_jugador_1 =
		tp_seleccionar_pokemon(tp, JUGADOR_1, nombre_existente);
	pa2m_afirmar(seleccion_jugador_1 == true,
		     "Jugador 1 selecciona pokemon correctamente.");

	const struct pokemon_info *pokemon_seleccionado_jugador_1_original =
		tp_pokemon_seleccionado(tp, JUGADOR_1);
	pa2m_afirmar(
		pokemon_seleccionado_jugador_1_original != NULL &&
			strcmp(pokemon_seleccionado_jugador_1_original->nombre,
			       nombre_existente) == 0,
		"El pokemon seleccionado por Jugador 1 es correcto.");

	bool seleccion_jugador_1_nuevo =
		tp_seleccionar_pokemon(tp, JUGADOR_1, nombre_existente_2);
	pa2m_afirmar(
		seleccion_jugador_1_nuevo == true,
		"Jugador 1 selecciona nuevo pokemon y se actualiza correctamente.");

	const struct pokemon_info *pokemon_seleccionado_jugador_1_nuevo =
		tp_pokemon_seleccionado(tp, JUGADOR_1);
	pa2m_afirmar(
		pokemon_seleccionado_jugador_1_nuevo != NULL &&
			strcmp(pokemon_seleccionado_jugador_1_nuevo->nombre,
			       nombre_existente_2) == 0,
		"El nuevo pokemon seleccionado por Jugador 1 es correcto.");

	tp_destruir(tp);
}

void cuando_se_agrega_un_obstaculo_a_una_pista_se_espera_la_cantidad_correcta_de_obstaculos()
{
	TP *tp = tp_crear(ARCHIVO_PRUEBA);

	pa2m_afirmar(
		tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_FUERZA, 0) == 1,
		"Agrega un obstaculo en la primer posición de la pista del jugador 1.");
	pa2m_afirmar(
		tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_INTELIGENCIA,
				     1) == 2,
		"Agrega un obstaculo en la segunda posición de la pista del jugador 1.");
	pa2m_afirmar(
		tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_DESTREZA, 2) == 3,
		"Agrega un obstaculo en una posición  de la pista del jugador 1.");

	tp_destruir(tp);
}

void cuando_se_agrega_un_obstaculo_en_misma_posicion_que_otro_se_espera_que_se_desplecen_a_la_siguiente_posición()
{
	TP *tp = tp_crear(ARCHIVO_PRUEBA);

	pa2m_afirmar(
		tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_FUERZA, 0) == 1,
		"Agrega un obstaculo en la primer posición de la pista del jugador 2.");
	pa2m_afirmar(
		tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_INTELIGENCIA,
				     0) == 2,
		"Agrega un obstaculo en la misma posición que otro obstaculo en la pista del jugador 2, desplazando el otro a la derecha.");
	pa2m_afirmar(
		tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_FUERZA, 0) == 3,
		"Agrega un obstaculo en la misma posición que otro obstaculo en la pista del jugador 2, desplazando el otro a la derecha.");

	tp_destruir(tp);
}

void cuando_se_agrega_un_obstaculo_en_una_posición_fuera_del_rango_se_espera_que_se_agregue_al_final()
{
	TP *tp = tp_crear(ARCHIVO_PRUEBA);

	pa2m_afirmar(
		tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_DESTREZA, 0) == 1,
		"Agrega un obstaculo en la primer posición de la pista del jugador 2.");
	pa2m_afirmar(
		tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_DESTREZA, 10) ==
			2,
		"Agrega un obstaculo en una posición fuera de rango y se agrega al final de la pista del jugador 2.");
	pa2m_afirmar(
		tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_INTELIGENCIA,
				     20) == 3,
		"Agrega un obstaculo en una posición fuera de rango y se agrega al final de la pista del jugador 2.");

	tp_destruir(tp);
}

void cuando_se_quita_un_obstaculo_de_una_pista_se_espera_la_cantidad_actual_total_de_obstaculos()
{
	TP *tp = tp_crear(ARCHIVO_PRUEBA);

	tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_FUERZA, 0);
	tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_DESTREZA, 2);
	tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_INTELIGENCIA, 1);

	pa2m_afirmar(
		tp_quitar_obstaculo(tp, JUGADOR_2, 1) == 2,
		"Se quita correctamente el obstaculo en la posicion y devuelve la cantidad correcta de obstaculos.");
	pa2m_afirmar(
		tp_quitar_obstaculo(tp, JUGADOR_2, 1) == 1,
		"Se quita correctamente el obstaculo en la posicion y devuelve la cantidad correcta de obstaculos.");
	pa2m_afirmar(
		tp_quitar_obstaculo(tp, JUGADOR_2, 0) == 0,
		"Se quita correctamente el último obstaculo de la pista y la pista esta vacía.");

	tp_destruir(tp);
}

void cuando_se_solicitan_los_obstaculos_de_una_pista_se_espera_un_string_con_los_obstaculos()
{
	TP *tp = tp_crear(ARCHIVO_PRUEBA);

	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_FUERZA, 0);
	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_DESTREZA, 2);
	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_INTELIGENCIA, 1);
	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_INTELIGENCIA, 10);
	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_INTELIGENCIA, 1);

	char *obstaculos_pista = tp_obstaculos_pista(tp, JUGADOR_1);

	pa2m_afirmar(obstaculos_pista != NULL,
		     "Los obstaculos en la pista son correctos.");

	printf("%s", obstaculos_pista);

	free(obstaculos_pista);

	tp_destruir(tp);
}

void cuando_se_limpia_la_pista_se_espera_que_la_pista_no_tenga_obstaculos()
{
	TP *tp = tp_crear(ARCHIVO_PRUEBA);

	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_FUERZA, 0);
	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_DESTREZA, 2);
	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_INTELIGENCIA, 1);
	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_INTELIGENCIA, 10);
	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_INTELIGENCIA, 1);

	tp_limpiar_pista(tp, JUGADOR_1);

	char *obstaculos_pista = tp_obstaculos_pista(tp, JUGADOR_1);

	pa2m_afirmar(obstaculos_pista == NULL,
		     "Se eliminan los obstaculos correctamente.");

	free(obstaculos_pista);

	tp_destruir(tp);
}

void cuando_se_calcula_el_tiempo_por_obstaculo_se_espera_un_string_csv()
{
	TP *tp = tp_crear(ARCHIVO_PRUEBA);

	char *poke = "Pikachu";

	tp_seleccionar_pokemon(tp, JUGADOR_1, poke);

	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_FUERZA, 0);
	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_DESTREZA, 1);
	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_DESTREZA, 1);
	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_INTELIGENCIA, 1);
	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_INTELIGENCIA, 4);
	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_INTELIGENCIA, 5);
	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_FUERZA, 6);

	char *tiempos_obstaculos = tp_tiempo_por_obstaculo(tp, JUGADOR_1);

	pa2m_afirmar(
		tiempos_obstaculos != NULL,
		"Se calculan los tiempos de los obstaculos correctamente.");

	free(tiempos_obstaculos);

	tp_destruir(tp);
}

void cuando_se_calcula_el_tiempo_por_obstaculo_se_espera_que_considere_sus_anteriores()
{
	TP *tp = tp_crear(ARCHIVO_PRUEBA);

	char *poke = "charizard";

	tp_seleccionar_pokemon(tp, JUGADOR_2, poke);

	tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_INTELIGENCIA, 0);
	tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_INTELIGENCIA, 1);
	tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_INTELIGENCIA, 2);
	tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_DESTREZA, 3);

	char *tiempos_obstaculos = tp_tiempo_por_obstaculo(tp, JUGADOR_2);

	pa2m_afirmar(
		tiempos_obstaculos != NULL,
		"Se calculan los tiempos de los obstaculos correctamente.");

	free(tiempos_obstaculos);

	tp_destruir(tp);
}

void cuando_se_calcula_tiempo_pista_se_devuelve_un_numero_válido()
{
	TP *tp = tp_crear(ARCHIVO_PRUEBA);

	char *poke = "charizard";

	tp_seleccionar_pokemon(tp, JUGADOR_2, poke);

	tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_INTELIGENCIA, 0);
	tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_INTELIGENCIA, 1);
	tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_INTELIGENCIA, 2);
	tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_DESTREZA, 3);
	tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_FUERZA, 3);
	tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_DESTREZA, 1);

	char *tiempos_obstaculos = tp_tiempo_por_obstaculo(tp, JUGADOR_2);

	char *pista = tp_obstaculos_pista(tp, JUGADOR_2);

	printf("%s", pista);

	unsigned tiempo_pista = tp_calcular_tiempo_pista(tp, JUGADOR_2);

	pa2m_afirmar(
		tiempos_obstaculos != NULL,
		"Se calculan los tiempos de los obstaculos correctamente.");

	printf("%s\n", tiempos_obstaculos);
	printf("%u", tiempo_pista);

	free(tiempos_obstaculos);

	free(pista);

	tp_destruir(tp);
}

bool imprimir_texto(void *texto)
{
	char *mi_texto = texto;
	printf("Se imprime: %s\n", mi_texto);
	return true;
}

bool imprimir_resultado(void *operacion)
{
	int resultado = *(int *)operacion;
	printf("El resultado es: %i\n", resultado);
	return true;
}

bool imprimir_resta(void *contexto)
{
	int *numeros = (int *)contexto;
	printf("Resta: %d\n", numeros[0] - numeros[1]);
	return true;
}

bool imprimir_multiplicacion(void *contexto)
{
	int *numeros = (int *)contexto;
	printf("Multiplicación: %d\n", numeros[0] * numeros[1]);
	return true;
}

bool imprimir_division(void *contexto)
{
	int *numeros = (int *)contexto;
	if (numeros[1] == 0) {
		printf("División por cero\n");
		return false;
	}
	printf("División: %d\n", numeros[0] / numeros[1]);
	return true;
}

void cuando_se_crea_un_menu_se_devuelve_correctamente_un_menu()
{
	menu_t *menu = menu_crear();

	pa2m_afirmar(menu != NULL, "Se crea un menu correctamente.");

	menu_destruir(menu);
}

void cuando_se_agrega_un_comando_con_argumentos_inválidos_se_espera_que_no_se_cree_el_comando()
{
	menu_agregar_comando(NULL, "imprimir_texto", "Imprime texto",
			     imprimir_texto);
	pa2m_afirmar(true, "Se intentó agregar un comando con menú nulo.");
	pa2m_afirmar(menu_contiene_comando(NULL, "imprimir_texto") == false,
		     "El comando no es agregado al menú.");

	menu_t *menu = menu_crear();
	pa2m_afirmar(menu != NULL, "Se crea un menú correctamente.");

	menu_agregar_comando(menu, NULL, "Imprime resultado operación",
			     imprimir_resultado);
	pa2m_afirmar(true, "Se intentó agregar un comando con nombre nulo.");
	pa2m_afirmar(!menu_contiene_comando(menu, NULL),
		     "El comando no es agregado al menú.");

	menu_agregar_comando(menu, "imprimir_suma", NULL, imprimir_resultado);
	pa2m_afirmar(true,
		     "Se intentó agregar un comando con descripción nula.");
	pa2m_afirmar(!menu_contiene_comando(menu, "imprimir_suma"),
		     "El comando no es agregado al menú.");

	menu_agregar_comando(menu, "imprimir_resta",
			     "Imprime resultado de una resta", NULL);
	pa2m_afirmar(true, "Se intentó agregar un comando con función nula.");
	pa2m_afirmar(!menu_contiene_comando(menu, "imprimir_resta"),
		     "El comando no es agregado al menú.");

	menu_destruir(menu);
}

void cuando_se_agrega_un_comando_válido_se_espera_que_se_cree_el_comando()
{
	menu_t *menu = menu_crear();

	pa2m_afirmar(menu != NULL, "Se crea un menu correctamente.");

	menu_agregar_comando(menu, "imprimir_texto", "Imprime texto",
			     imprimir_texto);
	pa2m_afirmar(true, "Se agrega un comando.");
	pa2m_afirmar(menu_contiene_comando(menu, "imprimir_texto"),
		     "El comando es agregado correctamente al menu.");

	menu_agregar_comando(menu, "imprimir_resultado",
			     "Imprime resutlado operación", imprimir_resultado);
	pa2m_afirmar(true, "Se agrega un comando.");
	pa2m_afirmar(menu_contiene_comando(menu, "imprimir_resultado"),
		     "El comando es agregado correctamente al menu.");

	menu_destruir(menu);
}

void cuando_se_ejecuta_un_comando_existente_se_devuelve_menu_ok()
{
	menu_t *menu = menu_crear();

	menu_agregar_comando(menu, "imprimir_texto", "Imprime el texto dado",
			     imprimir_texto);
	pa2m_afirmar(true, "Se agrega un comando.");
	pa2m_afirmar(
		menu_contiene_comando(menu, "imprimir_texto"),
		"El comando 'imprimir_texto' es agregado correctamente al menú.");
	pa2m_afirmar(menu_ejecutar_comando(menu, "imprimir_texto",
					   "Hola, mundo!") == MENU_OK,
		     "Se ejecuta correctamente el comando 'imprimir_texto'.");

	menu_agregar_comando(menu, "imprimir_resultado",
			     "Imprime el resultado de la operación",
			     imprimir_resultado);
	pa2m_afirmar(true, "Se agrega un comando.");
	pa2m_afirmar(
		menu_contiene_comando(menu, "imprimir_resultado"),
		"El comando 'imprimir_resultado' es agregado correctamente al menú.");
	int resultado = 42;
	pa2m_afirmar(
		menu_ejecutar_comando(menu, "imprimir_resultado", &resultado) ==
			MENU_OK,
		"Se ejecuta correctamente el comando 'imprimir_resultado'.");

	menu_agregar_comando(menu, "imprimir_resta",
			     "Imprime la resta de dos números", imprimir_resta);
	pa2m_afirmar(true, "Se agrega un comando.");
	pa2m_afirmar(
		menu_contiene_comando(menu, "imprimir_resta"),
		"El comando 'imprimir_resta' es agregado correctamente al menú.");
	int resta[2] = { 10, 4 };
	pa2m_afirmar(menu_ejecutar_comando(menu, "imprimir_resta", resta) ==
			     MENU_OK,
		     "Se ejecuta correctamente el comando 'imprimir_resta'.");

	menu_agregar_comando(menu, "imprimir_multiplicacion",
			     "Imprime la multiplicación de dos números",
			     imprimir_multiplicacion);
	pa2m_afirmar(true, "Se agrega un comando.");
	pa2m_afirmar(
		menu_contiene_comando(menu, "imprimir_multiplicacion"),
		"El comando 'imprimir_multiplicacion' es agregado correctamente al menú.");
	int multiplicacion[2] = { 7, 6 };
	pa2m_afirmar(
		menu_ejecutar_comando(menu, "imprimir_multiplicacion",
				      multiplicacion) == MENU_OK,
		"Se ejecuta correctamente el comando 'imprimir_multiplicacion'.");

	menu_agregar_comando(menu, "imprimir_division",
			     "Imprime la división de dos números",
			     imprimir_division);
	pa2m_afirmar(true, "Se agrega un comando.");
	pa2m_afirmar(
		menu_contiene_comando(menu, "imprimir_division"),
		"El comando 'imprimir_division' es agregado correctamente al menú.");
	int division[2] = { 20, 5 };
	pa2m_afirmar(
		menu_ejecutar_comando(menu, "imprimir_division", division) ==
			MENU_OK,
		"Se ejecuta correctamente el comando 'imprimir_division'.");

	menu_destruir(menu);
}

void cuando_se_ejecuta_un_comando_inexistente_se_devuelve_comando_inexistente()
{
	menu_t *menu = menu_crear();

	int suma = 2000 + 24;

	pa2m_afirmar(menu_ejecutar_comando(menu, NULL, &suma) ==
			     COMANDO_INEXISTENTE,
		     "No se ejecuta un comando inexistente.");

	menu_destruir(menu);
}

// bool imprimir_comando_descripcion(const char *clave, const char *descripcion, bool (*funcion)(void*), void *aux) {
//     printf("Comando: %s\nDescripción: %s\n", clave, descripcion);
//     return true;
// }

// void cuando_se_iteran_los_comandos_se_espera_que_se_impriman_los_comandos_y_las_descripciones() {
//     menu_t* menu = menu_crear();

//     menu_agregar_comando(menu, "imprimir_texto", "Imprime el texto dado", imprimir_texto);
//     pa2m_afirmar(true, "Se agrega un comando.");
//     pa2m_afirmar(menu_contiene_comando(menu, "imprimir_texto"), "El comando 'imprimir_texto' es agregado correctamente al menú.");

//     menu_agregar_comando(menu, "imprimir_resta", "Imprime la resta de dos números", imprimir_resta);
//     pa2m_afirmar(true, "Se agrega un comando.");
//     pa2m_afirmar(menu_contiene_comando(menu, "imprimir_resta"), "El comando 'imprimir_resta' es agregado correctamente al menú.");

//     menu_agregar_comando(menu, "imprimir_multiplicacion", "Imprime la multiplicación de dos números", imprimir_multiplicacion);
//     pa2m_afirmar(true, "Se agrega un comando.");
//     pa2m_afirmar(menu_contiene_comando(menu, "imprimir_multiplicacion"), "El comando 'imprimir_multiplicacion' es agregado correctamente al menú.");

//     menu_agregar_comando(menu, "imprimir_division", "Imprime la división de dos números", imprimir_division);
//     pa2m_afirmar(true, "Se agrega un comando.");
//     pa2m_afirmar(menu_contiene_comando(menu, "imprimir_division"), "El comando 'imprimir_division' es agregado correctamente al menú.");

//     pa2m_afirmar(true, "Se muestran todos los comandos agregados y su descripción.");
//     menu_con_cada_comando(menu, imprimir_comando_descripcion, NULL);

//     menu_destruir(menu);
// }

int main()
{
	pa2m_nuevo_grupo("PRUEBAS DE TP");

	pa2m_nuevo_grupo("Creación de tp con archivo nulo");
	cuando_se_crea_tp_nulo_se_devuelve_null();
	cuando_se_crea_tp_nulo_se_esperan_cero_pokemones();

	pa2m_nuevo_grupo("Creación de tp");
	cuando_se_crea_tp_válido_se_devuelve_el_tp();
	cuando_se_crea_tp_válido_se_espera_que_tenga_pokemones();

	pa2m_nuevo_grupo("Cantidad de pokemones");
	cuando_se_pide_cantidad_de_pokemones_se_devuelve_la_cantidad_correcta();

	pa2m_nuevo_grupo("Busqueda de pokemon");
	cuando_se_busca_un_nombre_existente_se_devuelve_el_pokemon();
	cuando_se_busca_un_nombre_en_minuscula_se_devuelve_el_pokemon();
	cuando_se_busca_un_nombre_irregular_se_devuelve_el_pokemon();
	cuando_se_busca_un_nombre_inexistente_se_devuelve_null();

	pa2m_nuevo_grupo("Disponibilidad de pokemones");
	cuando_se_solicita_pokemones_disponibles_se_devuelve_un_string_con_los_pokemones();
	cuando_se_solicita_pokemones_disponibles_no_se_muestran_los_pokemones_ya_seleccionados();

	pa2m_nuevo_grupo("Selección de pokemon");
	cuando_se_selecciona_un_pokemon_para_jugador_sin_pokemon_se_devuelve_true();
	cuando_se_selecciona_un_pokemon_utilizado_por_otro_jugador_se_devuelve_false();
	cuando_se_selecciona_un_pokemon_teniendo_uno_se_actualiza_y_se_devuelve_true();

	pa2m_nuevo_grupo("Agregar obstaculos a una pista");
	cuando_se_agrega_un_obstaculo_a_una_pista_se_espera_la_cantidad_correcta_de_obstaculos();
	cuando_se_agrega_un_obstaculo_en_misma_posicion_que_otro_se_espera_que_se_desplecen_a_la_siguiente_posición();
	cuando_se_agrega_un_obstaculo_en_una_posición_fuera_del_rango_se_espera_que_se_agregue_al_final();

	pa2m_nuevo_grupo("Quitar obstaculos a una pista");
	cuando_se_quita_un_obstaculo_de_una_pista_se_espera_la_cantidad_actual_total_de_obstaculos();

	pa2m_nuevo_grupo("Obstaculos en pista");
	cuando_se_solicitan_los_obstaculos_de_una_pista_se_espera_un_string_con_los_obstaculos();

	pa2m_nuevo_grupo("Limpiar pista");
	cuando_se_limpia_la_pista_se_espera_que_la_pista_no_tenga_obstaculos();

	pa2m_nuevo_grupo("Calculo de tiempo por obstaculo");
	cuando_se_calcula_el_tiempo_por_obstaculo_se_espera_un_string_csv();
	cuando_se_calcula_el_tiempo_por_obstaculo_se_espera_que_considere_sus_anteriores();

	pa2m_nuevo_grupo("Calculo de tiempo total de la pista");
	cuando_se_calcula_tiempo_pista_se_devuelve_un_numero_válido();

	printf("\n\n\n");
	pa2m_nuevo_grupo("PRUEBAS DE MENU");

	pa2m_nuevo_grupo("Creación de menú");
	cuando_se_crea_un_menu_se_devuelve_correctamente_un_menu();

	pa2m_nuevo_grupo("Agregar un comando");
	cuando_se_agrega_un_comando_con_argumentos_inválidos_se_espera_que_no_se_cree_el_comando();
	cuando_se_agrega_un_comando_válido_se_espera_que_se_cree_el_comando();

	pa2m_nuevo_grupo("Ejecutar un comando");
	cuando_se_ejecuta_un_comando_existente_se_devuelve_menu_ok();
	cuando_se_ejecuta_un_comando_inexistente_se_devuelve_comando_inexistente();

	pa2m_nuevo_grupo("Iterar comandos");
	// cuando_se_iteran_los_comandos_se_espera_que_se_impriman_los_comandos_y_las_descripciones();

	return pa2m_mostrar_reporte();
}
