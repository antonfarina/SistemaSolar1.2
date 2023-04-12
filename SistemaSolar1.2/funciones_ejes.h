#include <stdio.h>

#define DISTANCIA 1500
#define INCREMENTO .02
//numero pi
#define PI 3.141592
//tiempo cada el que se actualizan los giros
#define TIEMPO 70

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
	//textura
	int textura;
	//listarender
	int* lista_render;
}planeta;

//funcion que devuelve 1 si las orbitas estan activadas y 0 si no lo estan
int get_orbitas();
//funcion que devuelve 1 si el movimento esta activado y 0 si no lo esta
int get_movimiento();
//funcion que modifica la vision en funcion de las teclas
void moverCamara(float aspecto);
//funcion que enfoca la camara a un planeta desde el planeta tierra
void telescopio(float distancia_visor, float angulo_visor, float distancia_visto, float angulo_visto, int satelite, float aspecto);
//funcion de captura de las teclas
void teclas(unsigned char c, int x, int y);
//funcion de captura de las teclas especiales
void teclasEspeciales(int cursor, int x, int y);
