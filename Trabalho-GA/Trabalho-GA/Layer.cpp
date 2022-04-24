#include "Layer.h"

void Layer::adicionarObjeto(float posX,float posY,float posZ,float dimX,float dimY,float dimZ ,Shader* shader)
{
	sprite = new Sprite;
	sprite->setPosition(glm::vec3(posX, posY, posZ));
	sprite->setDimension(glm::vec3(dimX, dimY,dimZ));
	sprite->setShader(shader);
	objects.push_back(sprite);

	sprite->setPosXInicial(posX);
	sprite->setPosYInicial(posY);
	sprite->setPosZInicial(posZ);

	sprite->setPosX(posX);
}

//Os deslocamentos são iguais para todos objetos dentro de uma mesmo camada, aqui o valor desse deslocamento é setado
void Layer::setDesloc(float deslocamento)
{
	desloc = deslocamento;
}

float Layer::getDesloc()
{
	return desloc;
}

void Layer::update()
{
	for (int aux = 0; aux < objects.size(); aux++) {
		objects[aux]->update();
		objects[aux]->draw();
	}
}
