#include <glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include <stdio.h>
#include "funciones_ejes.h"

float alpha = 0;
float beta = 0;
int flag_orbitas = 1, flag_movimiento = 1;

void moverCamara() {
	//configuracion de la matriz de proyeccion
	glMatrixMode(GL_PROJECTION);
	//cargamos la identidad
	glLoadIdentity();
	//modificamos la vision
	glOrtho(-DISTANCIA, DISTANCIA, -DISTANCIA, DISTANCIA, 1, DISTANCIA*2);
	//enfocamos la camara
	gluLookAt(((float)DISTANCIA * (float)sin(alpha) * cos(beta)), ((float)DISTANCIA * (float)sin(beta)),((float)DISTANCIA * cos(alpha) * cos(beta)), 0, 0, 0, 0, 1, 0);
}

//funcion que reconoce las teclas
void teclas(unsigned char c, int x, int y) {
	switch (c) {
		//activacion o desactivacion de las orbitas
		case 'o':
			flag_orbitas = !flag_orbitas;
			break;
		//activacion del movimiento
		case 'p':
			flag_movimiento = !flag_movimiento;
			break;
	}
}

//getter de las orbitas
int get_orbitas() {
	return flag_orbitas;
}

//getter del movimiento
int get_movimiento() {
	return flag_movimiento;
}

//funcion de teclas especiales para mover la camara
void teclasEspeciales(int cursor, int x, int y){
	switch (cursor)	{
		//Traslaciones:
	case GLUT_KEY_F1:
		break;
	case GLUT_KEY_F2:
		break;
	case GLUT_KEY_F3:
		break;
	case GLUT_KEY_F4:
		break;
	case GLUT_KEY_F5:
		break;
	case GLUT_KEY_F6:
		break;
		//Giros:
	case GLUT_KEY_UP:
		beta -= INCREMENTO;
		break;
	case GLUT_KEY_DOWN:
		beta += INCREMENTO;
		break;
	case GLUT_KEY_RIGHT:
		alpha -= INCREMENTO;
		break;
	case GLUT_KEY_LEFT:
		alpha += INCREMENTO;
		break;
	default:
		break;
	}
	if (alpha >= PI * 2.0 || alpha <= 0) {
		alpha += -PI * 2.0;
	}
	if (beta >= PI * 2.0 || beta <= 0) beta += -PI * 2.0; //hay que repasarlo para evitar el salto
	glutPostRedisplay();
}