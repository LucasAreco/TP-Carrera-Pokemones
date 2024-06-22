#include "fase.h"
#include "comandos_juego.h"
#include "interfaces_menues.h"
#include <stdlib.h>
#include "split.h"

fase_t *seleccionar_dificultad()
{
	fase_t *fase_dificultad = inicializar_fase();
	if (!fase_dificultad) {
		return NULL;
	}
	menu_t *menu_dificultades = fase_dificultad->menu;
	menu_agregar_comando(menu_dificultades, "1",
			     "Elegir dificultad 'Fácil'.",
			     inicializar_dificultad);
	menu_agregar_comando(menu_dificultades, "2",
			     "Elegir dificultad 'Normal'.",
			     inicializar_dificultad);
	menu_agregar_comando(menu_dificultades, "3",
			     "Elegir dificultad 'Difícil'.",
			     inicializar_dificultad);
	menu_agregar_comando(menu_dificultades, "4",
			     "Elegir dificultad 'Imposible'.",
			     inicializar_dificultad);
	menu_agregar_comando(menu_dificultades, "Q", "Salir del juego.", salir);
	menu_agregar_comando(menu_dificultades, "H",
			     "Se imprime ayuda del menu.",
			     mostrar_comando_y_descripcion);

	fase_dificultad->contenido = mostrar_interfaz_dificultad;

	return fase_dificultad;
}

fase_t *armado_de_atributos_oponente()
{
	fase_t *fase_oponente = inicializar_fase();
	if (!fase_oponente) {
		return NULL;
	}
	menu_t *menu_oponente = fase_oponente->menu;
	menu_agregar_comando(menu_oponente, "Q", "Salir del juego.", salir);
	menu_agregar_comando(menu_oponente, "C", "Continuar el juego",
			     pasar_a_seleccionar_pokemon);
	menu_agregar_comando(menu_oponente, "H", "Se imprime ayuda del menu.",
			     mostrar_comando_y_descripcion);

	fase_oponente->contenido = mostrar_interfaz_info_oponente;

	return fase_oponente;
}

fase_t *inicializar_pokemon_usuario(estado_t *estado)
{
	fase_t *fase_usuario = inicializar_fase();
	if (!fase_usuario) {
		return NULL;
	}
	menu_t *menu_usuario = fase_usuario->menu;

	char separador = ',';
	char *nombres_pokemon = tp_nombres_disponibles(estado->juego);
	char **pokemones_disponibles = split(nombres_pokemon, separador);

	for (int i = 0; i < tp_cantidad_pokemon(estado->juego); i++) {
		menu_agregar_comando(menu_usuario, pokemones_disponibles[i],
				     "Seleccion el pokemon con ese nombre.",
				     seleccion_pokemon_usuario);
	}

	menu_agregar_comando(menu_usuario, "Q", "Salir del juego.", salir);
	menu_agregar_comando(menu_usuario, "C", "Continuar el juego",
			     pasar_a_armar_pista);
	menu_agregar_comando(menu_usuario, "H", "Se imprime ayuda del menu.",
			     mostrar_comando_y_descripcion);

	fase_usuario->contenido = mostrar_info_pokemon_usuario;

	liberar_memoria_split(pokemones_disponibles,
			      tp_cantidad_pokemon(estado->juego));
	free(nombres_pokemon);

	return fase_usuario;
}

fase_t *armar_pista_del_usuario()
{
	fase_t *fase_pista_usuario = inicializar_fase();
	if (!fase_pista_usuario) {
		return NULL;
	}

	menu_t *menu_pista_usuario = fase_pista_usuario->menu;

	menu_agregar_comando(menu_pista_usuario, "M",
			     "Descripción de la entrada", modificar_pista);
	menu_agregar_comando(menu_pista_usuario, "Q", "Salir del juego.",
			     salir);
	menu_agregar_comando(menu_pista_usuario, "C", "Continuar el juego",
			     pasar_a_correr_carrera);
	menu_agregar_comando(menu_pista_usuario, "H",
			     "Se imprime ayuda del menu.",
			     mostrar_comando_y_descripcion);

	fase_pista_usuario->contenido = mostrar_info_armado_pista;

	return fase_pista_usuario;
}

fase_t *correr_carrera()
{
	fase_t *fase_carrera = inicializar_fase();
	if (!fase_carrera) {
		return NULL;
	}

	menu_t *menu_carrera = fase_carrera->menu;

	menu_agregar_comando(menu_carrera, "Q", "Salir del juego.", salir);
	menu_agregar_comando(menu_carrera, "F", "Continuar el juego",
			     pasar_a_fin_juego);
	menu_agregar_comando(menu_carrera, "A", "Volver a modificar la pista",
			     volver_a_modificar_pista);
	menu_agregar_comando(menu_carrera, "H", "Se imprime ayuda del menu.",
			     mostrar_comando_y_descripcion);

	fase_carrera->contenido = mostrar_resultado_carrera;

	return fase_carrera;
}

fase_t *fin_del_juego()
{
	fase_t *juego_finalizado = inicializar_fase();
	if (!juego_finalizado) {
		return NULL;
	}

	juego_finalizado->contenido = mostrar_interfaz_fin_del_juego;

	return juego_finalizado;
}