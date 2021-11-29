#include "lab_m1/Tema1/Shooter.h"

//const GLint numberOfSides = 360;
//
// void drawCircle(GLfloat x, GLfloat y, GLfloat z, GLfloat radius) {
//	const GLint numberOfVertices = numberOfSides + 2;
//	GLfloat doublePi = 1.0f * M_PI;
//
//	GLfloat circleVerticesX[numberOfVertices];
//	GLfloat circleVerticesY[numberOfVertices];
//	GLfloat circleVerticesZ[numberOfVertices];
//
//	circleVerticesX[0] = x;
//	circleVerticesY[0] = y;
//	circleVerticesZ[0] = z;
//
//	for (int i = 0; i < numberOfVertices; i++)
//	{
//		circleVerticesX[i] = x + radius * cos(i * doublePi / numberOfSides);
//		circleVerticesY[i] = y + radius * sin(i * doublePi / numberOfSides);
//		circleVerticesZ[i] = z;
//	}
//	GLfloat allVertices[numberOfVertices * 3];
//	for (int i = 0; i < numberOfVertices; i++)
//	{
//		circleVerticesX[i * 3] = circleVerticesX[i];
//		circleVerticesY[i * 3 + 1] = circleVerticesY[i];
//		circleVerticesZ[i * 3 + 1] = circleVerticesZ[i];
//	}
//
//	glEnableClientState(GL_VERTEX_ARRAY);
//
//	glVertexPointer(3, GL_FLOAT, 0, allVertices);
//	glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices);
//
//	glDisableClientState(GL_VERTEX_ARRAY);
//}