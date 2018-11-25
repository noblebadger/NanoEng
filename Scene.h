#pragma once
#include "Resource.h"
#include "Object.h"
#include <vector>
#include <glm.hpp>
#include "Camera.h"
/* GLOBAL DATA STRUCTURES */
#define NUM_ARRAYS 2
#define NUM_BUF 2
#define NUM_OBJS 2


class Scene
{

public:
	Scene() = default;
	Scene(float winx, float winy);
	void Init();
	void Add(Object * obj);
	void Input(unsigned char key, int state, int x);
	inline Camera* giveCam() { return currentCam; };
	void Draw();
	~Scene();
protected:
	Camera* currentCam;
	vector<Object*> drawables;
	unsigned int oCount;
	GLuint vertexArrays[NUM_ARRAYS];
	/* Buffers */
	GLuint buffers[NUM_BUF];
	GLuint num_vertices;
	GLfloat* ambient;
	GLfloat* light_dir;
	/* Size in bytes of all data
	   being used*/
	GLuint currDataSize;

};


