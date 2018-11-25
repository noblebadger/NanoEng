#include "Sea.h"
#include <chrono>

Sea::Sea(Camera* cam)
	: Object(cam)
{
	vShaderName = "sea.vert";
	fShaderName = "sea.frag";
	InitWaves();
	vNum = 40;
}


void Sea::Init(GLuint buffer, GLuint buffStart)
{
	Object::Init(buffer, buffStart);

	/* Now to set our uniform array */

	string prefix("waveVec[");
	GLuint loc = glGetUniformLocation(shaderProg, "waveNum");
	glUniform1i(loc, WAVE_NUM);

	for (int i=0; i < WAVE_NUM; i++)
	{
		string num = std::to_string(i); 
		string end("].dir");
		GLuint loc = glGetUniformLocation(shaderProg, (prefix+num+end).c_str());
		glUniform2fv(loc, 2, waves[i].d);
		end = "].wl";
		loc = glGetUniformLocation(shaderProg, (prefix+num+end).c_str());
		glUniform1f(loc, waves[i].wl);
		end = "].spd";
		loc = glGetUniformLocation(shaderProg, (prefix+num+end).c_str());
		glUniform1f(loc, waves[i].spd);
		end = "].stp";
		loc = glGetUniformLocation(shaderProg, (prefix+num+end).c_str());
		glUniform1f(loc, waves[i].stp);
		end = "].a";
		loc = glGetUniformLocation(shaderProg, (prefix+num+end).c_str());
		glUniform1f(loc, waves[i].a);
	}

	loc = glGetUniformLocation(shaderProg, "time");
	glUniform1f(loc, time);
}

/** Creates and stores plane verts **/
void Sea::SetVertices()
{
	int columns = 8;
	int rows = 8;
	float width = 0.05;
	int vpos = 0;

	/* Create a normal grid */
	for (int r = 0; r < rows; r++) 
	{
		float z = width * r;
        for (int c = 0; c < columns; c++) 
		{
			float x = width * c;
			verts.push_back(x);
			verts.push_back(0);
			verts.push_back(z);
        }
    }

	/* Init index buffer */
	GLuint indexbuff;
	glGenBuffers(1, &indexbuff);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuff);

	/* Create a normal grid */
	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < columns; c++)
		{
			int start = r * columns+ c; // bottom left
			/* for each triangle in square */
			for (int i = 0; i < 2; i++)
			{
				/*Triangle*/
				if (i == 0) // even
				{
					indices.push_back(start);
					indices.push_back(start + 1);
					indices.push_back(start + columns);
				}
				else // odd
				{
					indices.push_back(start + 1);
					indices.push_back(start + 1 + columns);
					indices.push_back(start + columns);
				}
			}
		}
	}

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
	vNum = verts.size();
	vertSize = verts.size() * sizeof(GLfloat);
}

void Sea::Draw()
{


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuff);
	glDrawElements(GL_TRIANGLE_STRIP, indices.size(), GL_UNSIGNED_INT, &indices[0]);
	glFinish();

	// Get elapsed time (milliseconds)
	int t = glutGet(GLUT_ELAPSED_TIME);
	// Calculate difference in seconds between previous and current call 
	time = static_cast<double>(t) / 1000.0;
}

/** This function generates a WAVE_NUM wave
parameters. **/
void Sea::InitWaves()
{
	float r_val;
	float r_val1;
	float r_val2;
	for (int i = 0; i < WAVE_NUM; i++)
	{
		r_val = ((float)rand() / (float)RAND_MAX);// creates values 0-1
		r_val1 = ((float)rand() / (float)RAND_MAX);
		r_val2 = ((float)rand() / (float)RAND_MAX);
		waves[i].wl = r_val + rand() % 2;  // small wavelength makes less pronouned waves
		waves[i].a = (r_val1)+rand() % 1 + r_val2; // amplitue controls the max height 
		waves[i].stp = 0.01;
		waves[i].spd = (r_val1)+rand() % 1 + 1;
		r_val1 = rand() / (float)RAND_MAX;
		if (rand() % 2 == 0) r_val1 *= -1;
		waves[i].d[0] = r_val1;
		r_val = rand() / (float)RAND_MAX;
		if (rand() % 2 == 0) r_val *= -1;
		waves[i].d[1] = r_val;
		cout << "Wave created with " << "amplitude: " << waves[i].a;
		cout << " speed: " << waves[i].spd << " steepness: " << waves[i].stp;
		cout << " wavelength: " << waves[i].wl << " direction " << waves[i].d[0];
		cout << "," << waves[i].d[1] << endl;
	}
}

Sea::~Sea()
{
}