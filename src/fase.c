#include "fase.h"
#include <stdlib.h>

fase_t *inicializar_fase()
{
	fase_t *nueva_fase = calloc(1, sizeof(fase_t));
	if (!nueva_fase) {
		return NULL;
	}
	nueva_fase->menu = menu_crear();
	if (!nueva_fase->menu) {
		free(nueva_fase);
		return NULL;
	}
	return nueva_fase;
}

void destruir_fase(fase_t *fase)
{
	if (fase) {
		menu_destruir(fase->menu);
		free(fase);
	}
}
