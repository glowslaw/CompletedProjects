/*
Niniejszy program jest wolnym oprogramowaniem; możesz go
rozprowadzać dalej i / lub modyfikować na warunkach Powszechnej
Licencji Publicznej GNU, wydanej przez Fundację Wolnego
Oprogramowania - według wersji 2 tej Licencji lub(według twojego
wyboru) którejś z późniejszych wersji.

Niniejszy program rozpowszechniany jest z nadzieją, iż będzie on
użyteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domyślnej
gwarancji PRZYDATNOŚCI HANDLOWEJ albo PRZYDATNOŚCI DO OKREŚLONYCH
ZASTOSOWAŃ.W celu uzyskania bliższych informacji sięgnij do
Powszechnej Licencji Publicznej GNU.

Z pewnością wraz z niniejszym programem otrzymałeś też egzemplarz
Powszechnej Licencji Publicznej GNU(GNU General Public License);
jeśli nie - napisz do Free Software Foundation, Inc., 59 Temple
Place, Fifth Floor, Boston, MA  02110 - 1301  USA
*/

#define GLM_FORCE_RADIANS

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include "constants.h"
#include "allmodels.h"
#include "lodepng.h"
#include "shaderprogram.h"


float speed = 3; //Prędkość kątowa obrotu obiektu



//Procedura obsługi błędów
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

//Procedura obsługi klawiatury
/*
void key_callback(GLFWwindow* window, int key,
	int scancode, int action, int mods) {

	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_LEFT) speed=-PI; //Jeżeli wciśnięto klawisz "w lewo" ustaw prędkość na -PI
		if (key == GLFW_KEY_RIGHT) speed = PI; //Jeżeli wciśnięto klawisz "w prawo" ustaw prędkość na PI
	}

	if (action == GLFW_RELEASE) {
		speed = 0; //Jeżeli puszczono klawisz ustaw prędkość obrotu na zero
	}

}
*/

/*

float rot = 0;

void key_callback(GLFWwindow* window, int key,
	int scancode, int action, int mods) {

	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_A) rot = -PI/6; //Jeżeli wciśnięto klawisz "w lewo" ustaw prędkość na -PI
		if (key == GLFW_KEY_D) rot = PI/6; //Jeżeli wciśnięto klawisz "w prawo" ustaw prędkość na PI
	}

	if (action == GLFW_RELEASE) {
		rot = 0; //Jeżeli puszczono klawisz ustaw prędkość obrotu na zero
	}

}
*/

//Procedura inicjująca
void initOpenGLProgram(GLFWwindow* window) {
    initShaders();	
	//************Tutaj umieszczaj kod, który należy wykonać raz, na początku programu************
	glClearColor(0, 0, 0, 1);//Ustaw czarny kolor czyszczenia ekranu
	glEnable(GL_DEPTH_TEST); //Włącz test głębokości pikseli
	///glfwSetKeyCallback(window, key_callback); //Zarejestruj procedurę obsługi klawiatury
}


//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram(GLFWwindow* window) {
    freeShaders();
    //************Tutaj umieszczaj kod, który należy wykonać po zakończeniu pętli głównej************
}

void drawCog(glm::mat4 Mt) {
	//Załaduj macierz modelu i narysuj torus
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(Mt));
	glUniform4f(spLambert->u("color"), 0, 1, 0, 1);
	Models::torus.drawSolid();

	//W pętli narysuj 12 kostek.
	for (int k = 0; k < 12; k++) {
		//Macierz kostki to obrót, żeby wybrać kierunek wokół torusa...
		glm::mat4 Mk = glm::rotate(Mt, glm::radians(k * 30.0f ), glm::vec3(0.0f, 0.0f, 1.0f));
		//...przesunięcie żeby dotrzeć na obrzeże torusa...
		Mk = glm::translate(Mk, glm::vec3(1.0f, 0.0f, 0.0f));
		//...i skalowanie żeby pomniejszyć kostkę do porządanych rozmiarów
		Mk = glm::scale(Mk, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(Mk));
		glUniform4f(spLambert->u("color"), 0, 1, 0, 1);
		Models::cube.drawSolid();
	}
}

void draw_cube(float x, float y, float z, glm::mat4 I)
{
	Models::Cube cube;
	glm::mat4 Mt1 = glm::scale(I, glm::vec3(x/2, y/2, z/2));
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(Mt1));
	cube.drawSolid();
}

glm::mat4 draw_finger(glm::mat4 I, float rotation, float x, float y, float z,float last_x)
{
	glm::mat4 F;
	F = glm::translate(I, glm::vec3(last_x/2, 0.0f, 0.0f));
	F = glm::rotate(F, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
	F = glm::translate(F, glm::vec3(x/2, 0.0f, 0.0f));
	draw_cube(x, y, z, F);
	return F;
}

//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window,float angle) {
	//************Tutaj umieszczaj kod rysujący obraz******************l
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Wyczyszczenie bufora kolorów i bufora głębokości
	
	glm::mat4 P = glm::perspective(glm::radians(50.0f), 1.0f, 1.0f, 50.0f); //Wyliczenie macierzy rzutowania
	glm::mat4 V = glm::lookAt(glm::vec3(0.0f, 6.0f, -10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //Wyliczenie macierzy widoku

	spLambert->use();//Aktywacja programu cieniującego
	glUniformMatrix4fv(spLambert->u("P"), 1, false, glm::value_ptr(P)); //Załadowanie macierzy rzutowania do programu cieniującego
	glUniformMatrix4fv(spLambert->u("V"), 1, false, glm::value_ptr(V)); //Załadowanie macierzy widoku do programu cieniującego
	
	
	glm::mat4 I = glm::mat4(1.0f);

	/*

	glm::mat4 Mt1 = glm::translate(I, glm::vec3(-1.05f, 0.0f, 0.0f)); //Macierz torusa to najpierw przesunięcie do odpowiedniej pozycji...
	Mt1 = glm::rotate(Mt1, angle, glm::vec3(0.0f, 0.0f, 1.0f)); //... potem obrót żeby nasz "tryb" był odpowiednio obrócony
	drawCog(Mt1);

	glm::mat4 Mt2 = glm::translate(I, glm::vec3(1.05f, 0.0f, 0.0f)); //Macierz torusa to najpierw przesunięcie do odpowiedniej pozycji...
	Mt2 = glm::rotate(Mt2, -angle+glm::radians(15.0f), glm::vec3(0.0f, 0.0f, 1.0f)); //... potem obrót żeby nasz "tryb" był odpowiednio obrócony
	drawCog(Mt2);
	
	*/

	/*
	I = glm::rotate(I, angle, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 Mt1 = glm::scale(I, glm::vec3(1.5f, 0.125f, 1.0f));
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(Mt1));
	///glUniform4f(spLambert->u("color"), 0, 1, 0, 1);
	Models::cube.drawSolid();
	static Models::Torus kolo(0.3, 0.1, 36, 36);
	for (int i = 0; i < 4; i++)
	{
		glm::mat4 Mt2;
		if (i == 0)
			Mt2 = glm::translate(I, glm::vec3(-1.5f, 0.0f, -1.0f));
		if (i == 1)
			Mt2 = glm::translate(I, glm::vec3(1.5f, 0.0f, -1.0f));
		if (i == 2)
			Mt2 = glm::translate(I, glm::vec3(-1.5f, 0.0f, 1.0f));
		if (i == 3)
			Mt2 = glm::translate(I, glm::vec3(1.5f, 0.0f, 1.0f));
		///Mt2 = glm::rotate(Mt2, angle, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(Mt2));
		///glUniform4f(spLambert->u("color"), 0, 1, 0, 1);
		///Models::torus.drawSolid();
		kolo.drawWire();
	}
	*/

	/*
	I = glm::rotate(I, PI/2, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 Mt1 = glm::scale(I, glm::vec3(1.5f, 0.125f, 1.0f));
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(Mt1));
	///glUniform4f(spLambert->u("color"), 0, 1, 0, 1);
	Models::cube.drawSolid();
	static Models::Torus kolo(0.3, 0.1, 36, 36);
	for (int i = 0; i < 4; i++)
	{
		glm::mat4 Mt2;
		if (i == 0)
			Mt2 = glm::translate(I, glm::vec3(-1.5f, 0.0f, -1.0f));
		if (i == 1)
		{
			Mt2 = glm::translate(I, glm::vec3(1.5f, 0.0f, -1.0f));
			Mt2 = glm::rotate(Mt2, rot, glm::vec3(0.0f, 1.0f, 0.0f));
		}
		if (i == 2)
			Mt2 = glm::translate(I, glm::vec3(-1.5f, 0.0f, 1.0f));
		if (i == 3)
		{
			Mt2 = glm::translate(I, glm::vec3(1.5f, 0.0f, 1.0f));
			Mt2 = glm::rotate(Mt2, rot, glm::vec3(0.0f, 1.0f, 0.0f));
		}
		Mt2 = glm::rotate(Mt2, angle , glm::vec3(0.0f, 0.0f, -1.0f));
		glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(Mt2));
		///glUniform4f(spLambert->u("color"), 0, 1, 0, 1);
		///Models::torus.drawSolid();
		kolo.drawWire();
	}
	*/

	draw_cube(1.0f, 0.5f, 1.0f,I);
	for (int i = 0; i < 4; i++)
	{
		I = glm::rotate(I, PI/2 * i, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 F1 = draw_finger(I, 0.5f * sin(angle) * PI / 4 - PI / 8, 2.0f, 0.5f, 1.0f, 1.0f);
		glm::mat4 F2 = draw_finger(F1, 0.5f * sin(angle) * PI / 4 - PI / 8, 2.0f, 0.5f, 1.0f, 2.0f);
		glm::mat4 F3 = draw_finger(F2, 0.5f * sin(angle) * PI / 4 - PI / 8, 2.0f, 0.5f, 1.0f, 2.0f);
	}

	//Skopiowanie bufora ukrytego do widocznego. Z reguły ostatnie polecenie w procedurze drawScene.
	glfwSwapBuffers(window);
}


int main(void)
{
	GLFWwindow* window; //Wskaźnik na obiekt reprezentujący okno

	glfwSetErrorCallback(error_callback);//Zarejestruj procedurę obsługi błędów

	if (!glfwInit()) { //Zainicjuj bibliotekę GLFW
		fprintf(stderr, "Nie można zainicjować GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(500, 500, "OpenGL", NULL, NULL);  //Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL.

	if (!window) //Jeżeli okna nie udało się utworzyć, to zamknij program
	{
		fprintf(stderr, "Nie można utworzyć okna.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window); //Od tego momentu kontekst okna staje się aktywny i polecenia OpenGL będą dotyczyć właśnie jego.
	glfwSwapInterval(1); //Czekaj na 1 powrót plamki przed pokazaniem ukrytego bufora

	if (glewInit() != GLEW_OK) { //Zainicjuj bibliotekę GLEW
		fprintf(stderr, "Nie można zainicjować GLEW.\n");
		exit(EXIT_FAILURE);
	}

	initOpenGLProgram(window); //Operacje inicjujące


	float angle = 0; //Aktualny kąt obrotu obiektu
	glfwSetTime(0); //Wyzeruj timer
	//Główna pętla
	while (!glfwWindowShouldClose(window)) //Tak długo jak okno nie powinno zostać zamknięte
	{
		angle += speed * glfwGetTime(); //Oblicz przyrost kąta po obrocie
		glfwSetTime(0); //Wyzeruj timer
		drawScene(window,angle); //Wykonaj procedurę rysującą
		glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
	}

	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);
}
