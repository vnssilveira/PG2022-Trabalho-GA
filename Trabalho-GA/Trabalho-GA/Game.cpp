#include "Game.h"
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>

static const float screenWidth = 1400.0f, screenHeight = 800.0f;
static const float maxScreenWidth = 20;
static float frameWidth = 0.5;
static GLuint width = screenWidth, height = screenHeight;

static bool screenSizeChanged;
bool game = true;
bool isJumping = false;
bool isRaising = true;
const int bombs = 1;

const int backgroud	= 0;
const int character	= 1;
const int bomb = 2;

enum Movement{LEFT, STOP, RIGHT, JUMPING};
static Movement movement = STOP;
int characterType = 0;
float positionYCharacter = 0.0;
const float characterSpeed = 0.35f;

void Game::start()
{
	initialize();

	initializeScene();

	while (!glfwWindowShouldClose(window) && game)
	{
		glfwPollEvents();

		updateSprites();
		renderSprites();

		glfwSwapBuffers(window);
	}

}

void Game::initializeScene()
{
	//Cenário backgroud
	unsigned int texID = loadTexture("../textures/cenario.jpg");
	for (int aux = 0; aux < maxScreenWidth; aux++) {
		layers[backgroud].addObject(screenWidth * frameWidth, screenHeight/2, 0.0, screenWidth, screenHeight, 0.0f, shader);
		layers[backgroud].objects[aux]->setTexture(texID);
		frameWidth += 1.0;
	}
	layers[backgroud].setDesloc(-20.0f);

	//Personagem
	layers[character].addObject(50.0f, 240.0f, 0.0f, 100.0f, 160.0f, 0.0f, shader);
	layers[character].setDesloc(0.2f);
	texID = loadTexture("../textures/personagem.png");
	layers[character].objects[0]->setTexture(texID);
	layers[character].objects[0]->tempo = 100.0f;
	positionYCharacter = 240.0f;

	layers[character].addObject(-100.0f, 240.0f, 0.0f, 100.0f, 160.0f, 0.0f, shader);
	layers[character].setDesloc(0.2f);
	texID = loadTexture("../textures/personagemVolta.png");
	layers[character].objects[1]->setTexture(texID);
	layers[character].objects[1]->tempo = 100.0f;

	//Bomba, sorteia posições
	texID = loadTexture("../textures/bomb.png");
	frameWidth = 0.5;
	for (int aux = 0; aux < maxScreenWidth; aux++) {

		for (int l = 0; l < bombs; l++) {
			float drawnNumber = rand() % 100;

			if (drawnNumber > 30.0) {
				float width = (drawnNumber / 100.0f) * screenWidth;
				layers[bomb].addObject((screenWidth * aux) + width, 240.0f, 0.0f, 120.0f, 160.0f, 0.0f, shader);
				layers[bomb].objects[aux]->setTexture(texID);
			}
			else {
				layers[bomb].addObject(screenWidth * (aux + 1), 240.0f, 0.0f, 120.0f, 160.0f, 0.0f, shader);
				layers[bomb].objects[aux]->setTexture(texID);
			}
		}
	}
	layers[bomb].setDesloc(-20.0f);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Game::updateSprites()
{

	if (characterType == 0) {
		layers[character].objects[0]->setPosition(glm::vec3(layers[character].objects[0]->getPosX(), positionYCharacter, layers[character].objects[0]->getPosZInicial()));
		layers[character].objects[1]->setPosition(glm::vec3(layers[character].objects[1]->getPosX(), -100.0f, layers[character].objects[1]->getPosZInicial()));
	}

	if (characterType == 1) {
		layers[character].objects[0]->setPosition(glm::vec3(layers[character].objects[0]->getPosX(), -100.0f, layers[character].objects[0]->getPosZInicial()));
		layers[character].objects[1]->setPosition(glm::vec3(layers[character].objects[0]->getPosX(), positionYCharacter, layers[character].objects[1]->getPosZInicial()));
	}

	switch (movement)
	{
	case LEFT:
		if (layers[character].objects[characterType]->getPosX() > 50) {
			layers[character].objects[0]->setPosX(layers[character].objects[0]->getPosX() - characterSpeed);
			layers[character].objects[1]->setPosX(layers[character].objects[1]->getPosX() - characterSpeed);
		}
		break;
	case RIGHT:
		if (layers[character].objects[characterType]->getPosX() < screenWidth - 50) {
			layers[character].objects[0]->setPosX(layers[character].objects[0]->getPosX() + characterSpeed);
			layers[character].objects[1]->setPosX(layers[character].objects[1]->getPosX() + characterSpeed);
		}
		break;
	case JUMPING:
		if (!isJumping) {
		    isJumping = true;
		}
			
	default:
		break;
	}

	//Pular
	if (isJumping && layers[character].objects[characterType]->tempo >= 5.0) {

		if (isRaising) {
			layers[character].objects[characterType]->setPosY(layers[character].objects[characterType]->getPosY() + 0.5f);
		}
		else {
			layers[character].objects[characterType]->setPosY(layers[character].objects[characterType]->getPosY() - 0.3f);
		}

		positionYCharacter = layers[character].objects[characterType]->getPosY();

		if (layers[character].objects[characterType]->getPosY() > 500) {
			isRaising = false;
		}
		else if (layers[character].objects[characterType]->getPosY() < 240) {
			isJumping = false;
			isRaising = true;
			layers[character].objects[0]->tempo = 0.0f;
			layers[character].objects[1]->tempo = 0.0f;
		}
	}

	//Andar cenário
	for (int aux = 0; aux < maxScreenWidth; aux++) {
		
		layers[backgroud].objects[aux]->setPosition(glm::vec3(layers[backgroud].objects[aux]->getPosXInicial() + layers[backgroud].objects[aux]->tempo * layers[backgroud].getDesloc(), layers[backgroud].objects[aux]->getPosYInicial(), layers[backgroud].objects[aux]->getPosZInicial()));
		layers[backgroud].objects[aux]->tempo += 0.01f;
	}

	//Andar inimigo
	for (int aux = 0; aux < maxScreenWidth; aux++) {

		float bombY = (int) layers[bomb].objects[aux]->getPosY();
		float bombX = (int) layers[bomb].objects[aux]->getPosX();
		float characterX = (int) layers[character].objects[characterType]->getPosX();
		float characterY = (int)layers[character].objects[characterType]->getPosY();

		if ((bombX + 75  > characterX && bombX - 75 < characterX) &&
			(bombY + 120 > characterY && bombY - 120 < characterY)) {

			for (int aux = 0; aux < 500000000; aux++);
			game = false;
		}
	
		layers[bomb].objects[aux]->setPosition(glm::vec3(layers[bomb].objects[aux]->getPosXInicial() + layers[bomb].objects[aux]->tempo * layers[bomb].getDesloc(), layers[bomb].objects[aux]->getPosYInicial(), layers[bomb].objects[aux]->getPosZInicial()));
		layers[bomb].objects[aux]->tempo += 0.01f;
	}

	//Andar personagem
	for (int aux = 0; aux < 2; aux++) {
		layers[character].objects[aux]->setPosition(glm::vec3(layers[character].objects[aux]->getPosX(), layers[character].objects[aux]->getPosY(), layers[character].objects[aux]->getPosZInicial()));
		layers[character].objects[aux]->tempo += 0.025f;
	}

}

void Game::renderSprites()
{
	glClearColor(0.89f, 0.89f, 0.87f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (screenSizeChanged) {
		setupCamera2D();
		screenSizeChanged = false;
	}

	for (int aux = 0; aux < 3; aux++) {
		layers[aux].update();
	}
}

void Game::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		movement = RIGHT;
		characterType = 0;
	}
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		movement = LEFT;
		characterType = 1;
	}
	else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		movement = JUMPING;
		characterType = 0;
	}
	else
	{
		movement = STOP;
		characterType = 0;
	}

}

unsigned int Game::loadTexture(string filename)
{
	unsigned int texture;

	// Gera o identificador da textura na memória 
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); 
										
	//Ajusta os parâmetros de wrapping e filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Carregamento da imagem
	int width, height, nrChannels;
	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
	
	if (data)
	{
		if (nrChannels == 3) //jpg, bmp
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else //png
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, 0); 

	glActiveTexture(GL_TEXTURE0);

	return texture;
}


void Game::initialize()
{
	glfwInit();

	window = glfwCreateWindow(width, height, "Trabalho-GA Mayara Damiani e Vanessa Silveira", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);
	glfwSetWindowSizeCallback(window, resize);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}

	shader = new Shader(((string)"../shaders/sprite.vs").c_str(), ((string)"../shaders/sprite.fs").c_str());

	screenSizeChanged = true;

	glEnable(GL_DEPTH);
}

void Game::setupCamera2D()
{
	projection = glm::ortho(0.0f, screenWidth, 0.0f, screenHeight, -1.0f, 1.0f);
	GLint projLoc = glGetUniformLocation(shader->ID, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

void Game::resize(GLFWwindow* window, int w, int h)
{
	width = w;
	height = h;
	screenSizeChanged = true;
	glViewport(0, 0, width, height);
}


