#include <stdio.h>

#define DISTANCIA 1500
#define INCREMENTO .01
#define PI 3.141592
//tiempo cada el que se actualizan los giros
#define TIEMPO 60

//estructura para los cuerpos celestes
typedef struct {
	float distancia_sol;
	float velocidad_traslacion;
	float angulo_traslacion;
	float velocidad_rotacion;
	float angulo_rotacion;
	int tamano;
	float color_R;
	float color_G;
	float color_B;
}planeta;

void moverCamara();

void teclasEspeciales(int cursor, int x, int y);