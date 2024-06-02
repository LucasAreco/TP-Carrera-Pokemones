#include "split.h"
#include <stdlib.h>
#include <string.h>

#define FINAL_STR '\0'

/*
 * Pre: substring y string deben ser punteros válidos y no nulos.
 *      pos_separador y pos_anterior_separador deben ser posiciones válidas en string.
 * Post: Completa substring con los caracteres entre pos_anterior_separador y pos_separador en string.
 */
void completar_substring(char* substring, const char* string, size_t pos_separador, size_t pos_anterior_separador) 
{
    size_t indice_substring = 0;
    for (size_t i = pos_anterior_separador; i < pos_separador; i++) {
        substring[indice_substring] = string[i];
        indice_substring++;
    }
    substring[indice_substring] = FINAL_STR;
}

/*
 * Pre: string debe ser un puntero válido y no nulo.
 *      posicion_actual y pos_anterior_separador deben ser posiciones válidas en string.
 * Post: reserva memoria para un substring que contiene los caracteres entre pos_anterior_separador 
 *       y posicion_actual en string. En caso de error al reservar memoria, devuelve NULL.
 */
char* inicializar_substring(const char* string, size_t posicion_actual, size_t pos_anterior_separador) 
{
    size_t longitud_substring = posicion_actual - pos_anterior_separador;
    char* substring = malloc(sizeof(char) * (longitud_substring + 1)); 
    if (substring == NULL) {
        return NULL;
    }
    completar_substring(substring, string, posicion_actual, pos_anterior_separador);
    return substring;
}

/*
 * Pre: string debe ser un puntero válido y no nulo. Separador debe ser un carácter válido.
 *      longitud_string debe ser el tamaño válido de string.
 * Post: rcorre string y devuelve la cantidad de veces que aparece separador, 
 *       contabilizando el final de string como un separador adicional.
 */
size_t obtener_cantidad_separadores(const char* string, char separador, size_t longitud_string) {
    size_t cantidad_separadores = 0;
    for (size_t i = 0; i < longitud_string; i++) {
        if (string[i] == separador) {
            cantidad_separadores++;
        }
    }
    return cantidad_separadores + 1;
}

/*
 * Pre: recibe string_a_separar y string que deben ser punteros válidos y no nulos.
 *      Separador debe ser un carácter válido.
 *    
 * Post: divide la cadena string en subcadenas utilizando separador como delimitador.
 *       Asigna dinámicamente cada subcadena a string_a_separar y finaliza con NULL.
 *       Si falla la asignación de memoria, libera los recursos previamente asignados.
 */
void dividir_cadena(char** string_a_separar, const char* string, char separador, size_t longitud_string) {
    size_t pos_anterior_separador = 0;
    size_t cant_substrings = 0;

    for (size_t i = 0; i <= longitud_string; i++) {
        if (string[i] == separador || string[i] == FINAL_STR) {
            char* substring = inicializar_substring(string, i, pos_anterior_separador);
            if (substring == NULL) {
                for (size_t j = 0; j < cant_substrings; j++) {
                    free(string_a_separar[j]);
                }
                free(string_a_separar);
            }
            pos_anterior_separador = i + 1;
            string_a_separar[cant_substrings] = substring;
            cant_substrings++;
        }
    }
    string_a_separar[cant_substrings] = NULL;
}


char** split(const char* string, char separador) {
    if (string == NULL) {
        return NULL;
    }
    size_t longitud_string = strlen(string);
    size_t cantidad_separadores = obtener_cantidad_separadores(string, separador, longitud_string);

    char** string_separado = malloc(sizeof(char*) * (cantidad_separadores + 1)); 

    if (string_separado == NULL) {
        return NULL;
    }

    dividir_cadena(string_separado, string, separador, longitud_string);
    return string_separado;
}










































