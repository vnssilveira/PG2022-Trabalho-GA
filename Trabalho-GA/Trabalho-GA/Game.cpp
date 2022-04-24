#include "Game.h"

static const float screenWidth = 1400.0f, screenHeight = 800.0f;
static const float maxScreenWidth = 10;
static float frameWidth = 0.5;
static GLuint width = screenWidth, height = screenHeight;

static bool keys[1024];
static bool screenSizeChanged;
bool isJumping = false;
bool raise = true;

const int backgroud	= 0;
const int character	= 1;
const int plant = 2;

enum Movement{LEFT, STOP, RIGHT, JUMPING};
static Movement movement = STOP;
const float characterSpeed = 0.35f;

void Game::start()
{
	initialize();
	initializeScene();

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		updateSprites();
		renderSprites();

		glfwSwapBuffers(window);
	}
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

	shader = new Shader(((string) "../shaders/sprite.vs").c_str(), ((string)"../shaders/sprite.fs").c_str());
	
	screenSizeChanged = true;

	glEnable(GL_DEPTH);
}

void Game::initializeScene()
{
	unsigned int texID = loadTexture("../textures/cenario.jpg");

	for (int aux = 0; aux < maxScreenWidth; aux++) {
		layers[backgroud].adicionarObjeto(screenWidth * frameWidth, screenHeight / 2, 0.0, screenWidth, screenHeight, 0.0f, shader);
		layers[backgroud].objects[aux]->setTexture(texID);

		frameWidth += 1.0;
	}
	layers[backgroud].setDesloc(-20.0f);


	layers[character].adicionarObjeto(50.0f, 240.0f, 0.0f, 100.0f, 160.0f, 0.0f, shader);
	layers[character].setDesloc(0.2f);
	texID = loadTexture("../textures/personagem.png");
	layers[character].objects[0]->setTexture(texID);
	layers[character].objects[0]->tempo = 100.0f;

	frameWidth = 0.5;
	for (int aux = 0; aux < maxScreenWidth; aux++) {
		texID = loadTexture("../textures/plant.gif");
		layers[plant].adicionarObjeto(screenWidth * frameWidth, 240.0f, 0.0f, 100.0f, 160.0f, 0.0f, shader);
		layers[plant].objects[aux]->setTexture(texID);

		frameWidth += 1.0;
	}
	layers[plant].setDesloc(-20.0f);

	//texID = loadTexture("../textures/personagemPula.png");
	//camadas[personagemPula].objects[0]->setTexture(texID);

	//texID = loadTexture("../textures/personagemVolta.png");
	//camadas[personagemVolta].objects[0]->setTexture(texID);

	

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Game::updateSprites()
{
	if (keys[GLFW_KEY_ESCAPE])
		glfwSetWindowShouldClose(window, GL_TRUE);

	switch (movement)
	{
	case LEFT:
		if (layers[character].objects[0]->getPosX() > 50)
		layers[character].objects[0]->setPosX(layers[character].objects[0]->getPosX() - characterSpeed);
		break;
	case RIGHT:
		if(layers[character].objects[0]->getPosX() < screenWidth - 50)
		layers[character].objects[0]->setPosX(layers[character].objects[0]->getPosX() + characterSpeed);
		break;
	case JUMPING:
		if (!isJumping)
			isJumping = true;
	default:
		break;
	}

	layers[character].objects[0]->setPosition(glm::vec3(layers[character].objects[0]->getPosX(), layers[character].objects[0]->getPosY(), layers[character].objects[0]->getPosZInicial()));
	
	//Andar cenário
	for (int aux = 0; aux < maxScreenWidth; aux++) {
		
		layers[backgroud].objects[aux]->setPosition(glm::vec3(layers[backgroud].objects[aux]->getPosXInicial() + layers[backgroud].objects[aux]->tempo * layers[backgroud].getDesloc(), layers[backgroud].objects[aux]->getPosYInicial(), layers[backgroud].objects[aux]->getPosZInicial()));
		layers[backgroud].objects[aux]->tempo += 0.01f;
	}

	//Andar inimigo
	for (int aux = 0; aux < maxScreenWidth; aux++) {

		layers[plant].objects[aux]->setPosition(glm::vec3(layers[plant].objects[aux]->getPosXInicial() + layers[plant].objects[aux]->tempo * layers[plant].getDesloc(), layers[plant].objects[aux]->getPosYInicial(), layers[plant].objects[aux]->getPosZInicial()));
		layers[plant].objects[aux]->tempo += 0.01f;
	}

	layers[character].objects[0]->tempo += 0.01f;

	//Pular
	if (isJumping && layers[character].objects[0]->tempo >= 5.0) {

		if (raise) {
			layers[character].objects[0]->setPosY(layers[character].objects[0]->getPosY() + 0.5f);
		}
		else {
			layers[character].objects[0]->setPosY(layers[character].objects[0]->getPosY() - 0.3f);
		}

		if(layers[character].objects[0]->getPosY() > 450) {
			raise = false;
		} else if (layers[character].objects[0]->getPosY() < 240) {
			isJumping = false;
			raise = true;
			layers[character].objects[0]->tempo = 0.0f;
		} 
	}

	if(layers[character].objects[0]->getPosY() == )
	
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
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		movement = RIGHT;
	}
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		movement = LEFT;
	}
	else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		movement = JUMPING;
	}
	else
	{
		movement = STOP;
	}
}

unsigned int Game::loadTexture(string filename)
{
	unsigned int texture;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); 
										  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	
	int width, height, nrChannels;
	
	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
	
	if (data)
	{
		if (nrChannels == 3) 
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else 
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


