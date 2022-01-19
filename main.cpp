/*
*	CS 334 Final Project
*	David Jackson, jacks668
*
*/


#define GLEW_STATIC 1
#include <windows.h>
#include <GL/glew.h>
#include <stdio.h>
#include <GL/glut.h>
#include <GL/glui.h>
#include "GPUHeader.h"

static int winId = -1;
static GLUI *glui;
float wavewidth = 0.5;
float wavespe = 0.005;
int optionSelect = 0;
int mouseOn = 0;
int mousex = 0;
int mousey = 0;
int arcnum = 3;
GPUCom  *gpu;


static void	refresh (void)
{
	gpu->colorChoice = optionSelect;

	gpu->mouseConfirm = mouseOn;
	gpu->mouseloc[0] = mousex;
	gpu->mouseloc[1] = mousey;

	gpu->arcCount = arcnum;
	gpu->speed = wavespe;
	gpu->amplitude = wavewidth;

	gpu->restart();
}


/*****************************************************************************
*****************************************************************************/
static void
mouseCB(int button, int state, int x, int y)
{
	mousex = x + 10;
	mousey = y + 10;
}


/*****************************************************************************
*****************************************************************************/
static void motionCB(int x, int y)
{
	mousex = x + 10;
	mousey = y + 10;
}


/*****************************************************************************
*****************************************************************************/
void reshapeCB(int width, int height)
{
    if (height == 0) height = 1;
    
    glViewport(0, 0, width, height);
    
	// Use orthographic projection
    glMatrixMode(GL_PROJECTION);    
    glLoadIdentity();               
    gluOrtho2D(-1, 1, -1, 1);       
    glMatrixMode(GL_MODELVIEW);     
    glLoadIdentity();   
}

/*****************************************************************************
*****************************************************************************/
void
keyboardCB(unsigned char key, int x, int y)
{
}

/*****************************************************************************
*****************************************************************************/
void
idleFunc()
{	
	if (glutGetWindow() != winId) {
		glutSetWindow(winId);
	}
	glutPostRedisplay();
}

/*****************************************************************************
*****************************************************************************/
void
refreshCB()
{
	for (int i = 0; i < 10; ++i) {
		// Update the cells' states
		gpu->update();  

		// Display the results
		gpu->display();
	}

    glutSwapBuffers();
}

/*****************************************************************************
*****************************************************************************/
void MakeMenu()
{
	glui = GLUI_Master.create_glui("Settings", 0, 0, 0);
	//glui->add_statictext("Bolt Thickness Settings");
	//GLUI_Spinner *W = glui->add_spinner("Wave Amplitude (0-10):", GLUI_SPINNER_FLOAT, &wavewidth);
	//W->set_float_limits(1, 5);
	//W->set_float_val(1);
	glui->add_separator();
	glui->add_statictext("Bolt Speed Settings");
	GLUI_Spinner *WS = glui->add_spinner("Wave Speed (0-0.01):", GLUI_SPINNER_FLOAT, &wavespe);
	WS->set_float_limits(0, 0.01);
	WS->set_float_val(0.0005);
	glui->add_separator();
	glui->add_statictext("Arc Count Settings");
	GLUI_Spinner* WC = glui->add_spinner("Arc Count (3 - 15):", GLUI_SPINNER_INT, &arcnum);
	WC->set_int_limits(3, 15);
	WC->set_int_val(3);
	glui->add_separator();
	GLUI_Panel *color_panel = glui->add_panel("Arc Color Settings:");
	GLUI_RadioGroup *options = glui->add_radiogroup_to_panel(color_panel, &optionSelect);
	glui->add_radiobutton_to_group(options, "Red");
	glui->add_radiobutton_to_group(options, "Yellow");
	glui->add_radiobutton_to_group(options, "Green");
	glui->add_radiobutton_to_group(options, "Blue");
	glui->add_radiobutton_to_group(options, "Purple");
	glui->add_radiobutton_to_group(options, "Gamer");
	glui->add_separator();
	GLUI_Checkbox *mouseCheck = glui->add_checkbox("Activate Mouse Magnatism?", &mouseOn);
	glui->add_separator();

	glui->add_button("Refresh", 0, (GLUI_Update_CB)refresh);
	glui->set_main_gfx_window(winId);

	/* We register the idle callback with GLUI, *not* with GLUT */
	GLUI_Master.set_glutIdleFunc(idleFunc);
}

/*****************************************************************************
*****************************************************************************/
int
main(int argc, char *argv[])
{
	// init OpenGL/GLUT
	glutInit(&argc, argv);
	
	// create main window
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(720, 720);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	winId = glutCreateWindow("MainWindow");

	// Initialize GPU Communication Class
	glewInit();
	gpu = new GPUCom(720, 720);
	
	// setup callbacks
	glutDisplayFunc(refreshCB);
	glutReshapeFunc(reshapeCB);
	glutMouseFunc(mouseCB);
	glutMotionFunc(motionCB);
	glutIdleFunc(idleFunc);
	
	// force initial matrix setup
	reshapeCB(720, 720);

	// set modelview matrix stack to identity
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// make GLUI GUI
	MakeMenu();
	glutMainLoop();

	return (TRUE);
}
