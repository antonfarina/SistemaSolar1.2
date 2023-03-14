#include <stdio.h>

//tiempo cada el que se actualizan los giros
#define TIEMPO 60

//estructura para los cuerpos celestes
typedef struct {
	float distancia_sol;
	float velocidad_translacion;
	float angulo_translacion;
	float velocidad_rotacion;
	float angulo_rotacion;
	int tamano;
	float color_R;
	float color_G;
	float color_B;
}planeta;