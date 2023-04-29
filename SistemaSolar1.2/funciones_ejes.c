#include <glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include <stdio.h>
#include "funciones_ejes.h"

//angulos de rotacion de la camara
float alfa = 0;
float beta = 0;
//zoom de la camara
float zoom = 1;
//flags para tener las orbitas, movimiento, luces y ejes activados
int flag_orbitas = 1, flag_movimiento = 1, flag_luces = 1, flag_ejes = 1;

void camaraAlejada(float aspecto) {
	//configuracion de la matriz de proyeccion
	glMatrixMode(GL_PROJECTION);
	//cargamos la identidad
	glLoadIdentity();
	//modificamos la vision
	gluPerspective(45, aspecto, 1, DISTANCIA * 9);
	//enfocamos la camara
	gluLookAt(((float)DISTANCIA * 1.5 * zoom * (float)sin(alfa) * cos(beta)), ((float)DISTANCIA * 1.5 * zoom * (float)sin(beta)),((float)DISTANCIA * 1.5 * zoom * cos(alfa) * cos(beta)), 0, 0, 0, 0, cos(beta), 0);
}

void telescopio(float distancia_visor, float angulo_visor, float distancia_visto, float angulo_visto, unsigned int satelite, float aspecto){
	//matriz de proyeccion
	glMatrixMode(GL_PROJECTION);
	//cargamos la identidad
	glLoadIdentity();
	//perspectiva con aspecto
	gluPerspective(60, aspecto, 1, DISTANCIA * 6);
	//comprobamos si es un satelite o no
	switch (satelite){
		case 0://Tierra a cualquier planeta
			gluLookAt(distancia_visor * cos(angulo_visor * PI / 180.0), 0, -distancia_visor * sin(angulo_visor * PI / 180.0), distancia_visto * cos(angulo_visto * PI / 180.0), 0, -1 * distancia_visto * sin(angulo_visto * PI / 180.0), 0, 1, 0);
			break;
		case 1://Tierra a sus satélites
			gluLookAt(distancia_visor * cos(angulo_visor * PI / 180.0), 0, -distancia_visor * sin(angulo_visor * PI / 180.0), distancia_visto * cos((angulo_visto + angulo_visor) * PI / 180.0) + distancia_visor * cos(angulo_visor * PI / 180.0), 0, -distancia_visto * sin((angulo_visto + angulo_visor) * PI / 180.0) - distancia_visor * sin(angulo_visor * PI / 180.0), 0, 1, 0);
			break;
		case 2://Satélites a la Tierra
			gluLookAt(distancia_visor * cos((angulo_visor + angulo_visto) * PI / 180.0) + distancia_visto * cos(angulo_visto * PI / 180.0), 0, -distancia_visor * sin((angulo_visor + angulo_visto) * PI / 180.0) - distancia_visto * sin(angulo_visto * PI / 180.0), distancia_visto * cos(angulo_visto * PI / 180.0), 0, - distancia_visto * sin(angulo_visto * PI / 180.0), 0, 1, 0);
			break;
		default:
			break;
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
		case 'm':
			flag_movimiento = !flag_movimiento;
			break;
		case 'l':
			flag_luces = !flag_luces;
			break;
		case 'e':
			flag_ejes = !flag_ejes;
			break;
		//zoom para la camara alejada 
		case '+':
			if(zoom > 0.21)zoom -= 0.01;
			break;
		case '-':
			if(zoom < 1.9) zoom += 0.01;
			break;

	}
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
		alfa -= INCREMENTO;
		break;
	case GLUT_KEY_LEFT:
		alfa += INCREMENTO;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

//getter de las orbitas (dibujar las orbitas o no)
int get_orbitas() {
	return flag_orbitas;
}

//getter del movimiento (mover o parar los planetas9
int get_movimiento() {
	return flag_movimiento;
}

//getter luces (iluminar la escena o no)
int get_luces() {
	return flag_luces;
}

//getter de ejes (mostrar los ejes o no)
int get_ejes() {
	return flag_ejes;
}