#include "Scene.h"


Scene::Scene(float winx, float winy)
	: oCount(0), drawables(), vertexArrays{0}, buffers{0}
{
	currentCam = new Camera();
}

/* Static method to handle input */
void Scene::Input(unsigned char key, int state, int x)
{
	currentCam->moveCam(key);
	std::cout << glm::to_string(currentCam->getMVP()) << std::endl;

	/* bodge change this */
	GLuint mvpid = glGetUniformLocation(drawables[0]->ShaderProg(), "mvp");
	glUniformMatrix4fv(mvpid, 1, GL_FALSE, &currentCam->getMVP()[0][0]);
	Draw();

}

void Scene::Init()
{
	/* uniforms */ 
	ambient = new GLfloat[4]{ (GLfloat)0.9,  (GLfloat)0.9, (GLfloat)0.9,  (GLfloat)1.0 };
	light_dir = new GLfloat[3] { (GLfloat)-0.5, (GLfloat)-0.5, (GLfloat)0.0 };
	/* camera vars */
	

	/* need to register vertex array objects */
	glGenVertexArrays(NUM_ARRAYS, vertexArrays);
	glGenBuffers(NUM_BUF, buffers);
	glBindVertexArray(vertexArrays[0]);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);


}

void Scene::Add(Object* obj)
{
	glBindVertexArray(vertexArrays[0]);
	obj->Init(buffers[0], 0);
	if (oCount < NUM_OBJS)
	{
		drawables.push_back(obj);
		currDataSize += obj->DataSize();
		oCount++;
	}
}


void Scene::Draw()
{
	for (int i = 0; i < oCount; i++)
		drawables[i]->Draw();
	glFinish();
}


Scene::~Scene()
{
	drawables.clear();
	delete[] ambient;
	delete[] light_dir;
	delete currentCam;
}



