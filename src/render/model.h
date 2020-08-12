#include "C:/Users/panih/source/repos/Engine/Engine/include/glm/glm/gtc/matrix_transform.hpp"
#include "C:/Users/panih/source/repos/Engine/Engine/include/glm/glm/gtc/type_ptr.hpp"
#include "C:/Users/panih/source/repos/Engine/Engine/include/glm/glm/glm.hpp"
#include "../include.h"

struct  t_vertex
{
	glm::vec3 pos;
	glm::vec3 norm;
	glm::vec2 text;
};

struct t_triangle
{
	int index[3];
};

struct t_model
{
	struct vertex {
		t_vertex v;
		//mb name_model
	};

	struct triangle {
		int index[3];
		//material
	};

	vector<vertex> vert;
	vector<triangle> tri;

	int curent_shader;

	void load_obj(std::string path);
};