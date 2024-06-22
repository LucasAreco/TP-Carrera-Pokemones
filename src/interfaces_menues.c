#include "tp.h"
#include "fase.h"
#include "comandos_juego.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>
#include "split.h"
#include "comandos_juego.h"

#include "interfaces_menues.h"

void limpiar_pantalla_completa()
{
	printf("\033[2J\033[H");
}


bool imprimir_comando_descripcion(const char *clave, const char *descripcion,
				  bool (*funcion)(void *), void *aux)
{
	printf("Comando: %s\t %s\n", clave, descripcion);
	return true;
}

bool mostrar_comando_y_descripcion(void *contexto)
{
	estado_t *estado = (estado_t *)contexto;
	menu_con_cada_comando(estado->fase_actual->menu,
			      imprimir_comando_descripcion, NULL);
	return true;
}


void mostrar_interfaz_inicio(void *contexto) {
	limpiar_pantalla_completa();
    printf("\n\n\n\n"
    "\t  /$$$$$$                                                                      /$$                 /$$$$$$$          /$$                                                                      \n"
    "\t  /$$__  $$                                                                    | $$                | $$__  $$        | $$                                                                      \n"
    "\t | $$  \\__/ /$$$$$$   /$$$$$$  /$$$$$$  /$$$$$$   /$$$$$$  /$$$$$$         /$$$$$$$  /$$$$$$       | $$  \\ $$/$$$$$$ | $$   /$$  /$$$$$$  /$$$$$$/$$$$   /$$$$$$  /$$$$$$$   /$$$$$$  /$$$$$$$\n"
    "\t | $$      |____  $$ /$$__  $$/$$__  $$/$$__  $$ /$$__  $$|____  $$       /$$__  $$ /$$__  $$      | $$$$$$$/$$__  $$| $$  /$$/ /$$__  $$| $$_  $$_  $$ /$$__  $$| $$__  $$ /$$__  $$ /$$_____/ \n"
    "\t | $$       /$$$$$$$| $$  \\__/ $$  \\__/ $$$$$$$$| $$  \\__/ /$$$$$$$      | $$  | $$| $$$$$$$$      | $$____/ $$  \\ $$| $$$$$$/ | $$$$$$$$| $$ \\ $$ \\ $$| $$  \\ $$| $$  \\ $$| $$$$$$$$|  $$$$$$  \n"
    "\t | $$    $$/$$__  $$| $$     | $$     | $$_____/| $$      /$$__  $$      | $$  | $$| $$_____/      | $$    | $$  | $$| $$_  $$ | $$_____/| $$ | $$ | $$| $$  | $$| $$  | $$| $$_____/ \\____  $$ \n"
    "\t |  $$$$$$/  $$$$$$$| $$     | $$     |  $$$$$$$| $$     |  $$$$$$$      |  $$$$$$$|  $$$$$$$      | $$    |  $$$$$$/| $$ \\  $$|  $$$$$$$| $$ | $$ | $$|  $$$$$$/| $$  | $$|  $$$$$$$ /$$$$$$$/ \n"
    "\t \\______/ \\_______/|__/     |__/      \\_______/|__/      \\_______/       \\_______/ \\_______/      |__/     \\______/ |__/  \\__/ \\_______/|__/ |__/ |__/ \\______/ |__/  |__/ \\_______/|_______/  \n"
    "                                                                                                                                                                                               \n"
    "                                                                                                                                                                                               \n"
    );
	printf("\n\n\t\t\t\t\t\t\tIngresa 'c' para continuar o '?' para saber acerca del juego.\t Para ver opciones disponibles ingresa 'h'.\n\n\n");
}

bool mostrar_acerca_del_juego(void *contexto) {
	printf("En este juego, tú eliges la dificultad y tu pokémon favorito para competir contra otro seleccionado al azar.\n");
	printf("Deberás construir una pista de obstáculos, calculando el tiempo según los atributos de tu pokémon y el tipo de obstaculos.\n");
	printf("El objetivo es que ambos pokémones terminen la carrera lo más sincronizados posible para conseguir el mejor puntaje final.\n");


 	return true;
}

void mostrar_info_pokemon(const struct pokemon_info *pokemon)
{
	printf("\n\nNombre: %s\n", pokemon->nombre);
	printf("Atributos del pokemon.\n");
	printf("- Fuerza: %i puntos.\n", pokemon->fuerza);
	printf("- Destreza: %i puntos.\n", pokemon->destreza);
	printf("- Inteligencia: %i puntos.\n", pokemon->inteligencia);
}

void mostrar_interfaz_dificultad(void *contexto)
{
	limpiar_pantalla_completa();
	printf("Ingresa el número de la dificultad que quieras.\n");
	printf("1. Fácil\n");
	printf("2. Normal\n");
	printf("3. Difícil\n");
	printf("4. Imposible\n");
}

void liberar_memoria_split(char **nombres, int cantidad)
{
	for (int i = 0; i < cantidad; i++) {
		free(nombres[i]);
	}
	free(nombres);
}

int cantidad_ocultos_segun_dificultad(int dificultad,
				      unsigned int cantidad_obstaculos)
{
	int cantidad_ocultos = 0;
	switch (dificultad - 1) {
	case FACIL:
		cantidad_ocultos = 1;
		break;
	case NORMAL:
		cantidad_ocultos = 3;
		break;
	case DIFICIL:
		cantidad_ocultos = 6;
		break;
	case IMPOSIBLE:
		cantidad_ocultos = (int)cantidad_obstaculos - 2;
		break;
	default:
		break;
	}

	return cantidad_ocultos;
}

void mostrar_obstaculos_oponente(TP *tp, enum TP_JUGADOR jugador, int dificultad,
                                 unsigned int cantidad_obstaculos)
{
    char *obstaculos_jugador = tp_obstaculos_pista(tp, jugador);
    if (!obstaculos_jugador) {
        printf("No hay obstáculos disponibles.\n");
        return;
    }

    int cantidad_ocultos = cantidad_ocultos_segun_dificultad(dificultad, cantidad_obstaculos);

    for (unsigned int i = 0; i < cantidad_obstaculos; i++) {
        if (i < cantidad_ocultos) {
            printf("??? ");
        } else {
            printf("%c\t", obstaculos_jugador[i]);
        }
    }
    printf("\n");

    free(obstaculos_jugador);
}

void mostrar_interfaz_info_oponente(void *contexto)
{
	estado_t *estado = (estado_t *)contexto;
	const struct pokemon_info *pokemon_oponente =
		asignar_pokemon_jugador(estado->juego, JUGADOR_2);
	unsigned int cantidad_oponentes = construir_pista_oponente(
		estado->juego, JUGADOR_2, estado->dificultad);

	if (!pokemon_oponente) {
		printf("No se pudo asignar un Pokémon al oponente.\n");
	}

	limpiar_pantalla_completa();
	printf("El pokemon al que te enfrentáras es:");

	mostrar_info_pokemon(pokemon_oponente);

	mostrar_obstaculos_oponente(estado->juego, JUGADOR_2, estado->dificultad,
				   cantidad_oponentes);
}

void mostrar_pokemones_disponibles(TP *tp)
{
	char separador = ',';
	char *nombres_pokemon = tp_nombres_disponibles(tp);
	if (!nombres_pokemon) {
		printf("No hay pokemones disponibles.\n");
		return;
	}

	char **pokemones_disponibles = split(nombres_pokemon, separador);
	if (!pokemones_disponibles) {
		printf("Error al dividir los nombres de los pokemones.\n");
		free(nombres_pokemon);
		return;
	}

	int cantidad_pokemones = tp_cantidad_pokemon(tp) - 1;

	for (int i = 0; i < cantidad_pokemones; i++) {
		printf("-> %s \n", pokemones_disponibles[i]);
	}

	liberar_memoria_split(pokemones_disponibles, cantidad_pokemones);
	free(nombres_pokemon);
}

void mostrar_info_pokemon_usuario(void *contexto)
{
	estado_t *estado = (estado_t *)contexto;
	limpiar_pantalla_completa();
	printf("Es momento de elegir tu pokemon....\n");
	printf("Los pokemones disponibles son:\n\n");
	mostrar_pokemones_disponibles(estado->juego);
	printf("Escribe el nombre del pokemon que quieras: \n\n");
}

void mostrar_info_armado_pista(void *contexto)
{
	limpiar_pantalla_completa();
	estado_t *estado = (estado_t *)contexto;
	printf("Es hora de armar tu propia pista de obstaculos.\n");
	printf("Oprima m para modifica la lista\n");

	char *pista_usuario = tp_obstaculos_pista(estado->juego, JUGADOR_1);
	if (pista_usuario) {
		printf("%s", pista_usuario);
	} else {
		printf("La pista no tiene obstaculo. Inserta alguno.");
	}

	free(pista_usuario);
}

char *mostrar_pista_usuario(char *pista_usuario, estado_t *estado)
{
	if (!pista_usuario) {
		pista_usuario = tp_obstaculos_pista(estado->juego, JUGADOR_1);
		if (pista_usuario) {
			printf("%s\n", pista_usuario);
		}
	}

	return pista_usuario;
}

void mostrar_resultado_carrera(void *contexto)
{
    estado_t *estado = (estado_t *)contexto;
    unsigned tiempo_usuario = tp_calcular_tiempo_pista(estado->juego, JUGADOR_1);
    unsigned tiempo_oponente = tp_calcular_tiempo_pista(estado->juego, JUGADOR_2);
    limpiar_pantalla_completa();

    int tiempo_usuario_signed = (int)tiempo_usuario;
    int tiempo_oponente_signed = (int)tiempo_oponente;
    int diferencia_tiempos = abs(tiempo_usuario_signed - tiempo_oponente_signed);
    int suma_tiempos = tiempo_usuario_signed + tiempo_oponente_signed;

    unsigned puntaje_final = 100 - (unsigned)((100 * diferencia_tiempos) / suma_tiempos);

    printf("Puntaje final: %u\n", puntaje_final);

    if (puntaje_final >= 98) {
        printf("Felicitaciones, los pokemones acercaron sus tiempos.");
    } else if (puntaje_final == 100) {
        printf("Felicitaciones, los pokemones igualaron sus tiempos.");
    } else {
        printf("Los pokemones no acercaron sus tiempos. Si quieres modificar tu pista y volver a correr oprime 'a'. Tienes %i intentos.",
               estado->intentos);
    }
}
void mostrar_interfaz_fin_del_juego(void *contexto)
{
	estado_t *estado = (estado_t *)contexto;
	destruir_fase(estado->fase_actual);
	limpiar_pantalla_completa();
	if (estado->intentos == 0) {
		printf("Te has quedado sin intentos.\n");
	}
	printf("Se termino el juego papurri, listo!!!\n\n");
	estado->continuar = false;
}