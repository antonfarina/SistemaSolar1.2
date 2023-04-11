#include <windows.h>
#include <glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <math.h>
#include "funciones_ejes.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

const int ANCHO_VENTANA = 700;
const int ALTO_VENTANA = 700;

extern void arrayEsfera();
extern void arrayCubo();
//lista esfera
unsigned int esfera, cubo, anillo;
//camara alejada predeterminada
int camara = 1; 
float aspecto = 1;
//datos: distancia al sol, velocidad de traslacion, angulo de traslacion, velocidad de rotacion, angulo de rotacion, tamaño
planeta sol = {0, 0, 0, 1, 0, 100};
planeta mercurio = {180, 10, 0, 0, 0, 25};
planeta venus = {280, 8, 150, 5.2, 0, 35};
planeta tierra = {450, 7, 0, 9, 0, 40};
planeta marte = {650, 6, 120, 10, 0, 35};
planeta jupiter = {800, 5, 0, 15, 0, 60};
planeta saturno = {1050, 4.4, 0, 9, 0, 50};
planeta urano = {1250, 3.7, 26, 6, 0, 45};
planeta neptuno = {1400, 2.5, 160, 11, 0, 45};

//satelites de la tierra
planeta luna = {80, 10, 0, 2, 225, 10};
planeta ISS = {120, 15, 0, 3, 0, 9};

//luces 
GLfloat ambiente[] = {0.0f, 0.0f, 0.0f, 1.0f};
GLfloat difusa[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat especular[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat posicion_luz[] = {0.0f, 0.0f, 0.0f, 1.0f};
GLfloat direccion_luz[] = {1.0f, 1.0f, 1.0f, 1.0f};
void toro(double r, double R, int rSeg, int cSeg, int texture) {
	glFrontFace(GL_CW);

	glBindTexture(GL_TEXTURE_2D, texture);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	const double TAU = 2 * PI;

	for (int i = 0; i < rSeg; i++) {
		glBegin(GL_QUAD_STRIP);
		for (int j = 0; j <= cSeg; j++) {
			for (int k = 0; k <= 1; k++) {
				double s = (i + k) % rSeg + 0.5;
				double t = j % (cSeg + 1);

				double x = (R + r * cos(s * TAU / rSeg)) * cos(t * TAU / cSeg);
				double y = (R + r * cos(s * TAU / rSeg)) * sin(t * TAU / cSeg);
				double z = r * sin(s * TAU / rSeg);

				double u = (i + k) / (float)rSeg;
				double v = t / (float)cSeg;

				glTexCoord2d(u, v);
				glNormal3f(2 * x, 2 * y, 2 * z);
				glVertex3d(2 * x, 2 * y, 2 * z);
			}
		}
		glEnd();
	}
	glFrontFace(GL_CCW);
}
//funcion que carga la textura en el planeta a partir de una imagen
void crea_textura(planeta* p, char* ruta) {
	int width, height, nrChannels;
	//geneneramos la textura
	glGenTextures(1, &(p->textura));
	glBindTexture(GL_TEXTURE_2D, p->textura);
	//parametros wrap
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//parametros de escala
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	
	//para que las texturas aparezcan al derecho
	stbi_set_flip_vertically_on_load(1);
	//cargamos la imagen
	unsigned char* imagen = stbi_load(ruta, &width, &height, &nrChannels, 0);
	if (imagen) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imagen);

	} else {
		printf("Fallo en la carga de la textura %s\n", imagen);
	}
	stbi_image_free(imagen);
}

//funcion que reescala los objetos segun el tamaño de la ventana
void cambiar_tamano(GLint nuevo_ancho, GLint nuevo_alto) {
	//establecemos el viewport al tamaño
	glViewport(0, 0, nuevo_ancho, nuevo_alto);
	//matriz de proyeccion
	glMatrixMode(GL_PROJECTION);
	aspecto = (float)nuevo_ancho / (float)nuevo_alto;
	//cargamos la identidad
	glLoadIdentity();
	gluPerspective(45, aspecto, 1, DISTANCIA * 2);
}

//funcion que actualiza el angulo de rotacion y traslacion de un planeta
void suma_angulo(planeta* p) {
	//actualizamos la velocidad de traslacion
	p->angulo_traslacion += p->velocidad_traslacion;
	if (p->angulo_traslacion > 360)p->angulo_traslacion += -360;
	//actualizamos la velocidad de rotacion
	p->angulo_rotacion += p->velocidad_rotacion;
	if (p->angulo_rotacion > 360)p->angulo_rotacion += -360;
}

//funcion de dibujo de las orbitas de los planetas
void dibujar_orbitas() {
	float x, z;
	float radios[] = {mercurio.distancia_sol, venus.distancia_sol, tierra.distancia_sol, marte.distancia_sol, jupiter.distancia_sol, saturno.distancia_sol, urano.distancia_sol, neptuno.distancia_sol};
	//orbitas de los planetas
	for (int j = 0; j < 8; j++) {
	//dibujamos en un lazo
		glBegin(GL_LINE_LOOP);
		glColor3f(0.7f, 0.7f, 0.7f);
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

	//orbita de la luna
	glBegin(GL_LINE_LOOP);
	glColor3f(0.7f, 0.7f, 0.7f);
	for (int i = 0; i < 100; i++) {
		//componente x
		x = luna.distancia_sol * cos(2.0 * PI * i / 100.0) + tierra.distancia_sol * cos(-tierra.angulo_traslacion * PI / 180);
		//componene z
		z = luna.distancia_sol * sin(2.0 * PI * i / 100.0) + tierra.distancia_sol * sin(-tierra.angulo_traslacion * PI / 180);;
		//la componente y=0
		glVertex3f(x, 0.0, z);
	}
	glEnd();

	//orbita de la ISS
	glBegin(GL_LINE_LOOP);
	glColor3f(0.7f, 0.7f, 0.7f);
	for (int i = 0; i < 100; i++) {
		//componente x
		x = ISS.distancia_sol * cos(2.0 * PI * i / 100.0) + tierra.distancia_sol * cos(-tierra.angulo_traslacion * PI / 180);
		//componene z
		z = ISS.distancia_sol * sin(2.0 * PI * i / 100.0) + tierra.distancia_sol * sin(-tierra.angulo_traslacion * PI / 180);;
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
			//usamos la textura
			glBindTexture(GL_TEXTURE_2D, p.textura);
			//dibujamos el planeta
			glCallList(esfera);
		glPopMatrix();
	glPopMatrix();
	//quitamos la textura
	glBindTexture(GL_TEXTURE_2D, 0);
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
			//escalamos
			glScalef(tierra.tamano, tierra.tamano, tierra.tamano);
			dibuja_ejes();
			//usamos la textura
			glBindTexture(GL_TEXTURE_2D, tierra.textura);
			//dibujamos la tierra
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
				//usamos la textura
				glBindTexture(GL_TEXTURE_2D, luna.textura);
				glCallList(esfera);
			glPopMatrix();
		glPopMatrix();

		//dibujo de la ISS
		glPushMatrix();
			//rotamos alrededor de la tierra
			glRotatef(ISS.angulo_traslacion, 0, 1, 0);
			//trasladamos el la luna a su posicion
			glTranslatef(ISS.distancia_sol, 0, 0);
			glPushMatrix();
				//rotamos alrededor de si misma
				glRotatef(ISS.angulo_rotacion, 0, 1, 0);
				dibuja_ejes();
				glDisable(GL_LIGHTING);
				//escalamos a su posicion
				glScalef(2*ISS.tamano, ISS.tamano, ISS.tamano);
				//usamos la textura
				glBindTexture(GL_TEXTURE_2D, ISS.textura);
				glCallList(cubo);
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();
	//quitamos la textura
	glBindTexture(GL_TEXTURE_2D, 0);
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
			glBindTexture(GL_TEXTURE_2D, saturno.textura);
			//dibujamos saturno
			glCallList(esfera);
		glPopMatrix();
		//rotamos el anillo 
		glRotatef(80, 1.0f, 0.0f, 0.0f);
		//usamos un toro de anillo
		glScalef(1, 1, 0);
		glBindTexture(GL_TEXTURE_2D, anillo);
		toro(12, saturno.tamano+1, 5, 20, anillo);
		glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();
}

void usar_menu(int opcion) {
	//opciones del menu para colocar la camara
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

void menu(void) {
	//creacion del menu
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
	glutAddMenuEntry("Neptuno", 11);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}
// Función de display
void Display(void) {
	//switch de la camara/telescopio
	switch (camara) {
	case 1: 
		moverCamara(aspecto);
		break;
	case 2:
		camara = 2;
		telescopio(tierra.distancia_sol, tierra.angulo_traslacion, sol.distancia_sol, sol.angulo_traslacion,0, aspecto);
		break;
	case 3:
		camara = 3;
		telescopio(tierra.distancia_sol, tierra.angulo_traslacion, mercurio.distancia_sol, mercurio.angulo_traslacion, 0, aspecto);
		break;
	case 4:
		camara = 4;
		telescopio(tierra.distancia_sol, tierra.angulo_traslacion, venus.distancia_sol, venus.angulo_traslacion,0, aspecto);
		break;
	case 5:
		camara = 5;
		telescopio(tierra.distancia_sol, tierra.angulo_traslacion, luna.distancia_sol, luna.angulo_traslacion, 1, aspecto);
		break;
	case 6:
		camara = 6;
		telescopio(tierra.distancia_sol, tierra.angulo_traslacion, ISS.distancia_sol, ISS.angulo_traslacion, 1, aspecto);
		break;
	case 7:
		camara = 7;
		telescopio(tierra.distancia_sol, tierra.angulo_traslacion, marte.distancia_sol, marte.angulo_traslacion, 0, aspecto);
		break;
	case 8:
		camara = 8;
		telescopio(tierra.distancia_sol, tierra.angulo_traslacion, jupiter.distancia_sol, jupiter.angulo_traslacion, 0, aspecto);
		break;
	case 9:
		camara = 9;
		telescopio(tierra.distancia_sol, tierra.angulo_traslacion, saturno.distancia_sol, saturno.angulo_traslacion, 0, aspecto);
		break;
	case 10:
		camara = 10;
		telescopio(tierra.distancia_sol, tierra.angulo_traslacion, urano.distancia_sol, urano.angulo_traslacion, 0, aspecto);
		break;
	case 11:
		camara = 11;
		telescopio(tierra.distancia_sol, tierra.angulo_traslacion, neptuno.distancia_sol, neptuno.angulo_traslacion, 0, aspecto);
		break;
	}
	// Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Se activa la matriz del modelador
	glMatrixMode(GL_MODELVIEW);
	// Inicializamos la matriz del modelo a la identidad
	glLoadIdentity();
	//dibujo de los planetas
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//dibujamos el sol
	glDisable(GL_LIGHTING);
	//si el flag esta a 1 dibujamos las orbitas
	if (get_orbitas() == 1) {
		//glBindTexture(GL_TEXTURE_2D, 0);
		dibujar_orbitas();
	}
	dibuja_planeta(sol);
	if (get_luces() == 1) glEnable(GL_LIGHTING);
	dibuja_planeta(mercurio);
	dibuja_planeta(venus);
	dibuja_tierra();
	if (get_luces() == 1) glEnable(GL_LIGHTING);
	dibuja_planeta(marte);
	dibuja_planeta(jupiter);
	dibuja_saturno();
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
	glClearColor(0.035f, 0.051f, 0.19f, 1.0f);  // valor limpieza buffer color
	glEnable(GL_DEPTH_TEST); // z-buffer
	glEnable(GL_CULL_FACE); //ocultacion caras back
	glEnable(GL_NORMALIZE);
	glCullFace(GL_BACK);
	glEnable(GL_LIGHTING);
	glShadeModel(GL_FLAT);
	//creacion de la lista de la esfera
	esfera = glGenLists(1);
	glNewList(esfera, GL_COMPILE);
	arrayEsfera();
	glEndList();

	//lista para el cubo
	cubo = glGenLists(1);
	glNewList(cubo, GL_COMPILE);
	arrayCubo();
	glEndList();

	//luces
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambiente);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, difusa);
	glLightfv(GL_LIGHT0, GL_SPECULAR, especular);
	glLightfv(GL_LIGHT0, GL_POSITION, posicion_luz);
	//angulo de apertura del foco de la luz
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 179.0f);
	//habilitamos la luz
	glEnable(GL_LIGHT0);
	//para el color
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glMateriali(GL_FRONT, GL_SHININESS, 1);
	glEnable(GL_SMOOTH);
	glShadeModel(GL_SMOOTH);

	//texturas
	//las habilitamos
	glEnable(GL_TEXTURE_2D);
	//creamos las texturas
	crea_textura(&sol, "./texturas/sol.jpg");
	crea_textura(&mercurio, "./texturas/mercurio.jpg");
	crea_textura(&venus, "./texturas/venus.jpg");
	crea_textura(&tierra, "./texturas/tierra_nubes.bmp");
	crea_textura(&luna, "./texturas/luna.jpg");
	crea_textura(&ISS, "./texturas/iss4.jpg");
	crea_textura(&marte, "./texturas/marte.jpg");
	crea_textura(&jupiter, "./texturas/jupiter.jpg");
	crea_textura(&saturno, "./texturas/saturno.jpg");
	crea_textura(&urano, "./texturas/urano.jpg");
	crea_textura(&neptuno, "./texturas/neptuno.jpg");
	int width, height, nrChannels;
	//geneneramos la textura
	glGenTextures(1, &anillo);
	glBindTexture(GL_TEXTURE_2D, anillo);
	//parametros wrap
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	//parametros de escala
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//para que las texturas aparezcan al derecho
	stbi_set_flip_vertically_on_load(1);
	//cargamos la imagen
	unsigned char* imagen = stbi_load("./texturas/anillos.jpg", &width, &height, &nrChannels, 0);
	if (imagen) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imagen);

	}
	else {
		printf("Fallo en la carga de la textura %s\n", imagen);
	}
	stbi_image_free(imagen);
}

int main(int argc, char** argv) {
	//menu
	printf("Sistema solar con OPENGL 1.2 y glut\n");
	printf("Opciones:\n");
	printf("\tFlechas de teclado: mover la camara en modo alejada\n");
	printf("\tLetra o: activar/desactivar orbitas\n");
	printf("\tLetra m: activar/desactivar movimiento\n");
	printf("\tLetra l: activar/desactivar iluminacion\n");
	printf("\Clic derecho: abrir menu telescopio\n");
	glutInit(&argc, argv);
	//Posicion de la ventana
	glutInitWindowPosition(100, 100);
	//Tamano de la ventana
	glutInitWindowSize(ANCHO_VENTANA, ALTO_VENTANA);
	//Tipo de color y buffer
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	//Crear la ventana
	glutCreateWindow("Sistema solar en OpenGL 1.2");
	//funcion de escalado de la ventana
	glutReshapeFunc(cambiar_tamano);
	//Funcion de dibujo
	glutDisplayFunc(Display);
	//Funcion de actualizacion
	glutIdleFunc(Idle);
	openGlInit();
	//funcion de movimiento
	movimiento();
	//funciones de teclado
	glutKeyboardFunc(teclas);
	glutSpecialFunc(teclasEspeciales);
	//menu
	menu();
	//Lazo principal
	glutMainLoop();
	return 0;
}
