#pragma once
#include "Object.h"
#include "VectorMath.h"
#include <vector>
#include "Wave.h"


class Sea :
	public Object
{
public:
	Sea(Camera* cam);
	virtual void SetVertices();
	virtual void Draw();
	virtual void Init(GLuint buffer, GLuint buffStart);
	~Sea();
protected:
	void InitWaves();
	float time;
	/* protected index buffer */
	GLuint indexbuff;
	std::vector<GLuint> indices;
	waveVec waves[WAVE_NUM];
	
};

