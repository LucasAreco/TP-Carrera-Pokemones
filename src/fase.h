#ifndef FASE_H
#define FASE_H

#include "lista.h"
#include "menu.h"

typedef struct fase {
    menu_t* menu;
    bool valida;
    char* entrada;
    void (*contenido)(void*);
} fase_t;


// Funciones públicas
fase_t* inicializar_fase();

void destruir_fase(fase_t* fase);
#endif // FASE_H
