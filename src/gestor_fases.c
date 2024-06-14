#include "gestor_fases.h"
#include <stdlib.h>

gestor_fases_t* inicializar_gestor_fases() {
    gestor_fases_t* gestor = calloc(1, sizeof(gestor_fases_t));
    if (!gestor) {
        return NULL;
    }
    gestor->fases = lista_crear();
    if (!gestor->fases) {
        free(gestor);
        return NULL;
    }


    return gestor;
}

fase_t* inicializar_fase() {
    fase_t* nueva_fase = calloc(1, sizeof(fase_t));
    if (!nueva_fase) {
        return NULL;
    }
    nueva_fase->menu = menu_crear();
    if (!nueva_fase->menu) {
        free(nueva_fase);
        return NULL;
    }
    nueva_fase->valida = true;
    return nueva_fase;
}

void agregar_fase(gestor_fases_t* gestor, fase_t* fase, void (*funcion_contenido)(void*)) {
    fase->contenido = funcion_contenido;
    lista_insertar(gestor->fases, fase);
}

fase_t* obtener_fase(gestor_fases_t* gestor, size_t numero) {
    return lista_elemento_en_posicion(gestor->fases, numero);
}

void avanzar_fase(gestor_fases_t* gestor) {
    if (gestor->numero_fase_actual != lista_tamanio(gestor->fases) - 1) {
        gestor->numero_fase_actual++;
    }
}



void destruir_fase(void* fase) {
    fase_t* fase_actual = (fase_t*)fase;
    if (fase_actual) {
        menu_destruir(fase_actual->menu);
        free(fase_actual);
    }
}

void destruir_gestor(gestor_fases_t* gestor) {
    lista_destruir_todo(gestor->fases, destruir_fase);
    free(gestor);
}
