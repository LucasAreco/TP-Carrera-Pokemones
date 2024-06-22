#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "menu.h"
#include "hash.h"
#include "tp.h"

struct menu {
	hash_t *comandos;
};

typedef struct comando_info {
	char *descripcion;
	bool (*funcion)(void *);
} comando_info_t;

menu_t *menu_crear()
{
	menu_t *menu = malloc(sizeof(menu));
	if (!menu) {
		return NULL;
	}

	menu->comandos = hash_crear(3);
	if (!menu->comandos) {
		free(menu);
		return NULL;
	}

	return menu;
}

void menu_agregar_comando(menu_t *menu, const char *comando, char *descripcion,
			  bool (*f)(void *))
{
	if (!menu || !comando || !descripcion || !f) {
		return;
	}

	comando_info_t *info = malloc(sizeof(comando_info_t));
	if (!info) {
		return;
	}

	info->descripcion = descripcion;
	info->funcion = f;

	hash_t *comandos = hash_insertar(menu->comandos, comando, info, NULL);
	if (!comandos) {
		return;
	}
}

bool menu_contiene_comando(menu_t *menu, const char *comando)
{
	if (!menu || !hash_contiene(menu->comandos, comando)) {
		return false;
	}

	return true;
}

MENU_RESULTADO menu_ejecutar_comando(menu_t *menu, const char *comando,
				     void *contexto)
{
	comando_info_t *info_comando = hash_obtener(menu->comandos, comando);
	if (!info_comando) {
		return COMANDO_INEXISTENTE;
	}

	if (info_comando->funcion(contexto)) {
		return MENU_OK;
	}

	return MENU_ERROR;
}

struct datos_iterador {
	void *aux;
	bool (*f)(const char *clave, const char *descripcion,
		  bool (*funcion)(void *), void *aux);
};

bool funcion_para_iterador(const char *clave, void *valor, void *aux)
{
	comando_info_t *info = valor;
	struct datos_iterador *datos = aux;
	return datos->f(clave, info->descripcion, info->funcion, datos->aux);
}

void menu_con_cada_comando(menu_t *menu,
			   bool (*f)(const char *clave, const char *descripcion,
				     bool (*funcion)(void *), void *aux),
			   void *aux)
{
	if (!menu || !f) {
		return;
	}

	struct datos_iterador datos;
	datos.aux = aux;
	datos.f = f;

	hash_con_cada_clave(menu->comandos, funcion_para_iterador, &datos);
}

void destructor_info_comandos(void *comando_info)
{
	comando_info_t *info_comando = comando_info;
	free(info_comando);
}

void menu_destruir(menu_t *menu)
{
	hash_destruir_todo(menu->comandos, destructor_info_comandos);
	free(menu);
}
