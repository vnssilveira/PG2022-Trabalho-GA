#include "Camadas.h"



void Camadas::updateCamada()
{
	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->update();
		objects[i]->draw();
	}
}

//Aqui preparamos os Sprites com todas suas coordenadas e j� com a possibilidade de dimensionar as sprites
void Camadas::adicionarObjeto(float posX,float posY,float posZ,float dimX,float dimY,float dimZ ,Shader* shader)
{
	obj = new Sprite;
	obj->setPosition(glm::vec3(posX, posY, posZ));
	obj->setDimension(glm::vec3(dimX, dimY,dimZ)); //note que depois podemos reescalar conforme tamanho da sprite
	obj->setShader(shader);
	objects.push_back(obj);

	obj->setPosXInicial(posX);
	obj->setPosYInicial(posY);
	obj->setPosZInicial(posZ);

	obj->setPosX(posX);
}

//Os deslocamentos s�o iguais para todos objetos dentro de uma mesmo camada, aqui o valor desse deslocamento � setado
void Camadas::setDesloc(float deslocamento)
{
	desloc = deslocamento;
}

float Camadas::getDesloc()
{
	return desloc;
}
