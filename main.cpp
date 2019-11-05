#include <iostream>
#include <math.h>
#include <GLUT/glut.h>
#include <stdlib.h>
#include <stdbool.h>
#include "tgaClass.h"


#define MAX_TEXTURES    4

#define SCREEN_WIDTH 640                        // We want our screen width 640 pixels
#define SCREEN_HEIGHT 480

// global variables
GLfloat angle1 = 0;   /* in degrees */
GLfloat angle2 = 0;   /* in degrees */
int moving, startx, starty = 0;
bool map=false;
bool first = true;

using namespace std;

void display();
void init();
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);
void arrowkeys(int key, int x, int y);
void CreateTexture(unsigned int textureArray[], char * strFileName, int textureID);

unsigned int g_Texture[MAX_TEXTURES] = {0};

void CreateTexture(unsigned int textureArray[], char * strFileName, int textureID)
/*
 Read tga file to add texture into object
 */
{
    
    char buffer[30];    //filename holder
    int status = 0;        //error codes for file read
    TGA myTGAImage;
    
    if(!strFileName){            // Return from the function if no file name was passed in
        
        cout <<"Error occurred, bad file name "<< endl;
        return;
    }
    
    sprintf (buffer, strFileName);        //load buffer with filename
    status = myTGAImage.readTGA(buffer);// Load into a TGA object
    if(status > 1) {
        cout <<"Error occurred = " << status << strFileName<< endl;
        exit(0);
    }
    
    glGenTextures((GLsizei)1, (GLuint *)&textureArray[textureID]);
    
    // This sets the alignment requirements for the start of each pixel row in memory.
    glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
    
    // Bind the texture to the texture arrays index and init the texture
    glBindTexture(GL_TEXTURE_2D, textureArray[textureID]);
    
    // Build Mipmaps (builds different versions of the picture for distances - looks better)
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3,  myTGAImage.width,
                      myTGAImage.height, myTGAImage.format, GL_UNSIGNED_BYTE, myTGAImage.data);
    
}

void display()
/*
 Display the polygon to add textures
 */
{
    
    glShadeModel(GL_SMOOTH);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    // Clear The Screen And The Depth Buffer
    glLoadIdentity();                                    // Reset The matrix
    //       Position              View       Up Vector
    gluLookAt(0, 0, 12, 0, 0, 0, 0, 1, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    if(map == true) {
        glEnable(GL_TEXTURE_2D);
        
        if(first) {
            first = false;
        }
    }
    else {
        glDisable(GL_TEXTURE_2D);
    }

    glPushMatrix();
    glRotatef(angle2, 1.0, 0.0, 0.0);
    glRotatef(angle1, 0.0, 1.0, 0.0);
    if(map == true) {
        glBindTexture(GL_TEXTURE_2D,  g_Texture[0]);
        
    }
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-5.0f, 1.0f, 5.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-5.0f, -1.0f, 5.0f);

    glTexCoord2f(2.0f, 0.0f);
    glVertex3f(5.0f,  -1.0f, 5.0f);

    glTexCoord2f(2.0f, 1.0f);
    glVertex3f(5.0f, 1.0f, 5.0f);

    glTexCoord2f(1.0, 2.0);
    glVertex3f(1.0, -5.0, -5.0);
    glEnd();
    glPopMatrix();
    
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_DST_COLOR, GL_ZERO);

    // Since we are done, we can now turn OFF blending and turn ON depth testing
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);

    glutSwapBuffers();
}


static void motion(int x, int y)
/*
 Rotate the object with mouse
 */
{
    if (moving) {
        angle1 = (angle1 + (x - startx));
        angle2 = (angle2 + (y - starty));
        startx = x;
        starty = y;
        glutPostRedisplay();
    }
}


void mouse(int button, int state, int x, int y)
/*
 GLUT calls this when mouse is clicked
 */
{
    /* Rotate the scene with the left mouse button. */
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            moving = 1;
            startx = x;
            starty = y;
        }
        if (state == GLUT_UP) {
            moving = 0;
        }
    }
}

void keyboard(unsigned char key, int x, int y)
/*
 GLUT calls this routine when a key is pressed
 */
{
#pragma unused(x,y)
    switch (key) {
        case 'q': {
            int w = glutGetWindow();
            glutDestroyWindow(w);
            glDeleteTextures((GLsizei)MAX_TEXTURES, (GLuint *)g_Texture);
            exit(0);
            break;
        }
        case 'a': {
            map=true;
            CreateTexture(g_Texture, "marble.tga", 0);
            glutPostRedisplay();
            break;
        }
        case 'b': {
            map=true;
            CreateTexture(g_Texture, "fish.tga", 0);
            glutPostRedisplay();
            break;
        }
        case 'c': {
            map=true;
            CreateTexture(g_Texture, "color.tga", 0);
            glutPostRedisplay();
            break;
        }
    }
}


void menu_choose(int mode)
/*
 Create menu options to change different textures
 */
{
    switch (mode) {
        case 1: //nearest
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
            glutPostRedisplay();
            break;
        case 2: //linear
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
            glutPostRedisplay();
            break;
        case 3: //clamp
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
            glutPostRedisplay();
            break;
        case 4: //no texture
            map = false;
            glutPostRedisplay();
            break;
    }
}

void init()
/*
 Initializes OpenGL
 */
{
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    int width = glutGet(GLUT_WINDOW_WIDTH);
    int height = glutGet(GLUT_WINDOW_HEIGHT);
    glViewport(0,0,width,height);
    gluPerspective(60.0,(GLfloat)width/(GLfloat)height, 1.0, 30.0);
    float ambientCol[4] = {0.2, 0.5, 0.5, 1.0};
    float diffuse[4] = {1.0, 1.0, 1.0};
    glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuse);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientCol);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

void reshape (int w, int h)
/*
 GLUT calls this routine when the window is resized
 */
{
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 30.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void menu()
/*
 Create OpenGL menu when double clicked
 */
{
    glutCreateMenu(menu_choose);
    glutAddMenuEntry("Nearest", 1);
    glutAddMenuEntry("Linear", 2);
    glutAddMenuEntry("Clamp", 3);
    glutAddMenuEntry("No texture", 4);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char **argv)
/*
 Executes the main() function to display window.
 */
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(640, 480);
    glutCreateWindow("Lab 5");
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMotionFunc(motion);
    glutMouseFunc(mouse);
    init();
    menu();
    glutMainLoop();
}
