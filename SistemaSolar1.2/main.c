#include <windows.h>

#include <glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <math.h>
#include "definiciones.h"
#include "funciones_ejes.h"
#include "esfera.h"

const int ANCHO_VENTANA = 700;
const int ALTO_VENTANA = 700;
//lista esfera
unsigned int esfera;

//datos: distancia al sol, velocidad de translacion, angulo de translacion, velocidad de rotacion, angulo de rotacion, tamaño, colorRGB
planeta sol = {0, 0, 0, 0, 0, 100, 255, 171, 25};
planeta mercurio = {180, 10, 0, 0, 0, 25, 148, 108, 68};
planeta venus = {280, 8, 150, 5.2, 0, 35, 227, 172, 61};
planeta tierra = {450, 7, 0, 9, 0, 40};
planeta marte = {650, 6, 120, 10, 0, 35, 232, 71, 46 };
planeta jupiter = {800, 5, 0, 0, 0, 60, 227, 211, 163};
planeta saturno = {1050, 4.4, 10, 9, 0, 50, 169, 151, 204};
planeta urano = {1250, 3.7, 26, 6, 0, 45, 50, 156, 120};
planeta neptuno = {1400, 2.5, 160, 11, 0, 45, 76, 137, 212};

//satelites de la tierra
planeta luna = { 80, 10, 0, 15, 0, 10, 255, 255, 255};
planeta ISS = { 120, 7, 0, 12, 0, 9, 200, 100, 97};

//funcion que actualiza el angulo de rotacion y translacion de un planeta
void suma_angulo(planeta* p) {
	p->angulo_translacion += p->velocidad_translacion;
	if (p->angulo_translacion > 360)p->angulo_translacion += -360;
	p->angulo_rotacion += p->velocidad_rotacion;
	if (p->angulo_rotacion > 360)p->angulo_rotacion += -360;
	glutPostRedisplay();
}

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
	glRotatef(p.angulo_translacion, 0, 1, 0);
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
	//dibujamos la orbita
	dibuja_orbita(p.distancia_sol);
}

// Función de display
void Display(void) {
	// Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//funcion de movimiento de la camara con las teclas del teclado
	moverCamara();
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
	dibuja_planeta(sol);
	dibuja_planeta(mercurio);
	dibuja_planeta(venus);

	//TIERRA, LUNA Y ISS
	glPushMatrix();
		//rotamos alrededor del sol
		glRotatef(tierra.angulo_translacion, 0, 1, 0);
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
		
		glPushMatrix();
			//rotamos alrededor de la tierra
			glRotatef(luna.angulo_translacion, 0, 1, 0);
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
		dibuja_orbita(luna.distancia_sol);
		glPushMatrix();
		//rotamos alrededor de la tierra
		glRotatef(-ISS.angulo_translacion, 0, 1, 0);
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
		dibuja_orbita(ISS.distancia_sol);
	glPopMatrix();
	dibuja_orbita(tierra.distancia_sol);

	dibuja_planeta(marte);
	dibuja_planeta(jupiter);
	/*dibuja_planeta(saturno);*/

	glPushMatrix();
	//rotamos alrededor del sol
	glRotatef(saturno.angulo_translacion, 0, 1, 0);
	//trasladamos el planteta a su posicion
	glTranslatef(saturno.distancia_sol, 0, 0);
	glPushMatrix();
	//rotamos sobre si mismo
	glRotatef(saturno.angulo_rotacion, 0, 1, 0);
	//escalamnos
	glScalef(saturno.tamano, saturno.tamano, saturno.tamano);
	dibuja_ejes();
	//colores
	glColor3f(saturno.color_R / 255, saturno.color_G / 255, saturno.color_B / 255);
	//dibujamos el planeta
	glCallList(esfera);
	glPopMatrix();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor3f(1.0f, 1.0f, 1.0f);
	//anillo de saturno
	glScalef(saturno.tamano*1.5, saturno.tamano * 0.1, saturno.tamano*1.5);
	glCallList(esfera);
	glPopMatrix();
	//dibujamos la orbita
	dibuja_orbita(saturno.distancia_sol);

	dibuja_planeta(urano);
	dibuja_planeta(neptuno);
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
	glutKeyboardFunc(teclasEspeciales);
	//Lazo principal
	glutMainLoop();
	return 0;
}
