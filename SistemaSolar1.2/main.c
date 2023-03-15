#include <windows.h>
#include <glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <math.h>
#include "funciones_ejes.h"

const int ANCHO_VENTANA = 700;
const int ALTO_VENTANA = 700;

extern int arrayEsfera();
//lista esfera
unsigned int esfera;
int camara = 1; 
//datos: distancia al sol, velocidad de traslacion, angulo de traslacion, velocidad de rotacion, angulo de rotacion, tamaño, colorRGB
planeta sol = {0, 0, 0, 0, 0, 100, 255, 171, 25};
planeta mercurio = {180, 10, 0, 0, 0, 25, 148, 108, 68};
planeta venus = {280, 8, 150, 5.2, 0, 35, 227, 172, 61};
planeta tierra = {450, 7, 0, 9, 0, 40};
planeta marte = {650, 6, 120, 10, 0, 35, 232, 71, 46 };
planeta jupiter = {800, 5, 0, 15, 0, 60, 227, 211, 163};
planeta saturno = {1050, 4.4, 0, 9, 0, 50, 169, 151, 204};
planeta urano = {1250, 3.7, 26, 6, 0, 45, 50, 156, 120};
planeta neptuno = {1400, 2.5, 160, 11, 0, 45, 76, 137, 212};

//satelites de la tierra
planeta luna = {80, 10, 0, 15, 0, 10, 255, 255, 255};
planeta ISS = {120, 15, 0, 12, 0, 9, 200, 100, 97};


//funcion que actualiza el angulo de rotacion y traslacion de un planeta
void suma_angulo(planeta* p) {
	//actualizamos la velocidad de traslacion
	p->angulo_traslacion += p->velocidad_traslacion;
	if (p->angulo_traslacion > 360)p->angulo_traslacion += -360;
	//actualizamos la velocidad de rotacion
	p->angulo_rotacion += p->velocidad_rotacion;
	if (p->angulo_rotacion > 360)p->angulo_rotacion += -360;
}

void dibujar_orbitas() {
	float x, z;
	float radios[] = {mercurio.distancia_sol,venus.distancia_sol, tierra.distancia_sol, marte.distancia_sol, jupiter.distancia_sol,saturno.distancia_sol,urano.distancia_sol,neptuno.distancia_sol};
	for (int j = 0; j < 8;j++) {
	//dibujamos en un lazo
		glBegin(GL_LINE_LOOP);
		glColor3f(1.0f, 1.0f, 1.0f);
		for (int i = 0; i < 100; i++) {
			//componente x
			x = radios[j] * cos(2.0 * PI * i / 100.0);
			//componene z
			z = radios[j] * sin(2.0 * PI * i / 100.0);
			//la componente y=0
			glVertex3f(x, 0.0, z);
		}
		glEnd();
	}
	
}

//funcion que dibuja la orbita de un planeta
void dibuja_orbita(float radio) {
	float x, z;
	//dibujamos en un lazo
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 100; i++) {
		//componente x
		x = radio * cos(2.0 * PI * i / 100.0);
		//componene z
		z = radio * sin(2.0 * PI * i / 100.0);
		glColor3f(1.0f, 1.0f, 1.0f);
		//la componente y=0
		glVertex3f(x, 0.0, z);
	}
	glEnd();
}

//funcion de dibujo de los ejes
void dibuja_ejes() {
	//eje x
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3d(0.0f, 0.0f, 0.0f);
	glVertex3d(2.0f, 0.0f, 0.0f);
	glEnd();
	//eje y
	glBegin(GL_LINES);
	glColor3f(.0f, 1.0f, .0f);
	glVertex3d(0.0f, 0.0f, 0.0f);
	glVertex3d(.0f, 2.f, 0.0f);
	glEnd();
	//eje z
	glBegin(GL_LINES);
	glColor3f(.0f, .0f, 1.0f);
	glVertex3d(0.0f, 0.0f, 0.0f);
	glVertex3d(.0f, 0.0f, 2.f);
	glEnd();
	//vector (1,1,1)
	glBegin(GL_LINES);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3d(0.0f, 0.0f, 0.0f);
	glVertex3d(2.0f, 2.0f,2.f);
	glEnd();
}

//funcion de dibujo de los planetas
void dibuja_planeta(planeta p) {
	glPushMatrix();
		//rotamos alrededor del sol
		glRotatef(p.angulo_traslacion, 0, 1, 0);
		//trasladamos el planteta a su posicion
		glTranslatef(p.distancia_sol, 0, 0);
		glPushMatrix();
			//rotamos sobre si mismo
			glRotatef(p.angulo_rotacion, 0, 1, 0);
			//escalamnos
			glScalef(p.tamano, p.tamano, p.tamano);
			dibuja_ejes();
			//colores
			glColor3f(p.color_R/255, p.color_G/255, p.color_B/255);
			//dibujamos el planeta
			glCallList(esfera);
		glPopMatrix();
	glPopMatrix();
}

//funcion de dibujo de la tiera con sus satelites
void dibuja_tierra() {
	//dibujo de la tierra
	glPushMatrix();
		//rotamos alrededor del sol
		glRotatef(tierra.angulo_traslacion, 0, 1, 0);
		//trasladamos el planteta a su posicion
		glTranslatef(tierra.distancia_sol, 0, 0);
		glPushMatrix();
			//rotamos sobre si mismo
			glRotatef(tierra.angulo_rotacion, 0, 1, 0);
			//escalamnos
			glScalef(tierra.tamano, tierra.tamano, tierra.tamano);
			dibuja_ejes();
			glColor3f(0.0f, .0f, 1.0f);
			//dibujamos el planeta
			glCallList(esfera);
		glPopMatrix();

		//dibujo de la luna
		glPushMatrix();
			//rotamos alrededor de la tierra
			glRotatef(luna.angulo_traslacion, 0, 1, 0);
			//trasladamos el la luna a su posicion
			glTranslatef(luna.distancia_sol, 0, 0);
			glPushMatrix();
				//rotamos alrededor de si misma
				glRotatef(luna.angulo_rotacion, 0, 1, 0);
				dibuja_ejes();
				//escalamos a su posicion
				glScalef(luna.tamano, luna.tamano, luna.tamano);
				glColor3f(luna.color_R / 255, luna.color_G / 255, luna.color_B / 255);
				glCallList(esfera);
			glPopMatrix();
		glPopMatrix();
		if(get_orbitas())dibuja_orbita(luna.distancia_sol);

		//dibujo de la ISS
		glPushMatrix();
			//rotamos alrededor de la tierra
			glRotatef(-ISS.angulo_traslacion, 0, 1, 0);
			//trasladamos el la luna a su posicion
			glTranslatef(ISS.distancia_sol, 0, 0);
			glPushMatrix();
				//rotamos alrededor de si misma
				glRotatef(ISS.angulo_rotacion, 0, 1, 0);
				dibuja_ejes();
				//escalamos a su posicion
				glScalef(ISS.tamano, ISS.tamano, ISS.tamano);
				glColor3f(ISS.color_R / 255, ISS.color_G / 255, ISS.color_B / 255);
				glCallList(esfera);
			glPopMatrix();
		glPopMatrix();
		//dibujamos las orbitas
		if (get_orbitas()) dibuja_orbita(ISS.distancia_sol);
	glPopMatrix();
}

//funcion de dibujo de saturno y sus anillos
void dibuja_saturno() {
	//dibujo de saturno con sus anillos
	glPushMatrix();
	//rotamos alrededor del sol
	glRotatef(saturno.angulo_traslacion, 0.0f, 1.0f, 0.0f);
	//trasladamos el planteta a su posicion
	glTranslatef(saturno.distancia_sol, 0.0f, 0.0f);
	glPushMatrix();
	//rotamos sobre si mismo
	glRotatef(saturno.angulo_rotacion, 0.0f, 1.0f, 0.0f);
	//escalamnos
	glScalef(saturno.tamano, saturno.tamano, saturno.tamano);
	dibuja_ejes();
	//colores
	glColor3f(saturno.color_R / 255, saturno.color_G / 255, saturno.color_B / 255);
	//dibujamos el planeta
	glCallList(esfera);
	glPopMatrix();
	//blanco
	glColor3f(1.0f, 1.0f, 1.0f);
	//rotamos el anillo 
	glRotatef(90, 1.0f, 0.0f, 0.0f);
	//usamos un toro de anillo
	glutWireTorus(8.0f, 75.0f, 20, 20);
	glPopMatrix();
	//dibujamos la orbita
	//dibuja_orbita(saturno.distancia_sol);
}

void usar_menu(int opcion) {
	switch (opcion) {
		case 1:
			camara = 1;
		break;
		case 2:
			camara = 2;
		break;
		case 3:
			camara = 3;
		break;
		case 4:
			camara = 4;
		break;
		case 5:
			camara = 5;
		break;
		case 6:
			camara = 6;
		break;
		case 7:
			camara = 7;
		break;
		case 8:
			camara = 8;
		break;
		case 9:
			camara = 9;
		break;
		case 10:
			camara = 10;
		break;
		case 11:
			camara = 11;
		break;
	}
	glutPostRedisplay();
}

void menu() {
	int menu;
	menu = glutCreateMenu(usar_menu);
	glutAddMenuEntry("Alejada", 1);
	glutAddMenuEntry("Sol", 2);
	glutAddMenuEntry("Mercurio", 3);
	glutAddMenuEntry("Venus", 4);
	glutAddMenuEntry("Luna", 5);
	glutAddMenuEntry("ISS", 6);
	glutAddMenuEntry("Marte", 7);
	glutAddMenuEntry("Jupiter", 8);
	glutAddMenuEntry("Saturno", 9);
	glutAddMenuEntry("Urano", 10);
	glutAddMenuEntry("Nepuno", 11);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}
// Función de display
void Display(void) {
	//switch de la camara
	switch (camara) {
	case 1: 
		moverCamara();
		break;
	case 2:
		camara = 2;
		telescopio(tierra.distancia_sol, tierra.angulo_traslacion, mercurio.distancia_sol, mercurio.angulo_traslacion);
		break;
	case 3:
		camara = 3;
		break;
	case 4:
		camara = 4;
		break;
	case 5:
		camara = 5;
		break;
	case 6:
		camara = 6;
		break;
	case 7:
		camara = 7;
		break;
	case 8:
		camara = 8;
		break;
	case 9:
		camara = 9;
		break;
	case 10:
		camara = 10;
		break;
	case 11:
		camara = 11;
		break;
	}
	// Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Se activa la matriz del modelador
	glMatrixMode(GL_MODELVIEW);
	// Inicializamos la matriz del modelo a la identidad
	glLoadIdentity();
	//dibujo de los planetas y ejes
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPushMatrix();
	glScalef(100, 100, 100);
	dibuja_ejes();
	glPopMatrix();
	//dibujamos el sol
	dibuja_planeta(sol);
	//dibujamos mercurio
	dibuja_planeta(mercurio);
	//dibujamos venus
	dibuja_planeta(venus);
	dibuja_tierra();
	dibuja_planeta(marte);
	dibuja_planeta(jupiter);
	dibuja_saturno();
	//dibujamos urano
	dibuja_planeta(urano);
	//dibujamos neptuno
	dibuja_planeta(neptuno);
	//si el flag esta a 1 dibujamos las orbitas
	if (get_orbitas() == 1) {
		dibujar_orbitas();
	}
	// Se limpian los buffers
	glutSwapBuffers();
	glFlush();
}

// Funcion que actualiza la posicion de los electrones modificando su angulo de giro
void Idle(void) {
	glutPostRedisplay();
}

//funcion de movimiento de los planetas
void movimiento() {
	//movemos si esta el flag activado
	if (get_movimiento()) {
		suma_angulo(&sol);
		suma_angulo(&mercurio);
		suma_angulo(&venus);
		suma_angulo(&tierra);
		suma_angulo(&luna);
		suma_angulo(&ISS);
		suma_angulo(&marte);
		suma_angulo(&jupiter);
		suma_angulo(&saturno);
		suma_angulo(&urano);
		suma_angulo(&neptuno);
	}
	//redibujamos
	glutPostRedisplay();
	//reejecutamos cada TIEMPO ms
	glutTimerFunc(TIEMPO, movimiento, 10);
}

void openGlInit() {
	//Incializaciones varias
	glClearDepth(1.0f); //Valor z-buffer
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // valor limpieza buffer color
	glEnable(GL_DEPTH_TEST); // z-buffer
	glEnable(GL_CULL_FACE); //ocultacion caras back
	glEnable(GL_NORMALIZE);
	glCullFace(GL_BACK);
	//creacion de la lista de la esfera
	esfera = glGenLists(1);
	glNewList(esfera, GL_COMPILE);
	arrayEsfera();
	glEndList();
}



int main(int argc, char** argv) {
	glutInit(&argc, argv);
	//Posicion de la ventana
	glutInitWindowPosition(100, 100);
	//Tamano de la ventana
	glutInitWindowSize(ANCHO_VENTANA, ALTO_VENTANA);
	//Tipo de color y buffer
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	//Crear la ventana
	glutCreateWindow("Sistema solar en OpenGL 1.2");

	//Funcion de dibujo
	glutDisplayFunc(Display);
	//Funcion de actualizacion
	glutIdleFunc(Idle);
	
	openGlInit();
	movimiento();
	glutKeyboardFunc(teclas);
	glutSpecialFunc(teclasEspeciales);
	moverCamara();
	//Lazo principal
	glutMainLoop();
	return 0;
}
