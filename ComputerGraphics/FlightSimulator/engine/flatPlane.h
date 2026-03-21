#pragma once

float flatPlaneVertecies[] =
{
	 1.0f, 0.0f, 1.0f, 1.0f,
	 1.0f, 0.0f,-1.0f, 1.0f,
	-1.0f, 0.0f, 1.0f, 1.0f,

	-1.0f, 0.0f,-1.0f, 1.0f,
	-1.0f, 0.0f, 1.0f, 1.0f,
	 1.0f, 0.0f,-1.0f, 1.0f,

};

int flatPlaneCount = 6;

float flatPlaneTexCoords[] = {
	10.0f,10.0f, 10.0f,0.0f, 0.0f,10.0f,
	0.0f,0.0f, 0.0f,10.0f, 10.0f,0.0f,
};