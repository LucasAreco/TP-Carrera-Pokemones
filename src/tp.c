#include "tp.h"
#include "abb.h"
#include "lista.h"
#include "split.h"

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>


#define MODO_LECTURA "r"

#define SIMBOLO_OBSTACULO_FUERZA "F"
#define SIMBOLO_OBSTACULO_DESTREZA "D"
#define SIMBOLO_OBSTACULO_INTELIGENCIA "I"



typedef struct jugador {
	enum TP_JUGADOR tipo_jugador;
	const struct pokemon_info* pokemon_elegido;
	lista_t* obstaculos;
} jugador_t;

struct tp {
	abb_t* pokemones;
	jugador_t* jugadores[2];
	int cantidad_pokemones;
};


typedef struct cadena {
	char* palabras;
	char separador;
	bool hay_conversion;
} cadena_t;



bool imprimir_recorrido(void *elemento, void *nulo) {
    struct pokemon_info *pokemon = (struct pokemon_info *)elemento;
    printf(" -> %s, %i, %i, %i\n", pokemon->nombre, pokemon->fuerza, pokemon->destreza, pokemon->inteligencia);    
    return true; // Devolvemos true para continuar el recorrido
}





bool imprimir_obstaculo(void *elemento, void *contexto) {
    enum TP_OBSTACULO *obstaculo = (enum TP_OBSTACULO *)elemento;
    printf("%d - ", *obstaculo);
    return true; 
}

void convertir_primera_mayuscula(char* cadena) {
    if (!cadena|| *cadena == '\0') {
        return;  
    }

    if (*cadena >= 'a' && *cadena <= 'z') {
        *cadena -= ('a' - 'A');
    }

    for (char* c = cadena + 1; *c != '\0'; ++c) {
        if (*c >= 'A' && *c <= 'Z') {
            *c += ('a' - 'A');
        }
    }
}

int comparar_pokemon(void *a, void *b) {
    const struct pokemon_info *p1 = (const struct pokemon_info *)a;
    const struct pokemon_info *p2 = (const struct pokemon_info *)b;
    return strcmp(p1->nombre, p2->nombre);
}


char* copiar_nombre(const char* nombre) {
	size_t longitud = strlen(nombre) + 1; 
	char *copia_nombre = malloc(longitud);  
	if (copia_nombre == NULL) {
		return NULL;           
	}
	memcpy(copia_nombre, nombre, longitud); //podria hacerlo de otra forma quiza.

	return copia_nombre;
}




void inicializar_jugadores(TP* tp) 
{
	jugador_t* usuario = malloc(sizeof(jugador_t));
	if (!usuario) {
		return;
	}

	jugador_t* computadora = malloc(sizeof(jugador_t));
	if (!computadora) {
		free (usuario);
		return;
	}

	usuario->tipo_jugador = JUGADOR_1;
	computadora->tipo_jugador = JUGADOR_2;

	usuario->pokemon_elegido = NULL;
	computadora->pokemon_elegido = NULL;

	usuario->obstaculos = lista_crear();
	if (!usuario->obstaculos) {
		printf("La lista de obstáculos del usuario está vacía.\n");
		return;
	}

	computadora->obstaculos = lista_crear();
	if (!computadora->obstaculos) {
		printf("La lista de obstáculos del usuario está vacía.\n");
		return;
	}

	tp->jugadores[JUGADOR_1] = usuario;
	tp->jugadores[JUGADOR_2] = computadora;
}







TP *tp_crear(const char *nombre_archivo)
{
	char registro[100];

	if (nombre_archivo == NULL) {
        return NULL;
    }

	TP* tp = malloc(sizeof(TP));
	if (!tp) {
		return NULL;
	}

	FILE *archivo = fopen(nombre_archivo, MODO_LECTURA);
	if (!archivo) {
		free(tp);
		return NULL;
	}

	tp->pokemones = abb_crear(comparar_pokemon);
	if(!tp->pokemones) {
		fclose(archivo);
		free(tp);
		return NULL;
	}

	tp->cantidad_pokemones = 0;
	inicializar_jugadores(tp);

	while (fgets(registro, sizeof(registro), archivo) != NULL) {
		char separador = ',';
		char** partes_registro =  split(registro, separador);
		
		struct pokemon_info* datos_pokemon = malloc(sizeof(struct pokemon_info));
		if (!datos_pokemon) {
			fclose(archivo); // Cerrar el archivo antes de liberar la memoria
			abb_destruir(tp->pokemones); // Liberar el árbol creado hasta ahora
			free(tp);
			return NULL;
		}


		datos_pokemon->nombre = copiar_nombre(partes_registro[0]);
        if (!datos_pokemon->nombre) {
            fclose(archivo);
            abb_destruir(tp->pokemones);
            free(datos_pokemon);
            free(tp);
            return NULL;
        }


		datos_pokemon->fuerza = atoi(partes_registro[1]);
		datos_pokemon->destreza = atoi(partes_registro[2]);
		datos_pokemon->inteligencia = atoi(partes_registro[3]);

		convertir_primera_mayuscula(datos_pokemon->nombre);

		const struct pokemon_info* nuevo_pokemon = datos_pokemon;
		
		abb_insertar(tp->pokemones, (void*)nuevo_pokemon);

    	for (int i = 0; partes_registro[i] != NULL; i++) {
        	free(partes_registro[i]);
    	}
		free(partes_registro);

		tp->cantidad_pokemones++;
	}

	fclose(archivo);
	return tp;
} 



int tp_cantidad_pokemon(TP *tp)
{	
	if (!tp || !tp->pokemones) {
		return 0;
	}

	return tp->cantidad_pokemones;
}


const struct pokemon_info *tp_buscar_pokemon(TP *tp, const char *nombre) {
    if (!tp || !nombre) {
        return NULL;
    }

    char* nombre_buscado = copiar_nombre(nombre);
    if (!nombre_buscado) {
        return NULL;  
    }

	convertir_primera_mayuscula(nombre_buscado);


    struct pokemon_info pokemon_buscado = {
        .nombre = nombre_buscado,
    };

    struct pokemon_info *pokemon_encontrado = abb_buscar(tp->pokemones, &pokemon_buscado);

    free(nombre_buscado);


    return pokemon_encontrado;
}


const char* conversor_obstaculo_a_cadena(void* elemento) {
	enum TP_OBSTACULO* obstaculo = (enum TP_OBSTACULO *)elemento;
    switch (*obstaculo) {
        case OBSTACULO_FUERZA: return SIMBOLO_OBSTACULO_FUERZA;
        case OBSTACULO_DESTREZA: return SIMBOLO_OBSTACULO_DESTREZA;
        case OBSTACULO_INTELIGENCIA: return SIMBOLO_OBSTACULO_INTELIGENCIA;
        default: return NULL;
	}
}


bool llenar_cadena(void *elemento, void *contexto) {
    cadena_t *cadena = (cadena_t *)contexto;
	 const char *elemento_str;

	if (cadena->hay_conversion) {
		elemento_str = conversor_obstaculo_a_cadena(elemento);
        if (!elemento_str) {
            return false; 
        }
	}
	else {
		struct pokemon_info *pokemon = (struct pokemon_info *)elemento;
		elemento_str = pokemon->nombre;
	}


	size_t nuevo_tamanio = strlen(cadena->palabras) + strlen(elemento_str) + 2; 

    char *nueva_cadena = realloc(cadena->palabras, nuevo_tamanio);
    if (!nueva_cadena) {
        return false; 
    }

    if (strlen(nueva_cadena) != 0) {
        strncat(nueva_cadena, &cadena->separador, 1);
    }
    strcat(nueva_cadena, elemento_str);

    cadena->palabras = nueva_cadena;

    return true;
}



char *tp_nombres_disponibles(TP *tp) {
    if (!tp) {
        return NULL;
    }

    cadena_t cadena;
    cadena.palabras = malloc(1);
    if (!cadena.palabras) {
        return NULL;
    }
    cadena.palabras[0] = '\0';
    cadena.separador = ',';
	cadena.hay_conversion = false;

    if (abb_con_cada_elemento(tp->pokemones, INORDEN, llenar_cadena, &cadena) == false) {
        free(cadena.palabras); 
        return NULL;
    }


    return cadena.palabras;
}



bool tp_seleccionar_pokemon(TP *tp, enum TP_JUGADOR jugador, const char *nombre) {
    if (!tp || !nombre) {
        return false;
    }

    const struct pokemon_info* pokemon_seleccionado = tp_buscar_pokemon(tp, nombre);
    if (!pokemon_seleccionado) {
        return false;
    }

    jugador_t* jugador_actual = tp->jugadores[jugador];
    jugador_t* otro_jugador;

    if (jugador == JUGADOR_1) {
        otro_jugador = tp->jugadores[JUGADOR_2];
    } else {
        otro_jugador = tp->jugadores[JUGADOR_1];
    }

    if (otro_jugador->pokemon_elegido && strcmp(otro_jugador->pokemon_elegido->nombre, pokemon_seleccionado->nombre) == 0) {
        return false; 
    }

    jugador_actual->pokemon_elegido = pokemon_seleccionado;

    return true;
}

const struct pokemon_info *tp_pokemon_seleccionado(TP *tp, enum TP_JUGADOR jugador)
{
	if (!tp || (jugador != JUGADOR_1 && jugador != JUGADOR_2)) {
        return NULL;
    }

	return tp->jugadores[jugador]->pokemon_elegido;
}




unsigned tp_agregar_obstaculo(TP *tp, enum TP_JUGADOR jugador, enum TP_OBSTACULO obstaculo, unsigned posicion) {
    if (!tp) {
        return 0;
    }

    jugador_t *jugador_actual = tp->jugadores[jugador];
	
    lista_t *pista_actual = jugador_actual->obstaculos;
	if (!pista_actual) {
		return 0;
	}

    enum TP_OBSTACULO *obstaculo_elegido = malloc(sizeof(enum TP_OBSTACULO));
    if (!obstaculo_elegido) {
        return 0;
    }

	*obstaculo_elegido = obstaculo;

	if (!lista_insertar_en_posicion(pista_actual, obstaculo_elegido, posicion)) {
		free(obstaculo_elegido);
		return 0; 
	}

    return (unsigned)lista_tamanio(pista_actual);
}




unsigned tp_quitar_obstaculo(TP *tp, enum TP_JUGADOR jugador, unsigned posicion) {
    if (!tp) {
        return 0;
    }

    jugador_t *jugador_actual = tp->jugadores[jugador];
    lista_t *pista_actual = jugador_actual->obstaculos;
    if (!pista_actual) {
        return 0;
    }

    enum TP_OBSTACULO *obstaculo_a_quitar = lista_elemento_en_posicion(pista_actual, posicion);
    if (!obstaculo_a_quitar) {
        return 0;
    }

    if (!lista_quitar_de_posicion(pista_actual, posicion)) {
        return 0; 
    }

	free(obstaculo_a_quitar);

    return (unsigned)lista_tamanio(pista_actual);
}





char *tp_obstaculos_pista(TP *tp, enum TP_JUGADOR jugador)
{	
	if (!tp) {
        return 0;
    }

    jugador_t *jugador_actual = tp->jugadores[jugador];
	
    lista_t *pista_actual = jugador_actual->obstaculos;
	if (!pista_actual || lista_tamanio(pista_actual) == 0) {
		return NULL;
	}


    cadena_t cadena;
	cadena.palabras = malloc(1);
    if (!cadena.palabras) {
        return NULL;
    }

	cadena.palabras[0] = '\0';
    cadena.separador = ' ';
	cadena.hay_conversion = true;




	if (lista_con_cada_elemento(pista_actual, llenar_cadena, &cadena) == 0) {
		free(cadena.palabras);
		return NULL;
	}

	return cadena.palabras;
}


void liberar_nombre_pokemon(void *dato) {
    struct pokemon_info *pokemon = (struct pokemon_info *)dato;
    free(pokemon->nombre); 
	free(pokemon);
}

void liberar_obstaculos_pista(void* dato) {
    enum TP_OBSTACULO* obstaculo = (enum TP_OBSTACULO *)dato;
    free(obstaculo);
}

void tp_limpiar_pista(TP *tp, enum TP_JUGADOR jugador)
{
	if (!tp) {
		return;
	}

	if (tp->jugadores[jugador]->obstaculos != NULL) {
        lista_destruir_todo(tp->jugadores[jugador]->obstaculos, liberar_obstaculos_pista);
		tp->jugadores[jugador]->obstaculos = NULL;
    }
}




unsigned tiempo_por_obstaculo(TP* tp, enum TP_JUGADOR jugador)
{

	if (!tp) {
		return 0;
	}

	jugador_t* jugador_actual = tp->jugadores[jugador];
	if (!jugador_actual || !jugador_actual->pokemon_elegido) {
		return 0;
	}
	
	lista_t* pista_actual = jugador_actual->obstaculos;
	if (!pista_actual) {
		return 0;
	}

	unsigned tiempo_obstaculo = 0;

	



}








unsigned tp_calcular_tiempo_pista(TP *tp, enum TP_JUGADOR jugador)
{
	return 0;
}

char *tp_tiempo_por_obstaculo(TP *tp, enum TP_JUGADOR jugador)
{
	return NULL;
}






void tp_destruir(TP *tp) {
    if (!tp) {
        return;
    }

    if (tp->pokemones != NULL) {
        abb_destruir_todo(tp->pokemones, liberar_nombre_pokemon);
    }

    if (tp->jugadores[JUGADOR_1]->obstaculos != NULL) {
        lista_destruir_todo(tp->jugadores[JUGADOR_1]->obstaculos, liberar_obstaculos_pista);
    }

    if (tp->jugadores[JUGADOR_2]->obstaculos != NULL) {
        lista_destruir_todo(tp->jugadores[JUGADOR_2]->obstaculos, liberar_obstaculos_pista);
    }

    free(tp->jugadores[JUGADOR_1]);
    free(tp->jugadores[JUGADOR_2]);

    free(tp);
}
