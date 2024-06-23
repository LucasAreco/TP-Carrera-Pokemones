#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>
#include "src/tp.h"
#include "src/fases_juego.h"

#define SEPARADOR_COMA ','
#define SEPARADOR_PUNTO '.'
#define CARACTER_NULO '\0'

#define LONGITUD_LINEA 200

/**
 * Normaliza una cadena de texto convirtiendo su primer carácter a mayúscula y los siguientes a minúscula.
 * Si la cadena es nula o el primer carácter es el carácter nulo, la función no realiza ninguna acción.
 */
void normalizar_entrada(char *cadena)
{
    if (!cadena || *cadena == CARACTER_NULO) {
        return;
    }

    *cadena = (char)toupper((unsigned char)*cadena);

    for (char *c = cadena + 1; *c != CARACTER_NULO; c++) {
        *c = (char)tolower((unsigned char)*c);
    }
}

/**
 * Lee una línea de texto desde la entrada estándar (stdin) y la almacena en el buffer proporcionado.
 * Elimina el carácter de nueva línea ('\n') al final de la línea leída, si está presente.
 * 
 */
void tomar_linea(char *linea, int tamanio)
{
    if (fgets(linea, tamanio, stdin)) {
        int longitud = (int)strlen(linea);
        if (longitud > 0 && linea[longitud - 1] == '\n') {
            linea[longitud - 1] = CARACTER_NULO;
        }
    }
}

/**
 * Función principal para ejecutar el juego. Itera continuamente hasta que el estado indique detenerse.
 * Lee comandos del usuario, los normaliza y los ejecuta usando el menú asociado a la fase actual del juego.
 * Muestra mensajes de error si el comando no existe o si hay problemas con la ejecución del juego.
  
 */
void ejecutar_juego(estado_t *estado)
{
    while (estado->continuar) {
        printf("\n> ");
        char linea[LONGITUD_LINEA];
        tomar_linea(linea, sizeof(linea));
        normalizar_entrada(linea);
        estado->fase_actual->entrada = linea;
        MENU_RESULTADO resultado = menu_ejecutar_comando(
            estado->fase_actual->menu, linea, estado);
        if (resultado == COMANDO_INEXISTENTE) {
            printf("El comando ingresado no existe. Intenta de nuevo.\n");
        } else if (resultado == MENU_ERROR) {
            printf("UPS... hay problemas con el juego.\n");
        }
    }
}


int main(int argc, char const *argv[])
{
	TP *tp = tp_crear(argv[1]);
	if (!tp) {
		printf("No pasaste un archivo válido en formato txt.");
		return 0;
	}


	srand((unsigned int)time(NULL));

	estado_t estado = {
		.juego = tp,
		.continuar = true,
		.entrada = NULL,
		.dificultad = -1,
		.fase_actual = inicio_juego(),
		.intentos = 0,
	};

	estado.fase_actual->contenido(NULL);

	ejecutar_juego(&estado);

	tp_destruir(tp);

	return 0;
}
