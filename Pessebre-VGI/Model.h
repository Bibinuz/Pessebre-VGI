#ifndef MODEL_CLASS_H
#define MODEL_CLASS_H

#include"Mesh.h"

class Model
{
public:

	Mesh* objecte;
	int n_objectes;

	Model() { n_objectes = 0; objecte = NULL; };
	void loadObj(const char* file);

};
#endif

