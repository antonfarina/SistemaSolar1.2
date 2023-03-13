#include <windows.h>

#include <glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include "definiciones.h"
#include "funciones_ejes.h"
#include "esfera.h"

const int ANCHO_VENTANA = 700;
const int ALTO_VENTANA = 700;
//lista esfera
unsigned int esfera;

//no funciona
void suma_angulo(planeta p);

//datos: distancia al sol, velocidad de translacion, angulo de translacion, velocidad de rotacion, angulo de rotacion, tamaño
planeta sol = {0, 0, 0, 0, 0, 100};
planeta mercurio = {150, 2, 0, 0, 0, 20};
planeta venus = {250, 10, 150, 10, 0, 35};
planeta tierra = {450, 4, 0, 9, 0, 40};
planeta marte = {650, 3.7, 120, 10, 0, 30};
planeta jupiter = {850, 5, 0, 0, 0, 60};
planeta saturno = {1050, 6, 10, 10, 0, 50};
planeta urano = {1250, 8, 26, 10, 0, 45};
planeta neptuno = {1400, 2.5, 0, 10, 0, 45};

//satelites de la tierra
planeta luna = { 80, 10, 0, 15, 0, 10 };
planeta ISS = { 120, 7, 0, 12, 0, 9 };

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
	glColor3f(1.0f, 1.0f, 0.0f);
	//dibujamos el planeta
	glCallList(esfera);
	glPopMatrix();
	glPopMatrix();
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
				glCallList(esfera);
			glPopMatrix();
		glPopMatrix();
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
			glCallList(esfera);
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();

	dibuja_planeta(marte);
	dibuja_planeta(jupiter);
	dibuja_planeta(saturno);
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
	//actualizacion de los angulos
	sol.angulo_translacion += sol.velocidad_translacion;
	if (sol.angulo_translacion > 360)sol.angulo_translacion += -360;
	sol.angulo_rotacion += sol.velocidad_rotacion;
	if (sol.angulo_rotacion > 360)sol.angulo_rotacion += -360;
	//actualizacion de los angulos
	mercurio.angulo_translacion += mercurio.velocidad_translacion;
	if (mercurio.angulo_translacion > 360)mercurio.angulo_translacion += -360;
	mercurio.angulo_rotacion += mercurio.velocidad_rotacion;
	if (mercurio.angulo_rotacion > 360)mercurio.angulo_rotacion += -360;
	//actualizacion de los angulos
	venus.angulo_translacion += venus.velocidad_translacion;
	if (venus.angulo_translacion > 360)venus.angulo_translacion += -360;
	venus.angulo_rotacion += venus.velocidad_rotacion;
	if (venus.angulo_rotacion > 360)venus.angulo_rotacion += -360;
	//actualizacion de los angulos
	tierra.angulo_translacion += tierra.velocidad_translacion;
	if (tierra.angulo_translacion > 360)tierra.angulo_translacion += -360;
	tierra.angulo_rotacion += tierra.velocidad_rotacion;
	if (tierra.angulo_rotacion > 360)tierra.angulo_rotacion += -360;
	//actualizacion de los angulos
	luna.angulo_translacion += luna.velocidad_translacion;
	if (luna.angulo_translacion > 360)luna.angulo_translacion += -360;
	luna.angulo_rotacion += luna.velocidad_rotacion;
	if (luna.angulo_rotacion > 360)luna.angulo_rotacion += -360;
	//actualizacion de los angulos
	ISS.angulo_translacion += ISS.velocidad_translacion;
	if (ISS.angulo_translacion > 360)ISS.angulo_translacion += -360;
	ISS.angulo_rotacion += ISS.velocidad_rotacion;
	if (ISS.angulo_rotacion > 360)ISS.angulo_rotacion += -360;
	//actualizacion de los angulos
	marte.angulo_translacion += marte.velocidad_translacion;
	if (marte.angulo_translacion > 360)marte.angulo_translacion += -360;
	marte.angulo_rotacion += marte.velocidad_rotacion;
	if (marte.angulo_rotacion > 360)marte.angulo_rotacion += -360;
	//actualizacion de los angulos
	jupiter.angulo_translacion += jupiter.velocidad_translacion;
	if (jupiter.angulo_translacion > 360)jupiter.angulo_translacion += -360;
	jupiter.angulo_rotacion += jupiter.velocidad_rotacion;
	if (jupiter.angulo_rotacion > 360)jupiter.angulo_rotacion += -360;
	//actualizacion de los angulos
	saturno.angulo_translacion += saturno.velocidad_translacion;
	if (saturno.angulo_translacion > 360)saturno.angulo_translacion += -360;
	saturno.angulo_rotacion += saturno.velocidad_rotacion;
	if (saturno.angulo_rotacion > 360)saturno.angulo_rotacion += -360;
	//actualizacion de los angulos
	urano.angulo_translacion += urano.velocidad_translacion;
	if (urano.angulo_translacion > 360)urano.angulo_translacion += -360;
	urano.angulo_rotacion += urano.velocidad_rotacion;
	if (urano.angulo_rotacion > 360)urano.angulo_rotacion += -360;
	//actualizacion de los angulos
	neptuno.angulo_translacion += neptuno.velocidad_translacion;
	if (neptuno.angulo_translacion > 360)neptuno.angulo_translacion += -360;
	neptuno.angulo_rotacion += neptuno.velocidad_rotacion;
	if (neptuno.angulo_rotacion > 360)neptuno.angulo_rotacion += -360;
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
	//glutIdleFunc(Idle);

	openGlInit();
	movimiento();
	glutKeyboardFunc(teclasEspeciales);
	//Lazo principal
	glutMainLoop();
	return 0;
}

//no funciona
void suma_angulo(planeta p) {
	p.angulo_translacion += p.velocidad_translacion;
	if (p.angulo_translacion > 360)p.angulo_translacion += -360;
	p.angulo_rotacion += p.velocidad_rotacion;
	if (p.angulo_rotacion > 360)p.angulo_rotacion += -360;
	glutPostRedisplay();
}