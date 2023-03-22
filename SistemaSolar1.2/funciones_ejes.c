#include <glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include <stdio.h>
#include "funciones_ejes.h"

float alpha = 0;
float beta = 0;
//flags para tener las orbitas activadas y el movimiento activado
int flag_orbitas = 1, flag_movimiento = 1;

void moverCamara(float aspecto) {
	//configuracion de la matriz de proyeccion
	glMatrixMode(GL_PROJECTION);
	//cargamos la identidad
	glLoadIdentity();
	//modificamos la vision
	gluPerspective(45, aspecto, 1, DISTANCIA * 3);
	//enfocamos la camara
	gluLookAt(((float)DISTANCIA * 1.5 * (float)sin(alpha) * cos(beta)), ((float)DISTANCIA * 1.5 * (float)sin(beta)),((float)DISTANCIA * 1.5 * cos(alpha) * cos(beta)), 0, 0, 0, 0, cos(beta), 0);
}

void telescopio(float distancia_visor, float angulo_visor, float distancia_visto, float angulo_visto, int satelite, float aspecto){
	//matriz de proyeccion
	glMatrixMode(GL_PROJECTION);
	//cargamos la identidad
	glLoadIdentity();
	//perspectiva con aspecto
	gluPerspective(45, aspecto, 1, DISTANCIA * 2);
	//comprobamos si es un satelite o no
	if (satelite) {
		gluLookAt(distancia_visor * cos(angulo_visor * PI / 180), 0, -distancia_visor * sin(angulo_visor * PI / 180), distancia_visto * cos((angulo_visto + angulo_visor) * PI / 180) + distancia_visor * cos(angulo_visor * PI / 180), 0, -distancia_visto * sin((angulo_visto + angulo_visor) * PI / 180) - distancia_visor * sin(angulo_visor * PI / 180), 0, 1, 0);
	} else {
		gluLookAt(distancia_visor * cos(angulo_visor * PI / 180), 0, -distancia_visor * sin(angulo_visor * PI / 180), distancia_visto * cos(angulo_visto * PI / 180), 0, -1 * distancia_visto * sin(angulo_visto * PI / 180), 0, 1, 0);
	}
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
		beta += INCREMENTO;
		break;
	case GLUT_KEY_DOWN:
		beta -= INCREMENTO;
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
	glutPostRedisplay();
}