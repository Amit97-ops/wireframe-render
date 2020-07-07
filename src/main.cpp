//
//  main.cpp
//  Project 4
//
//  Created by Amit on 6/03/19.
//  Copyright © 2019 Amit. All rights reserved.
//

#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

int height;
int width;

class Point
{
public:
    double x,y,z;
    Point(double xSet, double ySet, double zSet)
    {
        x = xSet;
        y = ySet;
        z = zSet;
    }
};

std::vector<Point> controlP;
std::vector<std::vector<Point> > allLines;
std::vector<std::vector <Point> > pointlist;
void ComputeAitkens(std::vector<Point>, double);
void DrawAitkens(std::vector<Point>);

int count = 0;
GLfloat angleX = 0.0f;     // Rotational angle for cube [NEW]
GLfloat angleY = 0.0f;
GLfloat angleZ = 0.0f;
int refreshMills = 15;

double distance(Point one, Point two)
{
    double retVal = 0;
    
    double dx = two.x - one.x;
    double dy = two.y - one.y;
    double dz = two.z - one.z;
    
    dx = dx*dx;
    dy = dy*dy;
    dz = dz*dz;
    
    retVal= dx+dy+dz;
    
    return sqrt(retVal);
}

void drawPoint(double x, double y, double z)
{
    glPointSize(4);
    glColor3f(1.0f, 0.0f, 0.5f);
    glBegin(GL_POINTS);
    glVertex3d(x,y,z);
    glEnd();
    glutSwapBuffers();
}

void ComputeAitkens(std::vector<Point> c, double res)
{
    ///Begin drawing of control points
    glPointSize(4);
    glColor3f(1.0f, 0.0f, 0.5f);
    glBegin(GL_POINTS);
    for(int z = 0; z<c.size(); z++)
        glVertex3d(c[z].x, c[z].y, c[z].z);
    
    glEnd();
    
    //create a temporary vector of points b to store a point on the curve given a t value
    std::vector<Point> b;
    //create a y vector of all points which have been calculated to be on the curve
    std::vector<Point> y;
    
    double n = res;
    //push first control point into final vector
    y.push_back(c[0]);
    
    //calculate t value for parametrization
    std::vector<double> tVals;
    tVals.push_back(0);
    for(int i = 1; i<c.size(); i++)
    {
        tVals.push_back(distance(c[i-1], c[i]) + tVals[i-1]);
    }
    
    //algorithm incorporates Aitken's algo to calculate all points on the curve
    //once a point has been calculated, it is stored in y vector
    for(int x = 0; x<=n; x++)
    {
        b.clear();
        for(int a = 0; a < c.size(); a++)
        {
            b.push_back(c[a]);
        }
        for(int i = 1; i < c.size(); i++)
        {
            for(int j= 0; j < c.size()-i; j++)
            {
                double crrt_tVal = tVals[j];
                double nxt_tVal = tVals[i+j];
                double my_tVal = tVals[0]+((x/res)*(tVals[tVals.size()-1]-tVals[0]));
                
                double first = ((nxt_tVal - my_tVal)/(nxt_tVal - crrt_tVal));
                double second = ((my_tVal - crrt_tVal)/(nxt_tVal - crrt_tVal));
                
                
                b[j].x = first*(b[j].x) + second*(b[j+1].x);
                b[j].y = first*(b[j].y) + second*(b[j+1].y);
                b[j].z = first*(b[j].z) + second*(b[j+1].z);
            }
        }
        y.push_back(b[0]);
    }
    
    //push last control point into curve vector
    //y.push_back(c[c.size()-1]);
    DrawAitkens(y);
}

void DrawAitkens(std::vector<Point> a)
{
    glBegin(GL_LINE_STRIP);
    glColor3d(0.0, 0.0, 1);
    for(int i = 0; i<a.size(); i++)
    {
        glVertex3d(a[i].x, a[i].y, a[i].z);
    }
    glEnd();
}


void initGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void drawGrid()
{
    for (int i = 0; i<40; i++)
    {
        glPushMatrix();
        if(i<20)
            glTranslatef(0, 0, i);
        if(i>=20)
        {
            glTranslatef(i-20, 0, 0);
            glRotatef(-90, 0, 1, 0);
        }
        glBegin(GL_LINES);
        glColor3f(1.0, 1.0, 1.0);
        glLineWidth(1);
        glVertex3f(-1*width, -0.1, 0);
        glVertex3f(width, -0.1, 0);
        glEnd();
        glPopMatrix();
    }
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27: // Escape key
            exit(0);
            break;
        case 'x':
            angleX -= 0.45f;
            glRotatef(angleX, 1.0, 0.0, 0.0);
            break;
        case 'y':
            angleY -= 0.45f;
            glRotatef(angleY, 0.0, 1.0, 0.0);
            break;
        case 'z':
            angleZ -= 0.45f;
            glRotatef(angleZ, 0.0, 0.0, 1.0);
            break;

    }
    glutSwapBuffers();
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
    glLoadIdentity();             // Reset
    glViewport(0, 0, width, height);
    GLfloat aspect = (GLfloat)width/(GLfloat)height;
    // Set the aspect ratio of the clipping volume to match the viewport
    glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
    glLoadIdentity();             // Reset
    // Enable perspective projection with fovy, aspect, zNear and zFar
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    
    glLoadIdentity();
    glTranslatef(-0.5f, 0.0f, -7.0f);
    glRotatef(angleX, 1.0f, 0.0f, 0.0f);
    glRotatef(angleY, 0.0f, 1.0f, 0.0f);
    glRotatef(angleZ, 0.0f, 0.0f, 1.0f);
    
    drawGrid();
    glOrtho(0, width, 0, height, -1, 1);
    for(int i = 0; i<allLines.size(); i++)
        ComputeAitkens(allLines[i], 100);
    glutSwapBuffers();
}

void timer(int value) {
    glutPostRedisplay();      // Post re-paint request to activate display()
    glutTimerFunc(refreshMills, timer, 0); // next timer call milliseconds later
}

void reshape(GLsizei w, GLsizei h) {  // GLsizei for non-negative integer
    // Compute aspect ratio of the new window
    if (h == 0) h = 1;// To prevent divide by 0
    width = w;
    height = h;
    GLfloat aspect = (GLfloat)width / (GLfloat)height;
    
    // Set the viewport to cover the new window
    glViewport(0, 0, width, height);
    
    // Set the aspect ratio of the clipping volume to match the viewport
    glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
    glLoadIdentity();             // Reset
    // Enable perspective projection with fovy, aspect, zNear and zFar
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

int main(int argc, char * argv[]) {
    
    int h;
    int w;
    
    glutInit(&argc, argv);
    std::cout << "Please define the height of the window you would like to display:\n";
    std::cin >> h;
    std::cout << "Please enter the width:\n";
    std::cin >> w;
    
    height = h;
    width = w;
    
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(w, h);
    glutInitWindowPosition(50, 50);
    
    int MainWindow = glutCreateWindow("Assignment 4");
    int controlPoints, degree, num_shapes;
    double x, y, z;
    std::ifstream inputFile;
    inputFile.open(argv[1]);
    inputFile >> num_shapes;
    inputFile >> controlPoints;
    
    for(int j = 0; j<num_shapes; j++)
    {
        for(int i = 0; i<controlPoints; i++)
        {
            inputFile >> x;
            inputFile >> y;
            inputFile >> z;

            Point *p = new Point(x, y, z);
            controlP.push_back(*p);
        }
        allLines.push_back(controlP);
        controlP.clear();
        inputFile >> controlPoints;
    }
    
    //Point *endPoint = new Point(x,y,z);
    //controlP.push_back(*endPoint);
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    initGL();
    glutTimerFunc(0, timer, 0);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    
}
