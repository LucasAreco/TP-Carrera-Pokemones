#include "src/tp.h"
#include "src/gestor_fases.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "src/split.h"

typedef enum {FACIL, NORMAL, DIFICIL, IMPOSIBLE} DIFICULTAD;

typedef struct estado_juego {
    TP* juego;
    bool continuar;
    int dificultad;
    fase_t* fase_actual;
    char* entrada;
    gestor_fases_t* gestor;
} estado_t;


void mostrar_info_pokemon(const struct pokemon_info* pokemon) {
    printf("Nombre: %s\n", pokemon->nombre);
    printf("Atributos del pokemon.\n");
    printf("- Fuerza: %i puntos.\n", pokemon->fuerza);
    printf("- Destreza: %i puntos.\n", pokemon->destreza);
    printf("- Inteligencia: %i puntos.\n", pokemon->inteligencia);
}


//FUNCIONES AUXILIARES 

// char* copiar_linea(const char* linea) {
//     size_t largo = strlen(linea) + 1;
//     char* copia = malloc(largo);
//     if (!copia) {
//         return NULL;
//     }
//     strcpy(copia, linea);
//     return copia;
// }

int seleccionar_numero_aleatorio(int max) {
    return rand() % max;
}

void liberar_memoria_split(char** nombres, int cantidad) {
    for (int i = 0; i < cantidad; i++) {
        free(nombres[i]);
    }
    free(nombres);
}




//FUNCIONES DE COMANDOS

bool salir(void* e) {
    estado_t* estado = (estado_t*)e;
    estado->continuar = false;
    estado->fase_actual->valida = false;
    return true;
}

bool continuar(void* e) {
    estado_t* estado = (estado_t*)e;
    estado->fase_actual->valida = true;
    estado->continuar = true;
    avanzar_fase(estado->gestor);
    estado->fase_actual = obtener_fase(estado->gestor, estado->gestor->numero_fase_actual);
    estado->fase_actual->contenido(estado);

    return true;
}


bool inicializar_dificultad(void* e) {
    estado_t* estado = (estado_t*)e;
    int dificultad_elegida = atoi(estado->entrada);
    if (dificultad_elegida >= 1 && dificultad_elegida <= 4) {
        estado->dificultad = dificultad_elegida;
        printf("Dificultad es: %i\n", estado->dificultad);
        avanzar_fase(estado->gestor);
        estado->fase_actual = obtener_fase(estado->gestor, estado->gestor->numero_fase_actual);
        estado->fase_actual->contenido(estado);
        return true;
    }
    return false;
}


void mostrar_pokemones_disponibles(TP* tp) {
    char separador = ',';
    char* nombres_pokemon = tp_nombres_disponibles(tp);
    if (!nombres_pokemon) {
        printf("No hay pokemones disponibles.\n");
        return;
    }

    char** pokemones_disponibles = split(nombres_pokemon, separador);
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


bool seleccion_pokemon_usuario(void* e) {
    estado_t* estado = (estado_t*)e;
    if(tp_seleccionar_pokemon(estado->juego, JUGADOR_1, estado->entrada)) {
        const struct pokemon_info* pokemon_usuario = tp_pokemon_seleccionado(estado->juego, JUGADOR_1);
        mostrar_info_pokemon(pokemon_usuario);
        printf("\nSi el pokemon correcto es correcto, oprime 'c', para continuar o ingresa otro nombre:\n");
    } 
    else {
        avanzar_fase(estado->gestor);
        estado->fase_actual = obtener_fase(estado->gestor, estado->gestor->numero_fase_actual);
        estado->fase_actual->contenido(estado);
    } 

    return true;

}

//FUNCIONES DE JUGABILIDAD / ARMADO Y REGLAS DEL JUEGO

int cantidad_ocultos_segun_dificultad(int dificultad, unsigned int cantidad_obstaculos) {
    int cantidad_ocultos = 0;
    switch (dificultad - 1) {
        case FACIL:
        cantidad_ocultos = 1;
        break;
        case NORMAL:
        cantidad_ocultos = 4;
        break;
        case DIFICIL:
        cantidad_ocultos = 6;
        break;
        case IMPOSIBLE:
        cantidad_ocultos = (int)cantidad_obstaculos - 1;
        break;
        default:
        break;
    }

    return cantidad_ocultos;
}

unsigned int construir_pista_oponente(TP* tp, enum TP_JUGADOR jugador, int dificultad) {
    int cantidad_obstaculos = (dificultad * 2) + 4;

    enum TP_OBSTACULO obstaculos[] = {
        OBSTACULO_FUERZA,
        OBSTACULO_DESTREZA,
        OBSTACULO_INTELIGENCIA
    };

    int tipo_obtaculo = -1;

    for (unsigned int i = 0; i < cantidad_obstaculos; i++) {
        tipo_obtaculo = seleccionar_numero_aleatorio(3);
        tp_agregar_obstaculo(tp, jugador, obstaculos[tipo_obtaculo], i);
    }
    
    return (unsigned int)cantidad_obstaculos; 
}

const struct pokemon_info* obtener_pokemon_aleatorio(TP* tp) {
    char* pokemones_disponibles = tp_nombres_disponibles(tp);
    if (!pokemones_disponibles) {
        return NULL;
    }

    char separador = ',';
    char** nombres_disponibles = split(pokemones_disponibles, separador);

    int cantidad_pokemones = tp_cantidad_pokemon(tp);

    srand((unsigned int) time(NULL));

    int indice_random = seleccionar_numero_aleatorio(cantidad_pokemones);
    char* nombre_random = nombres_disponibles[indice_random];

    const struct pokemon_info* pokemon_oponente = tp_buscar_pokemon(tp, nombre_random);

    liberar_memoria_split(nombres_disponibles, cantidad_pokemones);
    free(pokemones_disponibles);

    return pokemon_oponente;
}

const struct pokemon_info* asignar_pokemon_jugador(TP* tp, enum TP_JUGADOR jugador) {
    const struct pokemon_info* pokemon_oponente = obtener_pokemon_aleatorio(tp);
    if (!pokemon_oponente) {
        return NULL;
    }

    if (tp_seleccionar_pokemon(tp, jugador, pokemon_oponente->nombre)) {
        return pokemon_oponente;
    }

    return NULL;
}

//INTERFACES QUE SE IMPRIMEN



void mostrar_interfaz_dificultad(void* contexto) {
    printf("Ingresa el número de la dificultad que quieras.\n");
    printf("1. Fácil\n");
    printf("2. Normal\n");
    printf("3. Difícil\n");
    printf("4. Imposible\n");
}

void mostar_obstaculos_oponente(TP* tp, enum TP_JUGADOR jugador, int dificultad, unsigned int cantidad_obstaculos) {
    char separador = ',';
    char* obstaculos_jugador = tp_obstaculos_pista(tp, jugador);
    if (!obstaculos_jugador) {
        printf("No hay obstáculos disponibles.\n");
        return;
    }

    char** pista = split(obstaculos_jugador, separador);
    if (!pista) {
        printf("Error al dividir los obstáculos.\n");
        free(obstaculos_jugador);
        return;
    }

    int cantidad_ocultos = cantidad_ocultos_segun_dificultad(dificultad, cantidad_obstaculos);
    for (int i = 0; i < cantidad_obstaculos; i++) {
        if (i < cantidad_ocultos) {
            printf("%-3s - ", pista[i]);
        } else {
            printf("%-3s - ", "???");
        }
    }
    printf("\n");

    liberar_memoria_split(pista, (int)cantidad_obstaculos);
    free(obstaculos_jugador);
}



void mostrar_interfaz_info_oponente(void* contexto) {
    estado_t* estado = (estado_t*)contexto;
    const struct pokemon_info* pokemon_oponente = asignar_pokemon_jugador(estado->juego, JUGADOR_2);
    unsigned int cantidad_oponentes = construir_pista_oponente(estado->juego, JUGADOR_2, estado->dificultad);

    if (!pokemon_oponente) {
        printf("No se pudo asignar un Pokémon al oponente.\n");
    }

    printf("El pokemon al que te enfrentáras es:");

    mostrar_info_pokemon(pokemon_oponente);
   

    mostar_obstaculos_oponente(estado->juego, JUGADOR_2, estado->dificultad, cantidad_oponentes);    
}








void mostrar_info_pokemon_usuario(void* contexto) {
    estado_t* estado = (estado_t*)contexto;        
    printf("Es momento de elegir tu pokemon....\n");
    printf("Los pokemones disponibles son:\n\n");
    mostrar_pokemones_disponibles(estado->juego);
    printf("Escribe el nombre del pokemon que quieras: \n\n");

}


void mostrar_interfaz_fin_del_juego(void* contexto) {
    estado_t* estado = (estado_t*)contexto;
    printf("Se termino el juego papurri, listo!!!\n\n");
    estado->continuar = false;
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

fase_t* armado_de_atributos_oponente() {
    fase_t* fase_oponente = inicializar_fase();
    if (!fase_oponente) {
        return NULL;
    }
    menu_t* menu_oponente = fase_oponente->menu;
    menu_agregar_comando(menu_oponente, "q", "Salir del juego.", salir);
    menu_agregar_comando(menu_oponente, "c", "Continuar el juego", continuar);



    return fase_oponente;
}




fase_t* inicializar_pokemon_usuario(TP* tp) {
    fase_t* fase_usuario = inicializar_fase();
        if (!fase_usuario) {
        return NULL;
    }
    menu_t* menu_usuario = fase_usuario->menu;

    char separador = ',';
    char* nombres_pokemon = tp_nombres_disponibles(tp);
    char** pokemones_disponibles = split(nombres_pokemon, separador);
    for (int i = 0; i < tp_cantidad_pokemon(tp); i++) {
        menu_agregar_comando(menu_usuario, pokemones_disponibles[i], "Seleccion el pokemon con ese nombre.", seleccion_pokemon_usuario);
    }
    
    menu_agregar_comando(menu_usuario, "q", "Salir del juego.", salir);
    menu_agregar_comando(menu_usuario, "c", "Continuar el juego", continuar);



    liberar_memoria_split(pokemones_disponibles, tp_cantidad_pokemon(tp));
    free(nombres_pokemon);
    

    return fase_usuario;
}




void mostrar_info_armado_pista(void* contexto) {
    estado_t* estado = (estado_t*)contexto;
    printf("Es hora de armar tu propia pista de obstaculos.\n");
    printf("Oprima m para modifica la lista\n");

    
    char* pista_usuario = tp_obstaculos_pista(estado->juego, JUGADOR_1); 
    if (pista_usuario) {
        printf("%s", pista_usuario);
    } else {
        printf("La pista no tiene obstaculo. Inserta alguno.");
    }

    free(pista_usuario);
}



bool volver(void* e) {
    estado_t* estado = (estado_t*)e;
    estado->fase_actual->contenido(estado);
    return true;
}


enum TP_OBSTACULO conversor_obstaculo_numero(char letra_obstaculo) {
    switch (letra_obstaculo)
    {
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

char* mostrar_pista_usuario(char* pista_usuario, estado_t* estado) {
    if (!pista_usuario) {
        pista_usuario = tp_obstaculos_pista(estado->juego, JUGADOR_1); 
        if (pista_usuario) {
            printf("%s\n", pista_usuario);
        }
    }

    return pista_usuario;
}


bool volver_a_menu_armado_pista(char* linea, estado_t* estado, char* pista_usuario) {
    if (strcmp(linea, "b") == 0) {
        printf("Volviendo al menú de armado de pista.\n");
        free(pista_usuario);  // Liberar memoria de pista_usuario
        return volver(estado);
    }
    return false;  // El comando no fue "b"
}


bool datos_de_modificacion_pista_validos(char** datos_modificacion, bool hay_tipo_modificacion) {
    bool son_validos = false;
    if (datos_modificacion && datos_modificacion[0] && datos_modificacion[1] && datos_modificacion[2]) {
        son_validos = true;
    }

    if (hay_tipo_modificacion) {
        if ((strcmp(datos_modificacion[0], "+") == 0 || strcmp(datos_modificacion[0], "-") == 0) &&
            conversor_obstaculo_numero(datos_modificacion[1][0]) != -1 && atoi(datos_modificacion[2]) >= 0) {
            son_validos = true;
        } else {
            son_validos = false;
        }
    }

    return son_validos;
}

bool ejecutar_tipo_de_ejecucion(char** datos_modificacion, char** pista_usuario, estado_t* estado) {
    char* tipo_modificacion = datos_modificacion[0];
    enum TP_OBSTACULO tipo_obstaculo = conversor_obstaculo_numero(datos_modificacion[1][0]);
    int posicion = atoi(datos_modificacion[2]);

    if (strcmp(tipo_modificacion, "+") == 0) {
        tp_agregar_obstaculo(estado->juego, JUGADOR_1, tipo_obstaculo, (unsigned)posicion);
        free(*pista_usuario);  // Liberar memoria antigua
        *pista_usuario = NULL;  // Reiniciar pista_usuario para obtener nueva pista
    } else if (strcmp(tipo_modificacion, "-") == 0) {
        tp_quitar_obstaculo(estado->juego, JUGADOR_1, (unsigned)posicion);
        free(*pista_usuario);  // Liberar memoria antigua
        *pista_usuario = NULL;  // Reiniciar pista_usuario para obtener nueva pista
    } else {
        printf("Comando inválido. Formato esperado: + TIPO_OBSTACULO POSICION (ej. + F 3)\n");
    }

    return true;
}


bool agregar_o_quitar_obstaculo(char* linea, estado_t* estado, char** pista_usuario) {
    char separador = ' ';
    char** datos_modificacion = split(linea, separador);

    if (!datos_modificacion) {
        return false;
    }

    if (datos_de_modificacion_pista_validos(datos_modificacion, true)) {
        ejecutar_tipo_de_ejecucion(datos_modificacion, pista_usuario, estado);
    } else {
        printf("Comando inválido. Formato esperado: + TIPO_OBSTACULO POSICION (ej. + F 3)\n");
        liberar_memoria_split(datos_modificacion, 3);
        return false;
    }

    liberar_memoria_split(datos_modificacion, 3);
    return true;
}


bool modificar_pista(void* e) {
    estado_t* estado = (estado_t*)e;
    char* pista_usuario = NULL;

    while (true) {
        pista_usuario = mostrar_pista_usuario(pista_usuario, estado);
        
        printf("\n> ");
        char linea[200];
        if (fgets(linea, sizeof(linea), stdin)) {
            linea[strlen(linea) - 1] = '\0';
            if (volver_a_menu_armado_pista(linea, estado, pista_usuario)) {
                return true;
            }

            if (strlen(linea) == 5) {
                if (!agregar_o_quitar_obstaculo(linea, estado, &pista_usuario)) {
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







fase_t* armar_pista_del_usuario() {
    fase_t* fase_pista_usuario = inicializar_fase();
    if (!fase_pista_usuario) {
        return NULL;
    }

    menu_t* menu_pista_usuario = fase_pista_usuario->menu;




    menu_agregar_comando(menu_pista_usuario, "m", "Descripción de la entrada", modificar_pista);


    menu_agregar_comando(menu_pista_usuario, "q", "Salir del juego.", salir);
    menu_agregar_comando(menu_pista_usuario, "c", "Continuar el juego", continuar);
    menu_agregar_comando(menu_pista_usuario, "b", "Volver a mostrar menu", volver);



    return fase_pista_usuario;
}


fase_t* fin_del_juego() {
    fase_t* juego_finalizado = inicializar_fase();
    if (!juego_finalizado) {
        return NULL;
    }

    return juego_finalizado;

}


int main(int argc, char const *argv[]) {
    TP* tp = tp_crear(argv[1]);
    if (!tp) {
        printf("No pasaste un archivo válido");
        return 0;
    }


    printf("Bienvenido, ingrese comando a continuación o escriba ayuda\n\n");


    srand((unsigned int) time(NULL));

    gestor_fases_t* gestor = inicializar_gestor_fases();
    if (!gestor) {
        printf("Error al inicializar el gestor de fases.\n");
        tp_destruir(tp);
        return 1;
    }

    agregar_fase(gestor, seleccionar_dificultad(), mostrar_interfaz_dificultad);
    
    estado_t estado = {
        .juego = tp,
        .continuar = true,
        .entrada = NULL,
        .dificultad = -1,
        .fase_actual = obtener_fase(gestor, gestor->numero_fase_actual),
        .gestor = gestor
        
    };



    agregar_fase(gestor, armado_de_atributos_oponente(), mostrar_interfaz_info_oponente);
    agregar_fase(gestor, inicializar_pokemon_usuario(estado.juego), mostrar_info_pokemon_usuario);
    agregar_fase(gestor, armar_pista_del_usuario(), mostrar_info_armado_pista);
    agregar_fase(gestor, fin_del_juego(), mostrar_interfaz_fin_del_juego);


    estado.fase_actual->contenido(NULL);

    while (estado.continuar) {
        printf("\n> ");
        char linea[200];
        if (fgets(linea, sizeof(linea), stdin)) {
            linea[strlen(linea) - 1] = '\0';
            estado.entrada = linea;
            MENU_RESULTADO resultado = menu_ejecutar_comando(estado.fase_actual->menu, linea, &estado);
            if (resultado != MENU_OK) {
                printf("Algo anda mal...\n");
            }
        }
    }

    destruir_gestor(gestor);
    tp_destruir(tp);

    return 0;
}



