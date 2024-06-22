#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>
#include "src/tp.h"
#include "src/fases_juego.h"

void normalizar_entrada(char *cadena)
{
	if (!cadena || *cadena == '\0') {
		return;
	}

	*cadena = (char)toupper((unsigned char)*cadena);

	for (char *c = cadena + 1; *c != '\0'; ++c) {
		*c = (char)tolower((unsigned char)*c);
	}
}

void tomar_linea(char *linea, int tamanio)
{
	if (fgets(linea, tamanio, stdin)) {
		int longitud = (int)strlen(linea);
		if (longitud > 0 && linea[longitud - 1] == '\n') {
			linea[longitud - 1] = '\0';
		}
	}
}

void ejecutar_juego(estado_t *estado)
{
	while (estado->continuar) {
		printf("\n> ");
		char linea[200];
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
