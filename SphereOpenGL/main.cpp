#include <gl/glut.h>
#include <math.h>
#include <stdio.h>

//#define DEBUG
//#define PERSPECTIVE

static int Width  = 400;
static int Height = 400;
int mouse_down_x, mouse_down_y, mouse_move_x, mouse_move_y;

#define STRANGLE 1
#define STRIANLESTRI 2
#define QUAD 3
#define QUADSTRIP 4

char DrawMethod = 2;

void myMenuEvents(int select)
{
	DrawMethod = select;
	glutPostRedisplay();
}

void CreateMenu()
{
	glutCreateMenu(myMenuEvents);
	glutAddMenuEntry("四边形带", QUADSTRIP);
	glutAddMenuEntry("三角形带", STRIANLESTRI);
	glutAddMenuEntry("三角形", STRANGLE);
	glutAddMenuEntry("四边形", QUAD);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

#define Pi (3.141592653)

float R = 1;
float level = 18;
int NumOfSphere = 3;

void initialize()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1, 1, 1);
	glPolygonMode(GL_FRONT, GL_LINE);
	glPolygonMode(GL_BACK, GL_LINE);
	
}

void myDisplay()
{
	initialize();
	
	for (int num = 0; num < NumOfSphere; num ++)
	{
		glViewport(Width*num, 0, Width, Height);

		float AngleAcc = Pi / level;
		float currentAngle = AngleAcc;

		float string = R*sin(currentAngle);
		float height = R*cos(currentAngle);
		float angle = 0;
		glBegin(GL_TRIANGLE_FAN);
		glVertex3f(0, R, 0);
		for (angle = 0; angle < 2 * Pi; angle += AngleAcc)
		{
			glVertex3f(string*cos(angle), height, string*sin(angle));
		}
		glEnd();

		glBegin(GL_TRIANGLE_FAN);
		glVertex3f(0, -R, 0);
		for (angle = 0; angle < 2 * Pi; angle += AngleAcc)
		{
			glVertex3f(string*cos(angle), -height, string*sin(angle));
		}
		glEnd();

		switch (DrawMethod)
		{
		case STRANGLE:
			glBegin(GL_TRIANGLES);
			while (currentAngle < Pi)
			{
				for (angle = 0; angle < 2 * Pi; angle += AngleAcc)
				{
					glVertex3f(
						R*sin(angle)*sin(currentAngle),
						R*cos(currentAngle),
						R*cos(angle)*sin(currentAngle)
						);
					glVertex3f(
						R*sin(angle)*sin(currentAngle + AngleAcc),
						R*cos(currentAngle + AngleAcc),
						R*cos(angle)*sin(currentAngle + AngleAcc)
						);
					glVertex3f(
						R*sin(angle + AngleAcc)*sin(currentAngle),
						R*cos(currentAngle),
						R*cos(angle + AngleAcc)*sin(currentAngle)
						);
				}
				currentAngle += AngleAcc;
			}
			glEnd();
			break;
		case STRIANLESTRI:
			glBegin(GL_TRIANGLE_STRIP);
		lable_draw:
			while (currentAngle < Pi)
			{
				for (angle = 0; angle < 2 * Pi; angle += AngleAcc)
				{
					glVertex3f(
						R*sin(angle)*sin(currentAngle),
						R*cos(currentAngle),
						R*cos(angle)*sin(currentAngle)
						);
					glVertex3f(
						R*sin(angle)*sin(currentAngle + AngleAcc),
						R*cos(currentAngle + AngleAcc),
						R*cos(angle)*sin(currentAngle + AngleAcc)
						);
				}
				currentAngle += AngleAcc;
			}
			glEnd();
			break;
		case QUAD:
			glBegin(GL_QUADS);
			while (currentAngle < Pi)
			{
				for (angle = 0; angle < 2 * Pi; angle += AngleAcc)
				{
					glVertex3f(
						R*sin(angle)*sin(currentAngle),
						R*cos(currentAngle),
						R*cos(angle)*sin(currentAngle)
						);
					glVertex3f(
						R*sin(angle)*sin(currentAngle + AngleAcc),
						R*cos(currentAngle + AngleAcc),
						R*cos(angle)*sin(currentAngle + AngleAcc)
						);
					glVertex3f(
						R*sin(angle + AngleAcc)*sin(currentAngle + AngleAcc),
						R*cos(currentAngle + AngleAcc),
						R*cos(angle + AngleAcc)*sin(currentAngle + AngleAcc)
						);
					glVertex3f(
						R*sin(angle + AngleAcc)*sin(currentAngle),
						R*cos(currentAngle),
						R*cos(angle + AngleAcc)*sin(currentAngle)
						);
				}
				currentAngle += AngleAcc;
			}
			glEnd();
			break;
		case QUADSTRIP:
			glBegin(GL_QUAD_STRIP);
			goto lable_draw;
		default:
			break;
		}
	}
	glutSwapBuffers();
	glFlush();
}

void myMotionFunc(int x, int y)
{
	mouse_move_x = x - mouse_down_x;
	mouse_move_y = y - mouse_down_y;
	glLoadIdentity();
	glRotated(mouse_move_x, 0, 1, 0);
	glRotated(mouse_move_y, 1, 0, 0);
	glutPostRedisplay();
}

void myMouseFunc(int Mouse, int State, int x, int y)
{
	if (Mouse == GLUT_LEFT_BUTTON && State == GLUT_DOWN)
	{
		mouse_down_x = x;
		mouse_down_y = y;
	}
}

void myKeyBoardFunc(unsigned char key, int x, int y)
{
	static float ScaleRate = 0.2;
#ifdef DEBUG
	printf("%c", key);
#endif // DEBUG
	glLoadIdentity();
	switch (key)
	{
	case 's'://smaller
		R *= (1.0-ScaleRate);
		goto E;
	case 'a'://bigger
		//glScalef(0.5, .5, .5);
		R = (R * (1.0 + ScaleRate)) < 1 ? R * (1.0 + ScaleRate) : 1;
		goto E;
	case 'w'://draw less lines
		level = level - 1 < 2 ? 2 : level - 1;
		goto E;
	case 'q'://draw more lines
		level += 1;
		goto E;
	case 'z'://add a sphere
		NumOfSphere++;
		goto E;
	case 'x'://delete a sphere until one last
		NumOfSphere = NumOfSphere - 1 < 1 ? 1 : NumOfSphere - 1;
		goto E;
	case '\b'://reset
		glLoadIdentity();
	default:
	E :
		glutPostRedisplay();
		break;
	}

}

void myReshapeWndFunc(int width, int height)
{
#ifdef PERSPECTIVE
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(80.0, (GLfloat) width / (GLfloat) height, 0.1, 10.0);
	gluLookAt(0, 0, 2 * R, 0, 0, 0, 10, 0, 0);
	glMatrixMode(GL_MODELVIEW);
#endif // PERSPECTIVE
	int size = (width > height) ? height : width;
	glViewport(0, 0, size, size);
	
}

int main(int argc, char* argv [])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowSize(Width, Height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Test1");
	CreateMenu();
	glutDisplayFunc(&myDisplay);
	glutMouseFunc(&myMouseFunc);
	glutMotionFunc(&myMotionFunc);
	glutKeyboardFunc(&myKeyBoardFunc);
	glutReshapeFunc(&myReshapeWndFunc);
	glutMainLoop();
	return 0;
}