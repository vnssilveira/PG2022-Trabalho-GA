#pragma once

#include "Shader.h"
#include "Sprite.h" 
#include "Camadas.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

using namespace std;

class Game
{
public:
	
	void start();

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	static void resize(GLFWwindow* window, int width, int height);

	void update();
	void render();

	void setupCamera2D();
	unsigned int loadTexture(string filename);
	
	float movimento;

private:

	void initialize();
	void initializeScene();
	
	GLFWwindow *window;
	Shader *shader;

	glm::vec4 ortho2D; 
	glm::mat4 projection;
	Camadas* camadas = new Camadas[6];
	vector <Sprite*> objects;

};

