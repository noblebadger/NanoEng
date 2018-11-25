#include "Object.h"
#include <ext.hpp>

Object::Object(Camera* cam)
	: vShaderName("shader.vert"), fShaderName("shader.frag"), shaderProg(0), vNum(6)
{
	currentCam = cam;
}

/** Init() loads your vertcies into the shared buffer 
    ready to display your object upon the next 
	call of draw() **/
void Object::Init(GLuint buffer, GLuint buffStart)
{ 
	/* buffer */
	this->buffer = buffer;

	/* Vertices */
	SetVertices();
	SetFaceNormals();
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	glBufferData(GL_ARRAY_BUFFER, vertSize, NULL, GL_STATIC_DRAW);

	/* Vertices */
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertSize, &verts[0]);
	/* Normals */
//	glBufferSubData(GL_ARRAY_BUFFER, buffStart+vertSize, normSize, faceNormals);

	LoadShader();
}

/** This function decides how to draw the object according to
    the openGL primitives. It also enables an object specifc
	shaders if they are present.
**/
void Object::Draw()
{
//	if (shaderProg) { glUseProgram(shaderProg); }
	glMatrixMode(GL_MODELVIEW);
	// gl clear very important
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDrawArrays(GL_TRIANGLES, 0, vNum);
	glFinish();

}

void Object::QueryBuffer()
{
	GLfloat* data = new GLfloat[vNum * 3];
	cout << "BUFFER CONTENTS  " << endl;
	glGetBufferSubData(GL_ARRAY_BUFFER, 0, 3*vNum*sizeof(GLfloat), data);
	for (int i = 0; i < 3*vNum; i++)
		cout << data[i] << " " << endl;
	delete[] data;
}

bool Object::LoadShader()
{

	GLuint vertShader = 0;
	GLuint fragShader = 0;
	GLint compflag = 0;
	fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	vertShader = glCreateShader(GL_VERTEX_SHADER);

	if (fragShader == 0 || vertShader == 0)
	{
		cout << "[Error] Shader could not be created" << endl;
		exit(-1);
	}
	else
		cout << "[SHADER] Shader Objects Created" << endl;
	
	string path = "Shaders/";
	string fullName = path + fShaderName;
	ifstream infilef(fullName, ios_base::in);
	fullName = path + vShaderName;
	ifstream infilev(fullName, ios_base::in);

	if (infilev.fail() || infilef.fail())
	{
		cout << "[Error] Could not find shader files" << endl;
		getchar();
		exit(-1);
	}
	else
		cout << "Shaderfile found" << endl;

	infilev.seekg(0, ifstream::end);
	infilef.seekg(0, ifstream::end);
	int sizev = infilev.tellg();
	int sizef = infilev.tellg();
	string line, textf, textv;
	infilev.seekg(0);
	infilef.seekg(0);


	cout << "Printing Fragment Shader" << endl;


	while (getline(infilef, line))
		textf += line + "\n";

	cout << textf << endl;
	cout << "Printing Vertex Shader" << endl;


	while (getline(infilev, line))
		textv += line + "\n";

	cout << textv << endl;

	const GLchar* shader_codef = (GLchar*)textf.c_str();
	const GLchar* shader_codev = (GLchar*)textv.c_str();

	glShaderSource(fragShader, 1, &shader_codef, NULL);
	glShaderSource(vertShader, 1, &shader_codev, NULL);
	glCompileShader(fragShader);
	glCompileShader(vertShader);

	int loglenf = 0; int loglenv = 0;
	int compflagv = 0; int compflagf = 0;

	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &compflagf);
	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &compflagv);
	glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &loglenf);
	glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &loglenv);

	if (compflagf & compflagv == GL_TRUE)
	{
		cout << "Shaders Complied Successfully" << endl;
		shaderProg = glCreateProgram();
		glAttachShader(shaderProg, vertShader);
		glAttachShader(shaderProg, fragShader);
		glLinkProgram(shaderProg);
		glUseProgram(shaderProg);
		GLuint pos = glGetAttribLocation(shaderProg, "position");
		/*
		Args
		GLuint index: The location value for the input var
		GLuint size: number of values for each vertex in array
		GLenum type: vertex values type
		GLboolean normalized: in -1,1 domain?
		GLsizei stride: contiguous (0) or spacing between values (> 0)
		const GLvoid *point: buffer_offset
		*/
		glVertexAttribPointer(pos, 3, GL_FLOAT, GL_TRUE, 0, 0);
		glEnableVertexAttribArray(pos);
		GLuint pos1 = glGetAttribLocation(shaderProg, "normals");
		glVertexAttribPointer(pos1, 3, GL_FLOAT, GL_TRUE, 0, 0);
		glEnableVertexAttribArray(pos1);
		std::cout<<glm::to_string(currentCam->getMVP())<<std::endl;
		GLuint mvpid = glGetUniformLocation(shaderProg, "mvp");
		glUniformMatrix4fv(mvpid, 1, GL_FALSE, &currentCam->getMVP()[0][0]);
	
	}
	else
	{
		/* prints out complier warning for shaders */
		cout << "Shader Failed to Compile" << endl;
		GLchar* logf = new char[loglenf];
		GLchar* logv = new char[loglenv];
		memset(logf, 0, loglenf);
		memset(logf, 0, loglenv);
		glGetShaderInfoLog(vertShader, loglenv, 0, logv);
		glGetShaderInfoLog(fragShader, loglenf, 0, logf);
		cout << "VERTEX LOG: " << logv << endl;
		cout << "FRAGMENT LOG: " << logf << endl;
		getchar();
		exit(-1);
	}
	infilef.close();
	infilev.close();
}

/*
	Sets the face Normals. Must be implemented by
	subclass. MUST BE CALLED AFTER setvertices
*/
void Object::SetFaceNormals()
{
}

/* 
   This function loads the vertices
   into memory however the subclass
   decides - it must be implemented 
*/
void Object::SetVertices()
{
}

/*
	cos(theta)=|a.b| !
	returns angle in radians
*/
float Object::vecs_to_angle(float u[3], float v[3])
{
	float angle = 0;
	float product = v[0] * u[0] + v[1] * u[1] + v[2] * u[2];
	product = abs(product);
	angle = acos(product);
	return angle;
}

Object::~Object()
{
	delete[] faceNormals;
}
