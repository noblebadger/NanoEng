#pragma once
#include "Resource.h"
#include <string>
#include <iostream>
#include <fstream>
#include "Camera.h"
#include <vector>

using namespace std;


class Object
{
	public:
		Object(Camera* cam);
		virtual ~Object();
		virtual void Init(GLuint buffer, GLuint buffStart);
		virtual void QueryBuffer();
		virtual bool LoadShader();
		virtual void Draw();
		inline GLuint DataSize() { return vertSize + normSize; };
		inline GLuint ShaderProg() { return shaderProg; };
	protected:
		Camera* currentCam;
		/* override these two for different verts */
		virtual void SetVertices();
		virtual void SetFaceNormals();
		/* -------------------------------------- */
		float vecs_to_angle(float u[3], float v[3]);
		inline float toDegrees(float rad) { return rad * (180.0 / M_PI); }
		inline float toRadians(float deg) { return deg * (M_PI / 180.0); }
		string vShaderName;
		string fShaderName;
		std::vector<GLfloat> verts;
		GLuint buffer;
		GLfloat *faceNormals;
		/* vNum the number of vertex arrays*/
		GLuint vNum;
		/* vertSize and normSize the number
		   of bytes of the vertex arrays
		*/
		GLuint vertSize;
		GLuint normSize;
		GLuint shaderProg;
		/* holds printout of shader code */
		GLchar* shader_codef;
		GLchar* shader_codev;
};

