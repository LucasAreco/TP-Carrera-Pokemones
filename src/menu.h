#ifndef __MENU_H__
#define __MENU_H__

#include <stdbool.h>

typedef struct menu menu_t;

typedef enum {MENU_OK, COMANDO_INEXISTENTE, MENU_ERROR} MENU_RESULTADO; 


menu_t* menu_crear();

void menu_agregar_comando(menu_t* menu, const char* comando, char* descripcion, bool (*f)(void*));

bool menu_contiene_comando(menu_t* menu, const char* comando);

MENU_RESULTADO menu_ejecutar_comando(menu_t *menu, const char *comando, void *contexto);

void menu_con_cada_comando(menu_t* menu, bool (*f)(const char *clave, const char *descripcion, bool (*funcion)(void*), void *aux), void *aux);

void menu_destruir(menu_t *m);




#endif //__MENU_H__