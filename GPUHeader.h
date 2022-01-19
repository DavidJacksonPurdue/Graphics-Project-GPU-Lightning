#pragma once

#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GL/glut.h>
#include <string>

class GPUCom
{
public:
	GPUCom(int w, int h);

	void restart();
	void update();
	void display();
	int loadShader(char* filename, std::string& text);

private:
    int _width;				// width of the screen
	int _height;			// height of the screen
	float _time;
	float _timeAtReset;

public: float mouseloc[2];
public: int colorChoice;
public: float amplitude;
public: float speed;
public: int mouseConfirm;
public: int arcCount;

	GLuint _colorOpLoc;
    GLuint _texUnitLoc;
	GLuint _timeLoc;
	GLuint _mousecomLoc;
	GLuint _mousexLoc;
	GLuint _mouseyLoc;
	GLuint _ampLoc;
	GLuint _speedLoc;
	GLuint _numarcLoc;

    GLuint _textureId;		// The texture ID used to store data array
    GLuint _programId;		// the program ID

    GLuint _fragmentShader;
};

