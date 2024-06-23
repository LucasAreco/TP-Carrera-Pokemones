#ifndef __MENU_H__
#define __MENU_H__

#include <stdbool.h>

typedef struct menu menu_t;

typedef enum { MENU_OK, COMANDO_INEXISTENTE, MENU_ERROR } MENU_RESULTADO;


/**
 * Crea un nuevo menú.
 * Devuelve un puntero al menú creado o NULL en caso de error.
 */
menu_t *menu_crear();

/**
 * Agrega un nuevo comando al menú junto con su descripción y función asociada.
 * Debe recibir un menú válido, el nombre del comando, una descripción y la función que se le asigna al comando.
 */
void menu_agregar_comando(menu_t *menu, const char *comando, char *descripcion, bool (*f)(void *));

/**
 * Revisa si el menú posee el comando especificado.
 * Devuelve true si el menú contiene el comando, false en caso contrario.
 */
bool menu_contiene_comando(menu_t *menu, const char *comando);

/**
 * Ejecuta el comando especificado en el menú.
 * Devuelve MENU_OK si se ejecutó correctamente, COMANDO_INEXISTENTE si el comando no existe en el menú, o MENU_ERROR si ocurrió un error.
 */
MENU_RESULTADO menu_ejecutar_comando(menu_t *menu, const char *comando,void *contexto);

/**
 * Itera sobre cada comando del menú y ejecuta la función f.
 * Para cada comando se pasa su clave (nombre), descripción, función asociada y un contexto auxiliar.
 */
void menu_con_cada_comando(menu_t *menu, bool (*f)(const char *clave, const char *descripcion, bool (*funcion)(void *), void *aux), void *aux);

/**
 * Libera la memoria utilizada por el menú y todos sus comandos.
 */
void menu_destruir(menu_t *menu);




#endif //__MENU_H__