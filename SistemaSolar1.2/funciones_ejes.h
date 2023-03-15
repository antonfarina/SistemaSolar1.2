#include <stdio.h>

#define DISTANCIA 1500
#define INCREMENTO .02
//numero pi
#define PI 3.141592
//tiempo cada el que se actualizan los giros
#define TIEMPO 60

//estructura para los cuerpos celestes
typedef struct {
	//distancia al sol (o al padre)
	float distancia_sol;
	//velocidad de translacion alrededor del padre
	float velocidad_traslacion;
	//angulo de translacion alrededor del padre
	float angulo_traslacion;
	//velocidad de rotacion y angulo de rotacion sobre si mismo
	float velocidad_rotacion;
	float angulo_rotacion;
	//tamaño del planeta
	int tamano;
	//colores
	float color_R;
	float color_G;
	float color_B;
}planeta;

int get_orbitas();
int get_movimiento();
void moverCamara();
void telescopio(float distancia_visor, float angulo_visor, float distancia_visto, float angulo_visto);
void teclas(unsigned char c, int x, int y);
void teclasEspeciales(int cursor, int x, int y);

