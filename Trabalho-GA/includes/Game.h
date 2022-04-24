#pragma once

#include "Shader.h"
#include "Sprite.h" 
#include "Layer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

using namespace std;

class Game
{
public:
	
	void start();

	unsigned int loadTexture(string filename);

private:

	void initialize();
	void initializeScene();
	void renderSprites();
	static void resize(GLFWwindow* window, int width, int height);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	void setupCamera2D();
	void updateSprites();

	float movimento;
	Layer* layers = new Layer[3];
	
	GLFWwindow *window;
	Shader *shader;

	glm::mat4 projection;
	
	vector <Sprite*> objects;

};

