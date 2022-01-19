//OPTION A WAVE SIMULATION

#include "GPUHeader.h"
#include <iostream>
#include <fstream>

GPUCom::GPUCom(int w, int h) : _width(w), _height(h)
{
    // Create a texture to store the framebuffer
    glGenTextures(1, &_textureId);
    glBindTexture(GL_TEXTURE_2D, _textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _width, _height, 0, GL_RGB, GL_FLOAT, 0);

    _programId = glCreateProgram();

	// Load fragment shader which will be used as computational kernel
	std::string edgeFragSource2;
	loadShader("fragment.glsl", edgeFragSource2);

    // Create the fragment program
    _fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* source = edgeFragSource2.c_str();
    glShaderSource(_fragmentShader, 1, &source, NULL);
    glCompileShader(_fragmentShader);
    glAttachShader(_programId, _fragmentShader);

    // Link the shader into a complete GLSL program.
    glLinkProgram(_programId);

	int infologLength1 = 0;
	glGetShaderiv(_fragmentShader, GL_INFO_LOG_LENGTH, &infologLength1);
	if (infologLength1 > 0) {
		char *infoLog1 = (char *)malloc(infologLength1);
		glGetShaderInfoLog(_fragmentShader, infologLength1, NULL, infoLog1);
		printf("%s\n", infoLog1);
		free(infoLog1);
	}
	// Check program
	{
		int infologLength = 0;
		glGetProgramiv(_programId, GL_INFO_LOG_LENGTH, &infologLength);
		if (infologLength > 0) {
			char *infoLog = (char *)malloc(infologLength);
			glGetProgramInfoLog(_programId, infologLength, NULL, infoLog);
			printf("%s\n",infoLog);
			free(infoLog);
		}
	}

    // Get location of the uniform variables
    _texUnitLoc = glGetUniformLocation(_programId, "texUnit");
    _timeLoc = glGetUniformLocation(_programId, "time");
	_mousecomLoc = glGetUniformLocation(_programId, "mousecom");
	_mousexLoc = glGetUniformLocation(_programId, "mousex");
	_mouseyLoc = glGetUniformLocation(_programId, "mousey");
    _ampLoc = glGetUniformLocation(_programId, "amp");
	_speedLoc = glGetUniformLocation(_programId, "speed");
    _numarcLoc = glGetUniformLocation(_programId, "numarcs");
    _colorOpLoc = glGetUniformLocation(_programId, "colorOp");

    mouseConfirm = 0;
    mouseloc[0] = 0;
    mouseloc[1] = 0;
    amplitude = 5.0;
    speed = 0.0005;
    arcCount = 3;
    colorChoice = 1;

	_timeAtReset = 0;
}

void GPUCom::restart()
{
    _timeAtReset = _time;
}

void GPUCom::update()
{
    // Backup the viewport dimensions
    int vp[4];
    glGetIntegerv(GL_VIEWPORT, vp);

    glViewport(0, 0, _width, _height);
        
    // Copy the results to the texture
	glBindTexture(GL_TEXTURE_2D, _textureId);

	glUseProgram(_programId);
            
    // Set the uniform variables
    glUniform1i(_texUnitLoc, 0);
    _time = glutGet(GLUT_ELAPSED_TIME);
	_time -= _timeAtReset;
    glUniform1f(_timeLoc, _time);
	glUniform1i(_mousecomLoc, mouseConfirm);
	glUniform1f(_mousexLoc, mouseloc[0]);
	glUniform1f(_mouseyLoc, mouseloc[1]);
	glUniform1f(_ampLoc, amplitude);
    glUniform1f(_speedLoc, speed);
    glUniform1i(_numarcLoc, arcCount);
    glUniform1i(_colorOpLoc, colorChoice);
	// By drawing a quad, the fragment shader will be called for each pixel.
    glBegin(GL_QUADS);
    {            
        glTexCoord2f(0, 0); glVertex3f(-1, -1, -0.5f);
        glTexCoord2f(1, 0); glVertex3f( 1, -1, -0.5f);
        glTexCoord2f(1, 1); glVertex3f( 1,  1, -0.5f);
        glTexCoord2f(0, 1); glVertex3f(-1,  1, -0.5f);
    }
    glEnd();

    glUseProgram(0);
        
    // Copy the rendered image to the texture so that we can start from the current state in the next iteration
    glBindTexture(GL_TEXTURE_2D, _textureId);
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, _width, _height);
        
    // Restore the stored viewport dimensions
	glViewport(vp[0], vp[1], vp[2], vp[3]);
}

void GPUCom::display()
{
    // Bind the texture
    glBindTexture(GL_TEXTURE_2D, _textureId);
    glEnable(GL_TEXTURE_2D);

    // Render the result image.
    glBegin(GL_QUADS);
    {
        glTexCoord2f(0, 0); glVertex3f(-1, -1, -0.5f);
        glTexCoord2f(1, 0); glVertex3f( 1, -1, -0.5f);
        glTexCoord2f(1, 1); glVertex3f( 1,  1, -0.5f);
        glTexCoord2f(0, 1); glVertex3f(-1,  1, -0.5f);
    }
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

int GPUCom::loadShader(char* filename, std::string& text)
{
	std::ifstream ifs;
	ifs.open(filename, std::ios::in);

	std::string line;
	while (ifs.good()) {
        getline(ifs, line);

		text += line + "\n";
    }

	return 0;
}