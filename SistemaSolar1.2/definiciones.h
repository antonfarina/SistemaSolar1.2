#include <stdio.h>

#define TIEMPO 50

//estructura para los cuerpos celestes
typedef struct {
	float distancia_sol;
	float velocidad_translacion;
	float angulo_translacion;
	float velocidad_rotacion;
	float angulo_rotacion;
	int tamano;
}planeta;