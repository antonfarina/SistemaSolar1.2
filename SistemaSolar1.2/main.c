#include <windows.h>
#include <glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <math.h>
#include "funciones_ejes.h"
#include "geometrias.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

const int ANCHO_VENTANA = 1000;
const int ALTO_VENTANA = 700;

//indices listas esfera, cubo y toro
unsigned int esfera, cubo, toro;
//texturas
unsigned int anillos, fondo;
//camara alejada predeterminada
int camara = 1; 
//relacion de aspecto de la ventana
float aspecto = 1;
//datos: distancia al sol, velocidad de traslacion, angulo de traslacion, velocidad de rotacion, angulo de rotacion, tamaño, textura y lista de geometria
planeta sol = {0, 0, 0, 1, 0, 100, 0, &esfera};
planeta mercurio = {180, 10, 0, 0, 0, 25, 0, &esfera};
planeta venus = {280, 8, 150, 5.2, 0, 35, 0, &esfera};
planeta tierra = {450, 7, 0, 9, 0, 40, 0, &esfera};
planeta marte = {650, 6, 120, 10, 0, 35, 0, &esfera};
planeta jupiter = {800, 5, 0, 15, 0, 60, 0, &esfera};
planeta saturno = {1050, 4.4, 0, 9, 0, 50, 0, &esfera};
planeta urano = {1250, 3.7, 26, 6, 0, 45, 0, &esfera};
planeta neptuno = {1400, 2.5, 160, 11, 0, 45, 0, &esfera};
//satelites de la tierra
planeta luna = {80, 10, 0, 1, 225, 10, 0, &esfera};
planeta ISS = {120, 15, 0, 3, 0, 9, 0, &cubo};

//luces 
GLfloat ambiente[] = {0.0f, 0.0f, 0.0f, 1.0f};
GLfloat difusa[] = {1.0f, 1.0f, .9f, 1.0f};
GLfloat especular[] = {1.0f, 1.0f, 1.0f, 1.0f};
//luz en el (0,0,0)
GLfloat posicion_luz[] = {0.0f, 0.0f, 0.0f, 1.0f};
GLfloat direccion_luz[] = {1.0f, 1.0f, 1.0f, 1.0f};

void cambio_camara() {
	//vector de planetas que se pueden ver desde la tierra
	planeta planetas[] = { sol, mercurio, venus, luna, ISS, marte, jupiter, saturno, urano, neptuno };
	if (camara == 1) {
		camaraAlejada(aspecto);
	}else if (camara == 5 || camara == 6) {
		//telescopio a satelites desde la tierra
		telescopio(tierra.distancia_sol, tierra.angulo_traslacion, planetas[camara - 2].distancia_sol, planetas[camara - 2].angulo_traslacion, 1, aspecto);
	}else if(camara == 12 || camara == 13){
		//satelites a la tierra
		telescopio(planetas[camara-9].distancia_sol, planetas[camara - 9].angulo_traslacion, tierra.distancia_sol, tierra.angulo_traslacion, 2, aspecto);
	}else {
		telescopio(tierra.distancia_sol, tierra.angulo_traslacion, planetas[camara - 2].distancia_sol, planetas[camara - 2].angulo_traslacion, 0, aspecto);
	}
}

//funcion que carga la textura en el planeta a partir de una imagen
void crea_textura(int* textura, char* ruta) {
	int width, height, nrChannels, formato;
	//geneneramos la textura
	glGenTextures(1, textura);
	glBindTexture(GL_TEXTURE_2D, *textura);
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
	if (nrChannels == 3) {
		formato = GL_RGB;
	}
	else if (nrChannels == 1) {
		formato = GL_RED;
	}
	else {
		formato = GL_RGBA;
	}

	if (imagen) {
		glTexImage2D(GL_TEXTURE_2D, 0, formato, width, height, 0, formato, GL_UNSIGNED_BYTE, imagen);

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
			if(get_ejes())dibuja_ejes();
			//usamos la textura
			glBindTexture(GL_TEXTURE_2D, p.textura);
			//dibujamos el planeta
			glCallList(*(p.lista_render));
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
			if (get_ejes())dibuja_ejes();
			//usamos la textura
			glBindTexture(GL_TEXTURE_2D, tierra.textura);
			//dibujamos la tierra
			glCallList(*(tierra.lista_render));
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
				//escalamos a su posicion
				glScalef(luna.tamano, luna.tamano, luna.tamano);
				if (get_ejes())dibuja_ejes();
				//usamos la textura
				glBindTexture(GL_TEXTURE_2D, luna.textura);
				glCallList(*(luna.lista_render));
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
				//escalamos a su posicion
				glScalef(2 * ISS.tamano, 1.3 * ISS.tamano, 2 * ISS.tamano);
				if (get_ejes())dibuja_ejes();
				//usamos la textura
				glBindTexture(GL_TEXTURE_2D, ISS.textura);
				glDisable(GL_LIGHTING);
				glCallList(*(ISS.lista_render));
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
			if (get_ejes())dibuja_ejes();
			glBindTexture(GL_TEXTURE_2D, saturno.textura);
			//dibujamos saturno
			glCallList(*(saturno.lista_render));
		glPopMatrix();
		//rotamos el anillo 
		glRotatef(80, 1.0f, 0.0f, 0.0f);
		//aplastamos el toro en el eje z
		glScalef(1, 1, 0);
		//textura
		glBindTexture(GL_TEXTURE_2D, anillos);
		//llamada a la lista
		glCallList(toro);
		//quitamos la textura
		glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();
}

void usar_menu(int opcion) {
	camara = opcion;
	glutPostRedisplay();
}

void menu(void) {
	//creacion del menu
	int menu;
	menu = glutCreateMenu(usar_menu);
	glutAddMenuEntry("Alejada", 1);
	glutAddMenuEntry("Tierra al Sol", 2);
	glutAddMenuEntry("Tierra a Mercurio", 3);
	glutAddMenuEntry("Tierra a Venus", 4);
	glutAddMenuEntry("Tierra a la Luna", 5);
	glutAddMenuEntry("Tierra a la ISS", 6);
	glutAddMenuEntry("Tierra a Marte", 7);
	glutAddMenuEntry("Tierra a Jupiter", 8);
	glutAddMenuEntry("Tierra a Saturno", 9);
	glutAddMenuEntry("Tierra a Urano", 10);
	glutAddMenuEntry("Tierra a Neptuno", 11);
	glutAddMenuEntry("Luna a la Tierra", 12);
	glutAddMenuEntry("ISS a la Tierra", 13);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}
// Función de display
void Display(void) {
	cambio_camara();
	// Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Se activa la matriz del modelo
	glMatrixMode(GL_MODELVIEW);
	// Inicializamos la matriz del modelo a la identidad
	glLoadIdentity();
	//poligonos rellenos
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//quitamos las luces para dibujar el fondo y el sol
	glDisable(GL_LIGHTING);
	//textura del fondo
	glBindTexture(GL_TEXTURE_2D, fondo);
	//deshabilitamos la ocultacion de caras
	glDisable(GL_CULL_FACE);
	//envolvemos el sistema solar en una esfera con las normales orientadas hacia dentro
	glPushMatrix();
		glScalef(DISTANCIA * 3, DISTANCIA * 3, DISTANCIA * 3);
		glCallList(esfera);
	glPopMatrix();
	glEnable(GL_CULL_FACE);
	//orbitas sin textura
	glBindTexture(GL_TEXTURE_2D, 0);
	//si el flag esta a 1 dibujamos las orbitas
	if (get_orbitas() == 1) {
		dibujar_orbitas();
	}
	//dibujamos los planetas
	dibuja_planeta(sol);
	if (get_luces() == 1) glEnable(GL_LIGHTING);
	dibuja_planeta(mercurio);
	dibuja_planeta(venus);
	dibuja_planeta(marte);
	dibuja_planeta(jupiter);
	dibuja_saturno();
	dibuja_planeta(urano);
	dibuja_planeta(neptuno);
	dibuja_tierra();
	if (get_luces() == 1) glEnable(GL_LIGHTING);
	
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
	//luces
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambiente);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, difusa);
	glLightfv(GL_LIGHT0, GL_SPECULAR, especular);
	glLightfv(GL_LIGHT0, GL_POSITION, posicion_luz);
	//angulo de apertura del foco de la luz
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 180.0f);
	//habilitamos la luz
	glEnable(GL_LIGHT0);
	//para el color
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, especular);
	glMateriali(GL_FRONT, GL_SHININESS, 128);
	glEnable(GL_SMOOTH);
	glShadeModel(GL_SMOOTH);

	//texturas
	//las habilitamos
	glEnable(GL_TEXTURE_2D);
	//para las texturas transparentes
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//creamos las texturas
	crea_textura(&sol.textura, "../texturas/sol.jpg");
	crea_textura(&mercurio.textura, "../texturas/mercurio.jpg");
	crea_textura(&venus.textura, "../texturas/venus.jpg");
	crea_textura(&tierra.textura, "../texturas/tierra_nubes.bmp");
	crea_textura(&luna.textura, "../texturas/luna.jpg");
	crea_textura(&marte.textura, "../texturas/marte.jpg");
	crea_textura(&jupiter.textura, "../texturas/jupiter.jpg");
	crea_textura(&saturno.textura, "../texturas/saturno.jpg");
	crea_textura(&urano.textura, "../texturas/urano.jpg");
	crea_textura(&neptuno.textura, "../texturas/neptuno.jpg");
	crea_textura(&anillos, "../texturas/anillos.jpg");
	crea_textura(&fondo, "../texturas/estrellas.jpg");
	crea_textura(&ISS.textura, "../texturas/iss.png");
	//creamos las listas para las geometrias de los cuerpos celestes
	//lista para el toro
	toro = glGenLists(1);
	glNewList(toro, GL_COMPILE);
	arrayToro(12, saturno.tamano + 1, 5, 20, anillos);
	glEndList();
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
}

int main(int argc, char** argv) {
	//menu
	printf("Sistema solar con OPENGL 1.2 y glut\n");
	printf("Opciones:\n");
	printf("\tFlechas de teclado: mover la camara en modo alejada\n");
	printf("\tTeclas +/-: zoom en modo alejado\n");
	printf("\tLetra o: activar/desactivar orbitas\n");
	printf("\tLetra m: activar/desactivar movimiento\n");
	printf("\tLetra l: activar/desactivar iluminacion\n");
	printf("\tLetra e: activar/desactivar ejes\n");
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
