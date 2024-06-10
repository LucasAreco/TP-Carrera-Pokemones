#include "src/tp.h"
#include "src/menu.h"
#include <stdio.h>
#include <stdlib.h>




char* leer_entrada_usuario() 
{
	char* entrada[200];
	char* leido = fgets(entrada, sizeof(entrada), stdin);

	return leido;
}






int main(int argc, char const *argv[])
{
	//Se recomienda pasar el archivo de texto con los pokemon como argumento al ejecutar el programa

	//crear el juego
	TP*	tp = tp_crear(argv[1]);
	//mostrar menues
	//correr carrera
	//etc
 
          


	return 0;
}
