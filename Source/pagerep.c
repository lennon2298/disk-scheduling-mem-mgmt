#include<stdlib.h>
#include<GL/glut.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#include<math.h>

int request[9]={0},counter[3]={0},pages[3]={0}, fault[9]={0}, colour[9]={1,1,1,1,1,1,1,1,1}, pagecolour[3]={1,1,1};
float assign[9]={-5.5,-5.5,-5.5,-5.5,-5.5,-5.5,-5.5,-5.5,-5.5};
int dest=0, showresult=0;
int step=-1, startani=0, faults=0;
char res[]="No. of page faults =  ";

float bgcolor[][3]={{1,0,0},{0,1,0},{0,0,1}};
int bgpointer=0;
float tilecolor[][3]={{1,1,0},{1,0.7,0.7},{0,1,1}};
int tilepointer=0;


void init()
{
	glColor3f(0,0,0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0,800,0,600);
	glMatrixMode(GL_MODELVIEW);

}

int getLRU()
{
	if(counter[0]>=counter[1] && counter[0]>=counter[2]) return 0;
	if(counter[1]>=counter[0] && counter[1]>=counter[2]) return 1;
	//if(counter[2]>=counter[1] && counter[2]>=counter[3]) return 2;
	return 2;
}

void tile(float x, float y, char n)
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

void draw_flag()
{

	glColor3fv(bgcolor[bgpointer]);
	glBegin(GL_POLYGON);
	glVertex2f(-10,-10);
	glVertex2f(10,-10);
	glVertex2f(10,10);
	glVertex2f(-10,10);
	glEnd();

}



void setup_request()
{
	int i;
	glColor3fv(bgcolor[bgpointer]);

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
		glColor3fv(tilecolor[tilepointer]);
		glTranslatef(70, 0, 0);
		glPushMatrix();

		if(assign[i]>-4.5)
		{
			
			glTranslatef(70*step-70*i,0,0);
		}

		glTranslatef(0, -250-45*assign[i], 0);

		if((int)assign[i]==dest && assign[i]>=0)
				glColor3f(1,0.3,0.3);
		else
			//glColor3f(colour[i],1,0);
			glColor3fv(tilecolor[tilepointer]);

		tile(10,10,request[i]+'0');
		glPopMatrix();

		if(fault[i])
		{
		glPushMatrix();
		glTranslatef(0, -385, 0);
		draw_flag();
		glPopMatrix();
		}

	}
	
	glPopMatrix();

}

void drawText(char *string,float x,float y,float z) 
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


void setup_pages()
{


	glPushMatrix();
	//glColor3f(fabs(sin(ycolour[1])), 1, 0);
//	glColor3f(1, pagecolour[0], 0);
	tile(0,0,pages[0]==0?' ':pages[0]+'0');
	glTranslatef(0, -45, 0);
//	glColor3f(fabs(sin(ycolour[2])), 1, 0);
	//glColor3f(1, pagecolour[1], 0);
	tile(0,0,pages[1]==0?' ':pages[1]+'0');
	glTranslatef(0, -45, 0);
//	glColor3f(fabs(sin(ycolour[3])), 1, 0);
	//glColor3f(1, pagecolour[2], 0);
	tile(0,0,pages[2]==0?' ':pages[2]+'0');
	glPopMatrix();


}


void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	
	glPushMatrix();
	glTranslatef(120+70*step,200,0);
	setup_pages();
	glPopMatrix();

	glColor3f(1,1,0);
	glPushMatrix();
	glTranslatef(50, 400, 0);
	setup_request();
	glPopMatrix();


	glEnd();

	

	if(showresult && step==8)
	{
		glColor3f(0,0,0);
		res[21]=faults+'0';
		drawText(res, 50, 20, 0);
	}
		drawText("LRU Page Replacement Algorithm", 250, 550, 0);

	glFlush();
	glutSwapBuffers();
}

void idle()
{

	if(!startani)
		return;

	if(dest>assign[step])
	assign[step]+=0.05;
	
	if(dest<=assign[step])
	{
		if(fault[step])
		pages[dest]=request[step];	
		startani=0;
		dest=-10;
		showresult=1;
	}

	display();

}


void mouse(int btn,int state,int x, int y)
{

	int n,i,j;

	if(startani==1)
		return;
	
	if(btn==GLUT_LEFT_BUTTON && state==GLUT_DOWN &&step<8)

		
	{
		

		if(step<9)
			step++;
		
		

	

		for(i=0;i<3;i++)
		{

			if(request[step]==pages[i])
			{
				fault[step]=0;
				counter[i]=0;
				
				for(j=0;j<3;j++)
					if(j!=i) counter[j]++;
				//dest=i;
					dest=-5;
				startani=1;
				colour[step]=0;
				glutPostRedisplay();
					return;

			}
				


		}


		n=getLRU();
		counter[n]=0;
		for(i=0;i<3;i++)
			if(i!=n)
			counter[i]++;
		//pagecolour[n]=0;
		//assign[step]=n;
			dest=n;
			startani=1;
		
		fault[step]=1;
		faults++;

	

	}

	glutPostRedisplay();

}

void handle_bg_colour(int action)
{
	bgpointer=action;
	glutPostRedisplay();
}

void handle_tile_colour(int action)
{
	tilepointer=action;
	glutPostRedisplay();
}

void menu(int action)
{

	if(action==0)
		exit(0);

}

void addMenu()
{
	int submenu1, submenu2;

	submenu1 = glutCreateMenu(handle_bg_colour);
	glutAddMenuEntry("Red",0);
	glutAddMenuEntry("Green",1);
	glutAddMenuEntry("Blue",2);
	

	submenu2 = glutCreateMenu(handle_tile_colour);
	glutAddMenuEntry("Yellow",0);
	glutAddMenuEntry("Light Red",1);
	glutAddMenuEntry("Light Blue",2);



	glutCreateMenu(menu);
	glutAddSubMenu("Background Colour",submenu1);
	glutAddSubMenu("Tile Colour",submenu2);
	glutAddMenuEntry("Quit",0);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void displayPage()
{
	int argc;
	char* argv[10];
	glutInit(&argc,argv);

	srand(time(0));
	
	int i;
	for(i=0; i<9; i++)
	{
		request[i] = rand() % 10;
	}

	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
	glutInitWindowSize(640,480);
	glutCreateWindow("COH");
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutIdleFunc(idle);
	glClearColor(1,1,1,1);
	init();
	addMenu();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0,800,0,600);
	glMatrixMode(GL_MODELVIEW);
	glutMainLoop();
}