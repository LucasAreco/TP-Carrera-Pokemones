#include "src/tp.h"
#include "src/menu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "src/lista.h"
#include "src/gestor_fases.h"

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


void imprimir_interfaz_seleccion_pokemones(void* contexto) {

    printf("===================================================================");

    printf("\n\n\n\nFase gestion de pokemon\n\n\n\n");

    printf("El pokemon contra el que competiras sera pepito. LPMMMMMMMMMMMM");
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
                estado.fase_actual->contenido(NULL);
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



