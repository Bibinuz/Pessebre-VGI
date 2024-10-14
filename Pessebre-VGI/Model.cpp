#include "Model.h"


void Model::loadObj(const char* file)
{
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	std::vector<glm::vec3> tempVert;
	std::vector<glm::vec3> tempNor;
	std::vector<glm::vec2> tempTex;
	std::vector<int> tempIndex;
	std::vector<int> tempTexIndex;
	std::vector<int> tempNorIndex;


	std::ifstream in(file, std::ios::in);
	if (!in)
	{
		std::cerr << "Cannot open " << file << std::endl;
		exit(1);

	}

	std::string line;
	while (std::getline(in, line))
	{

		//check v for vertices
		if (line.substr(0, 2) == "v ") {
			std::istringstream v(line.substr(2));
			glm::vec3 vert;
			double x, y, z;
			v >> x; v >> y; v >> z;
			vert = glm::vec3(x, y, z);
			tempVert.push_back(vert);
		}
		//check for normal vector
		else if (line.substr(0, 2) == "vn") {

			std::istringstream v(line.substr(3));
			glm::vec3 nor;
			double x, y, z;
			v >> x; v >> y, v >> z;
			nor = glm::vec3(x, y, z);
			tempNor.push_back(nor);

		}

		//check for texture co-ordinate
		else if (line.substr(0, 2) == "vt") {

			std::istringstream v(line.substr(3));
			glm::vec2 tex;
			int U, V;
			v >> U; v >> V;
			tex = glm::vec2(U, V);
			tempTex.push_back(tex);

		}
		//check for faces
		else if (line.substr(0, 2) == "f ") {
			int a, b, c; //to store mesh index
			int an, bn, cn; //to store normals
			int A, B, C; //to store texture index
			//std::istringstream v;
			//v.str(line.substr(2));
			const char* chh = line.c_str();
			sscanf(chh, "f %i/%i/%i %i/%i/%i %i/%i/%i\n", &a, &A, &an, &b, &B, &bn, &c, &C, &cn); //here it read the line start with f and store the corresponding values in the variables

			a--; b--; c--;
			A--; B--; C--;
			an--; bn--; cn--;
			tempIndex.push_back(a); tempTexIndex.push_back(A); tempNorIndex.push_back(an);
			tempIndex.push_back(b); tempTexIndex.push_back(B); tempNorIndex.push_back(bn);
			tempIndex.push_back(c); tempTexIndex.push_back(C); tempNorIndex.push_back(cn);
		}

	}

	for (unsigned int i = 0; i < tempIndex.size(); i++)
	{
		Vertex vert = { tempVert[tempIndex[i]], tempNor[tempNorIndex[i]], tempTex[tempTexIndex[i]] };
		vertices.push_back(vert);
		indices.push_back(i);
	}

	objecte = new Mesh(vertices, indices, textures);
};
