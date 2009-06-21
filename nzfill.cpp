#include <cstdlib>  
#include <cmath>  
#include <GL/glut.h>
#include <vector>

struct vec2f
{
	float x,y;
};

//Calc signed area of trapezoid
float calcTrArea(const vec2f& p1, const vec2f& p2)
{
	return (p1.x+p2.x)/2.0f * (p2.y-p1.y);
}

//Calc signed area of triangle
float calcArea(const vec2f& p1, const vec2f& p2, const vec2f& p3)
{
	return calcTrArea(p1, p2)+calcTrArea(p2, p3)+calcTrArea(p3, p1);
}

enum Commands
{
	VG_MOVE_TO,
	VG_LINE_TO,
	VG_CLOSE
};

//GLuint contours[] = {3, 3};
//const vec2f cp[] = {{-2,0},
//					{0, 2},
//					{2, 0},
//					{0, 0},
//					{1, 1},
//					{-1, 1}};


GLuint contours[] = {5,};
const vec2f cp[] = {{-2,0},
					{0, 4},
					{2, 0},
					{-3, 3},
					{3, 3}};

//GLuint contours[] = {5,};
//const vec2f cp[] = {{-2, 0},
//					{0, 4},
//					{2, 0},
//					{3, 3},
//					{-3, 3},
//					};
GLuint numCP = sizeof(cp)/sizeof(cp[0]);

std::vector<GLuint> idxCW;
std::vector<GLuint> idxCCW;

enum Winding
{
	windCW,
	windCCW,
};

void addTri(Winding wind, GLuint i1, GLuint i2, GLuint i3)
{
	std::vector<GLuint>&	idx = (wind==windCCW)?idxCCW:idxCW;
	idx.push_back(i1);
	idx.push_back(i2);
	idx.push_back(i3);
}

//Sort driangles by winding: CW or CCW
void analyze()
{
	GLuint c=0, numContours = sizeof(contours)/sizeof(contours[0]);
	GLuint base = 0;
	while (c<numContours)
	{
		GLuint i = 2;
		GLuint numCP = contours[c];
		while (i<numCP)
		{
			float area = calcArea(cp[base], cp[base+i-1], cp[base+i]);
			
			addTri((area<0)?windCW:windCCW, base, base+i-1, base+i);
			++i;
		}
		base+=numCP;
		++c;
	}
}

static void resize(int width, int height)  
{  
    const float ar = (float) width / (float) height;  
  
    glViewport(0, 0, width, height);  
    glMatrixMode(GL_PROJECTION);  
    glLoadIdentity();  
    gluPerspective(60,ar,1,100);  
    glMatrixMode(GL_MODELVIEW);  
    gluLookAt(0,0,-5,0,0,0,0,1,0);
	glScalef(0.5,0.5,1);
	glTranslatef(0,0,5);
	glEnable(GL_STENCIL_TEST);
}  

//Implemented technique for non-zero filling rule support at most 127 self overlapping
static void display(void)  
{
	//0x80 - it's our imagenery "zero"
	//because it's seems HW performs incr and decr stencil operations with saturation and not borrowing
	glClearStencil(0x80);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);  
	
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glStencilFunc(GL_ALWAYS, 0, 0xFF);
	glCullFace(GL_FRONT_AND_BACK);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, cp);

	//Decrement stencil for clockwise oriented triangles
	glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
	if (!idxCW.empty())
	{
		glDrawElements(GL_TRIANGLES, (GLsizei)idxCW.size(), GL_UNSIGNED_INT, &idxCW[0]);
	}

	//Increment stencil for counter-clockwise oriented triangles
	glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
	if (!idxCCW.empty())
	{
		glDrawElements(GL_TRIANGLES, (GLsizei)idxCCW.size(), GL_UNSIGNED_INT, &idxCCW[0]);
	}

	glDisableClientState(GL_VERTEX_ARRAY);

	//Fill region where stencil is "non-zero"(non-0x80)
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glStencilFunc(GL_NOTEQUAL, 0x80, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	glBegin(GL_QUADS);
		glVertex2f(-10, -10);
		glVertex2f(-10,  10);
		glVertex2f( 10,  10);
		glVertex2f( 10, -10);
	glEnd();

	glutSwapBuffers();  
}  
  
static void key(unsigned char key, int x, int y)  
{  
    switch (key)  
    {  
        case 27 : // esc for exit  
        case 'q':  
            //glutLeaveGameMode();  
            exit(0);  
            break;  
    }  
  
    glutPostRedisplay();  
}  
  
static void idle(void)  
{  
    glutPostRedisplay();  
}  
  
const GLfloat light_position[] = { 0.0f, 0.0f, 5.0f, 0.0f };  
  
int main(int argc, char *argv[])  
{  
    glutInit(&argc, argv);  
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);  
	glutGameModeString( "800x600:32@60" ); //the settings for fullscreen mode  
    glutEnterGameMode();  
  
    glutReshapeFunc(resize);  
    glutDisplayFunc(display);  
    glutKeyboardFunc(key);  
    glutIdleFunc(idle);  
  
    glClearColor(0,0,0,1);  

	analyze();
    glEnable(GL_DEPTH_TEST);  
    glDepthFunc(GL_LEQUAL);  
  
    glutMainLoop();  
  
    return EXIT_SUCCESS;  
}  
