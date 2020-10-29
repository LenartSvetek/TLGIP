#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <ctime>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/gtc/matrix_transform.hpp>

#include "Shader.h"
#include "Camera.h" 
#include "Rectangle.h"
#include "Cell.h"
#include "Snake.h"
#include "Fruit.h"

#define Nalova_Slinovka 0

const char* APP_TITLE = "Oh... shiat here we go again.";
int WINDOW_SIZE = 1024;
bool fullScreen = false;
bool wireFrame = false;
bool keys[1024] = {0};
GLFWwindow* pWindow = NULL;

FPS_Camera FPScamera;
const double ZOOM_SENSITIVITY = -3.0;
const float MOVE_SPEED = 50.0; //units per second
const float MOUSE_SENSITIVITY = 0.1f;

void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode);
void glfw_onMouseMove(GLFWwindow* window, double posX, double posY);
void glfw_onMouseScroll(GLFWwindow* window, double deltaX, double deltaY);
void update(double elapsedTime);
void glfw_onFrameBufferSize(GLFWwindow*, int widht, int height);
void showFPS(GLFWwindow* window);
bool initOpenGL();

std::vector<Rectangle> create_grid(int vel_tabele, float vel_celice, int vel_ekrana);

int main() {
	srand(time(NULL));
	int vel_tabele;
	std::cout << "Type in the size of map(5 = 5x5, 6 = 6x6...): ";
	std::cin >> vel_tabele;
	float vel_celice;
	std::cout << "Type in the size of cell(0 = default(32), small size may cause problems): ";
	std::cin >> vel_celice;
	if (vel_celice == 0) vel_celice = 32;
	bool key_buffer = false;
	float lenght;
	std::cout << "Type in the speed of snake(0 = default(0.08), smaller the value = faster the speed): ";
	std::cin >> lenght;
	if (lenght == 0) lenght == 0.08;
	WINDOW_SIZE = vel_celice * vel_tabele + vel_tabele - 2;
	if (!initOpenGL()) {
		std::cerr << "openGL initialisation failed...";
		return -1;
	}
	
	Shader Shader;
	Shader.LoadShaders("Shaders/Shader.vS", "Shaders/Shader.fS");

	Snake snake(vel_tabele/2, vel_tabele);

	FPScamera.move(glm::vec3(0));

	std::vector<Cell> CellList;

	for (int i = 0; i < vel_tabele; i++) {
		for (int j = 0; j < vel_tabele; j++) {
			Cell cell((vel_celice * j) + j, (vel_celice * i) + i, vel_celice);

			cell.setMode("Nada");
			CellList.push_back(cell);
		}
	}

	CellList[snake.getX() + snake.getY() * vel_tabele].setMode("Snake");
	CellList[snake.getX() + snake.getY() * vel_tabele].setHP(1);

	Fruit fruit(vel_tabele, CellList);

	/*Texture2D texture;
	texture.loadTexture("Textures/floor.jpg");
	
	Texture2D texture2;
	texture2.loadTexture("Textures/crate.jpg");*/

	double lastTime = glfwGetTime();

	float previousSecond = 0;
	
	//main loop
	while (!glfwWindowShouldClose(pWindow)) {
		showFPS(pWindow);

		double currentTime = glfwGetTime();
		double deltaTime = currentTime - lastTime;

		glfwPollEvents();
		//update(deltaTime);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//texture2.bind();

		glm::mat4 model, view, projection;

		if (keys['W'] || keys['S'] || keys['A'] || keys['D'] || keys[262] || keys[263] || keys[264] || keys[265]) {
			snake.changeDir(keys);
			key_buffer = true;
		}


		if (glfwGetTime() - previousSecond > lenght) {
			previousSecond = glfwGetTime();
			key_buffer = false;
			snake.move();
			int x = snake.getX();
			int y = snake.getY();
			if (x < 0 || x >= vel_tabele || y < 0 || y >= vel_tabele) {
				std::cout << "X: " << x << ", Y: " << y << ", vel_tabele: " << vel_tabele << std::endl;
				std::cout << "You hit a wall you dummy...";
				glfwSetWindowShouldClose(pWindow, GL_TRUE);
			}
			int pos = x + y * vel_tabele;
			std::string mode = CellList[pos].getMode();
			if (mode == "Nada"){
				CellList[pos].setMode("Snake");
				CellList[pos].setHP(snake.getScore());
			}
			else if (mode == "Snake" && CellList[pos].getHP() + 1 != snake.getScore()){
				std::cout << "You hit yourself dummy!" << std::endl;
				glfwSetWindowShouldClose(pWindow, GL_TRUE);
				lenght -= Nalova_Slinovka;
			}
			else if (mode == "Fruit") {
				//lenght -= 0.01;
				snake.upScore();
				CellList[pos].setMode("Snake");
				CellList[pos].setHP(snake.getScore());
				fruit.~Fruit();
				Fruit fruit(vel_tabele, CellList);
			}
			for (int i = 0; i < CellList.size(); i++) {
				CellList[i].update();
			}
		}
		
		model = glm::mat4(1.0f);

		//model = glm::rotate(model, glm::radians(cubeAngle), glm::vec3(0.0f, 1.0f, 0.0f));

		view = FPScamera.getViewMatrix();

		projection = glm::ortho((GLfloat)0, (GLfloat)WINDOW_SIZE, (GLfloat)WINDOW_SIZE, (GLfloat)0, 0.0f, 100.0f);

		Shader.use();

		glm::vec4 vCol(1.0f, 1.0f, 1.0f, 1.0f);

		Shader.setUniform("model", model);
		Shader.setUniform("view", view);
		Shader.setUniform("projection", projection);
		Shader.setUniform("vCol", vCol);

		for (int i = 0; i < CellList.size(); i++) {
			Shader.setUniform("vCol", CellList[i].getColour());
			CellList[i].draw();
		}

		//glUniform1i(glGetUniformLocation(shader.getProgram(), "myTexture"), 0);
		//glUniform1i(glGetUniformLocation(shader.getProgram(), "myTexture2"), 1);

		glfwSwapBuffers(pWindow);

		lastTime = currentTime;

		glUseProgram(0);
	}

	glfwTerminate();
	return 0;
}

void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode){
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}
	
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key == GLFW_KEY_E && action == GLFW_PRESS) {
		wireFrame = !wireFrame;
		if (wireFrame) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}
}

void glfw_onMouseMove(GLFWwindow* window, double posX, double posY){
	/*static glm::vec2 lastMousePos = glm::vec2(0, 0);

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == 1) {
		gYaw -= ((float)posX - lastMousePos.x) * MOUSE_SENSITIVITY;
		gPitch += ((float)posY - lastMousePos.y) * MOUSE_SENSITIVITY;
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == 1) {
		float dx = 0.01f * ((float)posX - lastMousePos.x);
		float dy = 0.01f * ((float)posY - lastMousePos.y);
	}

	lastMousePos.x = (float)posX;
	lastMousePos.y = (float)posY;*/


}

void glfw_onMouseScroll(GLFWwindow* window, double deltaX, double deltaY){
	double fov = FPScamera.getFOV() + deltaY * ZOOM_SENSITIVITY;

	fov = glm::clamp(fov, 1.0, 120.0);

	FPScamera.setFOV((float)fov);
}

void update(double elapsedTime){
	double mouseX, mouseY;

	glfwGetCursorPos(pWindow, &mouseX, &mouseY);

	//FPScamera.rotate((float)(WINDOW_WIDTH / 2.0 - mouseX) * MOUSE_SENSITIVITY, (float)(WINDOW_HEIGHT / 2.0 - mouseY) * MOUSE_SENSITIVITY);

	glfwSetCursorPos(pWindow, WINDOW_SIZE / 2.0, WINDOW_SIZE / 2.0);

	if (glfwGetKey(pWindow, GLFW_KEY_W) == GLFW_PRESS)
		FPScamera.move(MOVE_SPEED * (float)elapsedTime * FPScamera.getLook());
	else if (glfwGetKey(pWindow, GLFW_KEY_S) == GLFW_PRESS)
		FPScamera.move(MOVE_SPEED * (float)elapsedTime * -FPScamera.getLook());

	if (glfwGetKey(pWindow, GLFW_KEY_A) == GLFW_PRESS)
		FPScamera.move(MOVE_SPEED * (float)elapsedTime * -FPScamera.getRight());
	else if (glfwGetKey(pWindow, GLFW_KEY_D) == GLFW_PRESS)
		FPScamera.move(MOVE_SPEED * (float)elapsedTime * FPScamera.getRight());

	if (glfwGetKey(pWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
		FPScamera.move(MOVE_SPEED * (float)elapsedTime * FPScamera.getUp());
	else if(glfwGetKey(pWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		FPScamera.move(MOVE_SPEED * (float)elapsedTime * -FPScamera.getUp());
}

void glfw_onFrameBufferSize(GLFWwindow*, int width, int height){
	glfwSetWindowSize(pWindow, WINDOW_SIZE, WINDOW_SIZE);
}

void showFPS(GLFWwindow* window){
	static double previousSeconds = 0.0;
	static int frameCount = 0;
	double elapsedSeconds;
	double currentSeconds = glfwGetTime();

	elapsedSeconds = currentSeconds - previousSeconds;

	//limit text update 4 times per second
	if (elapsedSeconds > 0.25) {
		previousSeconds = currentSeconds;
		double fps = (double)frameCount / elapsedSeconds;
		double msPerFrame = 1000.0 / fps;

		std::ostringstream outs;
		outs.precision(3);
		outs << std::fixed << APP_TITLE << " FPS: " << fps << " Frame Time: " << msPerFrame << " ms";
		glfwSetWindowTitle(window, outs.str().c_str());
	
		frameCount = 0;
	}
	frameCount++;
}

bool initOpenGL(){
	if (!glfwInit()) {
		std::cerr << "GLFW failed to initialise...";
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	if (fullScreen) {
		GLFWmonitor* pMonitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* pMode = glfwGetVideoMode(pMonitor);
		if (pMode != NULL) {
			pWindow = glfwCreateWindow(pMode->width, pMode->height, APP_TITLE, pMonitor, NULL);
		}
	}
	else {
		pWindow = glfwCreateWindow(WINDOW_SIZE, WINDOW_SIZE, APP_TITLE, NULL, NULL);
	}

	if (pWindow == NULL) {
		std::cerr << "pWindow didn't create";
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(pWindow);

	glfwSetKeyCallback(pWindow, glfw_onKey);
	glfwSetCursorPosCallback(pWindow, glfw_onMouseMove);
	glfwSetScrollCallback(pWindow, glfw_onMouseScroll);

	glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(pWindow, WINDOW_SIZE / 2.0, WINDOW_SIZE / 2.0);

	glewExperimental = GL_TRUE;

	glViewport(0, 0, WINDOW_SIZE, WINDOW_SIZE);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	if (glewInit() != GLEW_OK) {
		std::cerr << "GLEW failed to initialise...";
		return false;
	}

	glEnable(GL_DEPTH_TEST);

	return true;
}

std::vector<Rectangle> create_grid(int vel_tabele, float vel_celice, int vel_ekrana) {
	std::vector<Rectangle> grid;
	
	// Vertical lines
	for (int i = 1; i < vel_tabele; i++) {
		Rectangle rect((vel_celice + 1) * i, 0, 1, vel_ekrana);
		grid.push_back(rect);
	}

	// Horizontal lines
	for (int i = 1; i < vel_tabele; i++) {
		Rectangle rect(0, (vel_celice + 1) * i, vel_ekrana, 1);
		grid.push_back(rect);
	}

	return grid;
}