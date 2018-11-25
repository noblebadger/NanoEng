#pragma once
/* from glm */
#include <gtc/matrix_transform.hpp>
#include <glm.hpp>

#include <ext.hpp>

class Camera
{
public:
	Camera();
	~Camera();

	inline void setUpViewProjection()
	{
		/* FROM: http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/ */
		proj = glm::perspective(glm::radians(45.0f), (float)512/512, 0.1f, 100.0f);
		view = glm::lookAt(pos, target, upDir);
		model = glm::mat4(1);
		mvp = proj*view*model;
	}

	inline glm::mat4 getMVP() { return mvp; }

	void moveCam(char key);

protected:

	glm::mat4 view;
	glm::mat4 proj;
	glm::mat4 model;
	glm::vec3 pos;
	glm::vec3 upDir;
	glm::vec3 target;
	glm::vec3 right;
	glm::mat4 mvp;

};

