#pragma once
#include "Main.h"
#include "Scene.h"
#include "Camera.h"
#include "Sea.h"
#include <string>


/* Set up draw callback */
Scene* currentScene;

extern "C"
void DrawCallback()
{
	cout << "Called" << currentScene << endl;
	currentScene->Draw();
}

void InputHandler(unsigned char key, int state, int x)
{
	currentScene->Input(key, state, x);
}

Main::Main()
{
}


Main::~Main()
{
}


void checkErr()
{
	GLint ecode = glGetError();
	if (ecode != GL_NO_ERROR)
	{
		cout << "[Error] OpenGL error" << endl;
		cout << "[Error Code] " << ecode << endl;
	}
	else
		cout << "[*] Buffers bound" << endl;
}


/* MAIN ENTRY */
int main(int argc, char** argv)
{
	float winx = 512; float winy = 512;
	Scene scene(winx, winy);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(winx, winy);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow(argv[0]);
	
	if (GLEW_OK != glewInit())
	{
		cout << "GLEW Failed to Load" << endl;
		exit(-1);
	}
	else
		cout << "GLEW loaded" << endl;


	checkErr();

	glEnable(GL_DEPTH_TEST);

	glutKeyboardFunc(InputHandler);

	checkErr();

	/* can't just use pointer to a member function */
	scene.Init();
	checkErr();
	currentScene = &scene;
	checkErr();
	scene.Add(new Sea(scene.giveCam()));
	checkErr();
	glutDisplayFunc(DrawCallback);
	checkErr();
	try { glutMainLoop(); }
	catch (exception e)
	{
		cout << "Error " << endl;
		GLenum err = glGetError();
		string mess;
		switch(err) 
		{
			case GL_INVALID_OPERATION:      mess="INVALID_OPERATION";      break;
			case GL_INVALID_ENUM:           mess="INVALID_ENUM";           break;
			case GL_INVALID_VALUE:          mess="INVALID_VALUE";          break;
			case GL_OUT_OF_MEMORY:          mess="OUT_OF_MEMORY";          break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:  mess="INVALID_FRAMEBUFFER_OPERATION";  break;
			default: mess = "UNDEFINED";  break;
        }

		cout << "[Error] " << mess << endl;

	}

	getchar();

	return 0;
}

void initshader()
{

}
