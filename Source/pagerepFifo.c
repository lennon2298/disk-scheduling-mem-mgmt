#include<stdlib.h>
#include<GL/glut.h>
#include<stdio.h>
#include<math.h>
#include <time.h>

int requestFifo[9]={0},counterFifo[3]={0},pagesFifo[3]={0}, faultFifo[9]={0}, colourFifo[9]={1,1,1,1,1,1,1,1,1}, pagecolourFifo[3]={1,1,1};
float assignFifo[9]={-5.5,-5.5,-5.5,-5.5,-5.5,-5.5,-5.5,-5.5,-5.5};
int destFifo=0, showresultFifo=0;
int stepFifo=-1, startaniFifo=0, faultsFifo=0;
char resFifo[]="No. of page faults =  ";

float bgcolorFifo[][3]={{0,0,0.7},{0,0.5,0},{0,0,0.5}};
int bgpointerFifo=0;
float tilecolorFifo[][3]={{1,1,0},{1,0.7,0.7},{0,1,1}};
int tilepointerFifo=0;


void initFifo()
{
	glColor3f(0,0,0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0,800,0,600);
	glMatrixMode(GL_MODELVIEW);

}

int getFifo()
{
	if(counterFifo[0]>=counterFifo[1] && counterFifo[0]>=counterFifo[2]) return 0;
	if(counterFifo[1]>=counterFifo[0] && counterFifo[1]>=counterFifo[2]) return 1;
	if(counterFifo[2]>=counterFifo[1] && counterFifo[2]>=counterFifo[3]) return 2;
	return 2;
}

void tileFifo(float x, float y, char n)
{

	float size=20;

	//glColor3f(1,1,0);
	glBegin(GL_POLYGON);
	glVertex2f(x-size, y-size);
	glVertex2f(x+size, y-size);
	glVertex2f(x+size, y+size);
	glVertex2f(x-size, y+size);
	glEnd();
	glColor3f(0,0,0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(x-size, y-size);
	glVertex2f(x+size, y-size);
	glVertex2f(x+size, y+size);
	glVertex2f(x-size, y+size);
	glEnd();
	glRasterPos2f(x-size/2, y);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, n);


}

void draw_flagFifo()
{

	glColor3fv(bgcolorFifo[bgpointerFifo]);
	glBegin(GL_POLYGON);
	glVertex2f(-10,-10);
	glVertex2f(10,-10);
	glVertex2f(10,10);
	glVertex2f(-10,10);
	glEnd();

}



void setup_requestFifo()
{
	int i;
	glColor3fv(bgcolorFifo[bgpointerFifo]);

	glBegin(GL_POLYGON);
	glVertex2f(0,0);
	glVertex2f(700,0);
	glVertex2f(700,100);
	glVertex2f(0,100);
	glEnd();


	glPushMatrix();
	glTranslatef(-10, 40, 0);
	for(i=0; i<9; i++)
	{
		glColor3fv(tilecolorFifo[tilepointerFifo]);
		glTranslatef(70, 0, 0);
		glPushMatrix();

		if(assignFifo[i]>-4.5)
		{

			glTranslatef(70*stepFifo-70*i,0,0);
		}

		glTranslatef(0, -250-45*assignFifo[i], 0);

		if((int)assignFifo[i]==destFifo && assignFifo[i]>=0)
				glColor3f(1,0.3,0.3);
		else
			//glColor3f(colour[i],1,0);
			glColor3fv(tilecolorFifo[tilepointerFifo]);

		tileFifo(10,10,requestFifo[i]+'0');
		glPopMatrix();

		if(faultFifo[i])
		{
		glPushMatrix();
		glTranslatef(0, -385, 0);
		draw_flagFifo();
		glPopMatrix();
		}

	}

	glPopMatrix();

}

void drawTextFifo(char *string,float x,float y,float z)
{
char *c;
glRasterPos3f(x, y,z);
for (c=string; *c != '\0'; c++)
{
	if(*c=='\n')
	glRasterPos3f(x, y-0.05,z);
	else
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *c);
}
}


void setup_pagesFifo()
{


	glPushMatrix();
	//glColor3f(fabs(sin(ycolour[1])), 1, 0);
//	glColor3f(1, pagecolour[0], 0);
	tileFifo(0,0,pagesFifo[0]==0?' ':pagesFifo[0]+'0');
	glTranslatef(0, -45, 0);
//	glColor3f(fabs(sin(ycolour[2])), 1, 0);
	//glColor3f(1, pagecolour[1], 0);
	tileFifo(0,0,pagesFifo[1]==0?' ':pagesFifo[1]+'0');
	glTranslatef(0, -45, 0);
//	glColor3f(fabs(sin(ycolour[3])), 1, 0);
	//glColor3f(1, pagecolour[2], 0);
	tileFifo(0,0,pagesFifo[2]==0?' ':pagesFifo[2]+'0');
	glPopMatrix();


}


void displayFifo()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();


	glPushMatrix();
	glTranslatef(120+70*stepFifo,200,0);
	setup_pagesFifo();
	glPopMatrix();

	glColor3f(1,1,0);
	glPushMatrix();
	glTranslatef(50, 400, 0);
	setup_requestFifo();
	glPopMatrix();


	glEnd();



	if(showresultFifo && stepFifo==8)
	{
		glColor3f(0,0,0);
		resFifo[21]=faultsFifo+'0';
		drawTextFifo(resFifo, 50, 20, 0);
	}
		drawTextFifo("First In First Out", 250, 550, 0);

	glFlush();
	glutSwapBuffers();
}

void idleFifo()
{

	if(!startaniFifo)
		return;

	if(destFifo>assignFifo[stepFifo])
	assignFifo[stepFifo]+=0.05;

	if(destFifo<=assignFifo[stepFifo])
	{
		if(faultFifo[stepFifo])
		pagesFifo[destFifo]=requestFifo[stepFifo];
		startaniFifo=0;
		destFifo=-10;
		showresultFifo=1;
	}

	displayFifo();

}


void mouseFifo(int btn,int state,int x, int y)
{

	int n,i,j;

	if(startaniFifo==1)
		return;

	if(btn==GLUT_LEFT_BUTTON && state==GLUT_DOWN &&stepFifo<8)


	{


		if(stepFifo<9)
			stepFifo++;





		for(i=0;i<3;i++)
		{

			if(requestFifo[stepFifo]==pagesFifo[i])
			{
				faultFifo[stepFifo]=0;
				//counterFifo[i]=0;

				for(j=0;j<3;j++)
					if(j!=i) counterFifo[j]++;
				//dest=i;
					destFifo=-5;
				startaniFifo=1;
				colourFifo[stepFifo]=0;
				glutPostRedisplay();
					return;

			}



		}


		n=getFifo();
		counterFifo[n]=0;
		for(i=0;i<3;i++)
			if(i!=n)
			counterFifo[i]++;
		//pagecolour[n]=0;
		//assign[step]=n;
			destFifo=n;
			startaniFifo=1;

		faultFifo[stepFifo]=1;
		faultsFifo++;



	}

	glutPostRedisplay();

}

void handle_bg_colourFifo(int action)
{
	bgpointerFifo=action;
	glutPostRedisplay();
}

void handle_tile_colourFifo(int action)
{
	tilepointerFifo=action;
	glutPostRedisplay();
}

void menuFifo(int action)
{

	if(action==0)
		exit(0);

}

void addMenuFifo()
{
	int submenu1, submenu2;

	submenu1 = glutCreateMenu(handle_bg_colourFifo);
	glutAddMenuEntry("Green",0);
	glutAddMenuEntry("Red",1);
	glutAddMenuEntry("Blue",2);


	submenu2 = glutCreateMenu(handle_tile_colourFifo);
	glutAddMenuEntry("Blue",0);
	glutAddMenuEntry("Light Red",1);
	glutAddMenuEntry("Light Blue",2);



	glutCreateMenu(menuFifo);
	glutAddSubMenu("Background Colour",submenu1);
	glutAddSubMenu("Tile Colour",submenu2);
	glutAddMenuEntry("Quit",0);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void displayPageFifo()
{
    int argc;
    char* argv[10];
	glutInit(&argc,argv);

	srand(time(0));
	
	int i;
	for(i=0; i<9; i++)
	{
		requestFifo[i] = rand() % 10;
	}

	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
	glutInitWindowSize(640,480);
	glutCreateWindow("PageReplacement");
	glutDisplayFunc(displayFifo);
	glutMouseFunc(mouseFifo);
	glutIdleFunc(idleFifo);
	glClearColor(1,1,1,1);
	initFifo();
	addMenuFifo();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0,800,0,600);
	glMatrixMode(GL_MODELVIEW);
	glutMainLoop();
}
