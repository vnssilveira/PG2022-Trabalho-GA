#include "Game.h"

static GLuint width = 1400, height = 800;

static bool keys[1024];
static bool resized;

const int camada3		  = 0;
const int camada2		  = 1;
const int camada1		  = 2;
const int chao			  = 3;
const int personagem	  = 4;
//const int personagemVolta = 5;
const int personagemPula = 6;

enum Movimento{esquerda, parado, direita, pulando};
static Movimento mov = parado;

void Game::start()
{
	initialize();
	initializeScene();

	while (!glfwWindowShouldClose(window))
	{

		glfwPollEvents();

		update();
		render();

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
	
	resized = true; 

	glEnable(GL_DEPTH);
}

void Game::initializeScene()
{
	//Definindo posições de cada Sprites.
	camadas[camada3].adicionarObjeto(402.0f, 340.0f, 0.0, 800.0f, 550.0f, 0.0f, shader);
	camadas[camada3].adicionarObjeto(1200.0f, 340.0f, 0.0, 800.0f, 550.0f, 0.0f, shader);
	camadas[camada3].setDesloc(-35.0f);//Direção de deslocamento.

	camadas[camada2].adicionarObjeto(402.0f, 340.0f, 0.0, 800.0f, 550.0f, 0.0f, shader);
	camadas[camada2].adicionarObjeto(1200.0f, 340.0f, 0.0, 800.0f, 550.0f, 0.0f, shader);
	camadas[camada2].setDesloc(-50.0f);

	camadas[camada1].adicionarObjeto(402.0f, 340.0f, 0.0, 800.0f, 550.0f, 0.0f, shader);
	camadas[camada1].adicionarObjeto(1200.0f, 340.0f, 0.0, 800.0f, 550.0f, 0.0f, shader);
	camadas[camada1].setDesloc(-65.0f);

	camadas[chao].adicionarObjeto(405.0f, 36.0f, 0.0, 800.0f, 95.0f, 1.0f, shader);
	camadas[chao].adicionarObjeto(1200.0f, 36.0f, 0.0, 800.0f, 95.0f, 1.0f, shader);
	camadas[chao].setDesloc(-65.0f);

	camadas[personagem].adicionarObjeto(50.0f, 140.0f, 0.0f, 60.0f, 120.0f, 0.0f, shader);
	camadas[personagem].setDesloc(0.1f);

	//camadas[personagemVolta].adicionarObjeto(50.0f, 140.0f, 0.0f, 60.0f, 120.0f, 0.0f, shader);
	//camadas[personagemVolta].setDesloc(0.1f);

	//Adicionando Textura para cada Sprites
	unsigned int texID = loadTexture("../textures/cenario1.jpg");
	camadas[camada3].objects[0]->setTexture(texID);
	camadas[camada3].objects[1]->setTexture(texID);

	texID = loadTexture("../textures/cenario1.jpg");
	camadas[camada2].objects[0]->setTexture(texID);
	camadas[camada2].objects[1]->setTexture(texID);

	texID = loadTexture("../textures/cenario1.jpg");
	camadas[camada1].objects[0]->setTexture(texID);
	camadas[camada1].objects[1]->setTexture(texID);

	texID = loadTexture("../textures/chao1.png");
	camadas[chao].objects[0]->setTexture(texID);
	camadas[chao].objects[1]->setTexture(texID);

	texID = loadTexture("../textures/personagem.png");
	camadas[personagem].objects[0]->setTexture(texID);

	texID = loadTexture("../textures/personagemPula.png");
	camadas[personagemPula].objects[0]->setTexture(texID);

	//texID = loadTexture("../textures/personagemVolta.png");
	//camadas[personagemVolta].objects[0]->setTexture(texID);

	//Definindo a janela do mundo (ortho2D)
	ortho2D[0] = 0.0f; //xMin
	ortho2D[1] = 800.0f; //xMax
	ortho2D[2] = 0.0f; //yMin
	ortho2D[3] = 600.0f; //yMax

	//Habilita transparência
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void Game::key_callback(GLFWwindow * window, int key, int scancode, int action, int mode)
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
		mov = direita;
	}
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		mov = esquerda;
	}
	else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		mov = pulando;
	}
	else
	{
		mov = parado;
	}
}

void Game::resize(GLFWwindow * window, int w, int h)
{
	width = w;
	height = h;
	resized = true;

	// Define the viewport dimensions
	glViewport(0, 0, width, height);
}

void Game::update()
{
	if (keys[GLFW_KEY_ESCAPE])
		glfwSetWindowShouldClose(window, GL_TRUE);
	//AQUI APLICAREMOS TRANSFORMAÇÕES NOS SPRITES//

	//Controle de movimentação do personagem
	switch (mov)
	{
	case esquerda:
		camadas[personagem].objects[0]->setPosX(camadas[personagem].objects[0]->getPosX() - 0.3f);
		break;
	case direita:
		camadas[personagem].objects[0]->setPosX(camadas[personagem].objects[0]->getPosX() + 0.3f);
		break;
	case pulando:
		//camadas[personagem].objects[0]->setPosX(camadas[personagem].objects[0]->getPosX() + 0.3f);
	default:
		break;
	}
	camadas[personagem].objects[0]->setPosition(glm::vec3(camadas[personagem].objects[0]->getPosX(), camadas[personagem].objects[0]->getPosYInicial(), camadas[personagem].objects[0]->getPosZInicial()));

	for (int i = 0; i < 4; i++)
	{
		//Movimentação constante das camadas do parallax
		camadas[i].objects[0]->setPosition(glm::vec3(camadas[i].objects[0]->getPosXInicial() + camadas[i].objects[0]->tempo * camadas[i].getDesloc(), camadas[i].objects[0]->getPosYInicial(), camadas[i].objects[0]->getPosZInicial()));
		camadas[i].objects[1]->setPosition(glm::vec3(camadas[i].objects[1]->getPosXInicial() + camadas[i].objects[1]->tempo * camadas[i].getDesloc(), camadas[i].objects[1]->getPosYInicial(), camadas[i].objects[1]->getPosZInicial()));
		//Adição na variação de tempo dentro da variável que todos objetos possuem
		camadas[i].objects[0]->tempo += 0.001f;
		camadas[i].objects[1]->tempo += 0.001f;

		//Atualização da posição dos sprites que formam o parallax
		if (camadas[i].objects[0]->getPosX() <= -400.0f)
		{

			camadas[i].objects[0]->setPosXInicial(1190.0f);
			camadas[i].objects[0]->tempo = 0;

		}
		if (camadas[i].objects[1]->getPosX() <= -400.0f)
		{
			camadas[i].objects[1]->tempo = 0;
			camadas[i].objects[1]->setPosXInicial(1190.0f);
		}
	}
}

void Game::render()
{
	// Clear the colorbuffer
	glClearColor(0.89f, 0.89f, 0.87f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	if (resized) //se houve redimensionamento na janela, redefine a projection matrix
	{
		setupCamera2D();
		resized = false;
	}

	//atualiza e desenha os Sprites

	for (int i = 0; i < 6; i++)
	{
		camadas[i].updateCamada();
	}
	
}


void Game::setupCamera2D() //TO DO: parametrizar aqui
{
	float zNear = -1.0, zFar = 1.0; //estão fixos porque não precisamos mudar

	projection = glm::ortho(ortho2D[0], ortho2D[1], ortho2D[2], ortho2D[3], zNear, zFar);

	//Obtendo o identificador da matriz de projeção para enviar para o shader
	GLint projLoc = glGetUniformLocation(shader->ID, "projection");
	//Enviando a matriz de projeção para o shader
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
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


