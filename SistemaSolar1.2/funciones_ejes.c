#include <glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include <stdio.h>
#include "funciones_ejes.h"


float alpha = PI/2;
float beta = PI/2;

void moverCamara() {
	//configuracion de la matriz de proyeccion
	glMatrixMode(GL_PROJECTION);
	//cargamos la identidad
	glLoadIdentity();
	//modificamos la vision (el cubo 1x1x1 a DISTACIAxDISTANCIAxDISTANCIA)
	glOrtho(-DISTANCIA, DISTANCIA, -DISTANCIA, DISTANCIA, -DISTANCIA, DISTANCIA);
	gluLookAt(((float)DISTANCIA * (float)sin(alpha) * cos(beta)), ((float)DISTANCIA * (float)sin(beta)),
		((float)DISTANCIA * cos(alpha) * cos(beta)), 0, 0, 0, 0, 1, 0);
}

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
		printf("pulsado");
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

	if (alpha >= PI * 2.0 && alpha <= 0) alpha = 0;
	if (beta >= PI * 2.0 && beta <= 0) beta = 0; //hay que repasarlo para evitar el salto
	glutPostRedisplay();
}