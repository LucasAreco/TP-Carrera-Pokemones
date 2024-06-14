#include "src/tp.h"
#include "src/gestor_fases.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "src/split.h"

// Definiciones de estructuras y funciones
typedef enum {FACIL, NORMAL, DIFICIL, IMPOSIBLE} DIFICULTAD;

typedef struct estado_juego {
    TP* juego;
    bool continuar;
    int dificultad;
    fase_t* fase_actual;
    char* entrada;
} estado_t;

bool salir(void* e) {
    estado_t* estado = (estado_t*)e;
    estado->continuar = false;
    return true;
}

char* copiar_linea(const char* linea) {
    size_t largo = strlen(linea) + 1;
    char* copia = malloc(largo);
    if (!copia) {
        return NULL;
    }
    strcpy(copia, linea);
    return copia;
}

bool inicializar_dificultad(void* e) {
    estado_t* estado = (estado_t*)e;
    int dificultad_elegida = atoi(estado->entrada);
    if (dificultad_elegida >= 1 && dificultad_elegida <= 4) {
        estado->dificultad = dificultad_elegida;
        printf("Dificultad es: %i\n", estado->dificultad);
        return true;
    }
    return false;
}

void imprimir_interfaz_dificultad(void* contexto) {
    printf("Ingresa el número de la dificultad que quieras.\n");
    printf("1. Fácil\n");
    printf("2. Normal\n");
    printf("3. Difícil\n");
    printf("4. Imposible\n");
}



const struct pokemon_info* obtener_pokemon_aleatorio(estado_t* estado) {
    char* pokemones_disponibles = tp_nombres_disponibles(estado->juego);
    if (!pokemones_disponibles) {
        return NULL;
    }

    char separador = ',';
    char** nombres_disponibles = split(pokemones_disponibles, separador);

    int count = tp_cantidad_pokemon(estado->juego);

    if (count <= 0) {
        // Liberar la memoria si no hay pokemones disponibles
        free(nombres_disponibles);
        free(pokemones_disponibles);
        return NULL;
    }

    srand((unsigned int) time(NULL));

    int indice_random = rand() % count;
    char* nombre_random = nombres_disponibles[indice_random];

    const struct pokemon_info* pokemon_oponente = tp_buscar_pokemon(estado->juego, nombre_random);

    // Liberar la memoria asignada por split
    for (int i = 0; i < count; i++) {
        free(nombres_disponibles[i]);
    }
    free(nombres_disponibles);
    free(pokemones_disponibles);

    return pokemon_oponente;
}

const struct pokemon_info* asignar_pokemon_oponente(estado_t* estado) {
    const struct pokemon_info* pokemon_oponente = obtener_pokemon_aleatorio(estado);
    if (!pokemon_oponente) {
        return NULL;
    }

    if (tp_seleccionar_pokemon(estado->juego, JUGADOR_2, pokemon_oponente->nombre)) {
        return pokemon_oponente;
    }

    return NULL;
}

void imprimir_interfaz_seleccion_pokemones(void* contexto) {
    estado_t* estado = (estado_t*)contexto;
    const struct pokemon_info* pokemon_oponente = asignar_pokemon_oponente(estado);
    if (pokemon_oponente) {
        printf("Tu oponente es: %s\n", pokemon_oponente->nombre);
        printf("Atributos del pokemon oponente.\n\n");
        printf("- Fuerza: %i puntos.\n", pokemon_oponente->fuerza);
        printf("- Destreza: %i puntos.\n", pokemon_oponente->destreza);
        printf("- Inteligencia: %i puntos.\n", pokemon_oponente->inteligencia);
    } else {
        printf("No se pudo asignar un Pokémon al oponente.\n");
    }
}



























fase_t* seleccionar_dificultad() {
    fase_t* fase_dificultad = inicializar_fase();
    if (!fase_dificultad) {
        return NULL;
    }
    menu_t* menu_dificultades = fase_dificultad->menu;
    menu_agregar_comando(menu_dificultades, "1", "Elegir dificultad 'Fácil'.", inicializar_dificultad);
    menu_agregar_comando(menu_dificultades, "2", "Elegir dificultad 'Normal'.", inicializar_dificultad);
    menu_agregar_comando(menu_dificultades, "3", "Elegir dificultad 'Difícil'.", inicializar_dificultad);
    menu_agregar_comando(menu_dificultades, "4", "Elegir dificultad 'Imposible'.", inicializar_dificultad);
    menu_agregar_comando(menu_dificultades, "q", "Salir del juego.", salir);


    return fase_dificultad;
}

fase_t* gestion_pokemones() {
    fase_t* fase_pokemones = inicializar_fase();
    if (!fase_pokemones) {
        return NULL;
    }
    menu_t* menu_pokemones = fase_pokemones->menu;
    menu_agregar_comando(menu_pokemones, "q", "Salir del juego.", salir);

    return fase_pokemones;
}





int main(int argc, char const *argv[]) {
    TP* tp = tp_crear(argv[1]);

    printf("Bienvenido, ingrese comando a continuación o escriba ayuda\n\n");


    srand((unsigned int) time(NULL));
    // Seleccionar un nombre aleatorio

    gestor_fases_t* gestor = inicializar_gestor_fases();
    if (!gestor) {
        printf("Error al inicializar el gestor de fases.\n");
        tp_destruir(tp);
        return 1;
    }

    agregar_fase(gestor, seleccionar_dificultad(), imprimir_interfaz_dificultad);
    agregar_fase(gestor, gestion_pokemones(), imprimir_interfaz_seleccion_pokemones);

    estado_t estado = {
        .juego = tp,
        .continuar = true,
        .entrada = NULL,
        .dificultad = -1,
        .fase_actual = obtener_fase(gestor, gestor->numero_fase_actual)
    };

    estado.fase_actual->contenido(NULL);

    while (estado.continuar) {
        printf("\n> ");
        char linea[200];
        if (fgets(linea, sizeof(linea), stdin)) {
            linea[strlen(linea) - 1] = '\0';
            free(estado.entrada);
            estado.entrada = copiar_linea(linea);
            MENU_RESULTADO resultado = menu_ejecutar_comando(estado.fase_actual->menu, linea, &estado);
            if (resultado != MENU_OK) {
                printf("Algo anda mal...\n");
            } else if (estado.continuar) {
                avanzar_fase(gestor);
                estado.fase_actual =  obtener_fase(gestor, gestor->numero_fase_actual);
                estado.fase_actual->contenido(&estado);
            } else {
                estado.continuar = false;
            }
        }
    }

    destruir_gestor(gestor);
    free(estado.entrada);
    tp_destruir(tp);

    return 0;
}



