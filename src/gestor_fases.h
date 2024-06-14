#ifndef GESTOR_FASES_H
#define GESTOR_FASES_H

#include "lista.h"
#include "menu.h"

typedef struct fase {
    menu_t* menu;
    bool valida;
    void (*contenido)(void*);
} fase_t;

typedef struct gestor_fases {
    lista_t* fases;
    size_t numero_fase_actual;
} gestor_fases_t;

// Funciones públicas
gestor_fases_t* inicializar_gestor_fases();
fase_t* inicializar_fase();
void agregar_fase(gestor_fases_t* gestor, fase_t* fase, void (*funcion_contenido)(void*));
fase_t* obtener_fase(gestor_fases_t* gestor, size_t numero);

void avanzar_fase(gestor_fases_t* gestor);
void destruir_gestor(gestor_fases_t* gestor);

#endif // GESTOR_FASES_H
