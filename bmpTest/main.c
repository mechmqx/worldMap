#define WindowWidth  800
#define WindowHeight 400
#define WindowTitle  "OpenGL纹理测试"

#define GLUT_BUILDING_LIB

#include <gl/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "genBmpTexture.h"
#include "matrixCalc.h"

//定义两个纹理对象编号
GLint texGround;
GLint texWall;
typedef struct tagRoam{
	GLfloat x;
	GLfloat y;
	GLfloat z;
}sRoam;

typedef struct tagScene{
	GLdouble projection[16];
	GLdouble modelview[16];
	GLdouble invert_projection[16];
	GLdouble inver_modelview[16];
	sRoam engineRoam;
}sDrawScene;

sDrawScene m_scene;

#define BMP_Header_Length 54  //图像数据在内存块中的偏移量
static GLfloat angle = 0.0f;   //旋转角度

void screenPt2geo(int x,int y, float* plon, float* plat);

void initParam()
{
	m_scene.engineRoam.x = 0;
	m_scene.engineRoam.y = 0;
	m_scene.engineRoam.z = 0;

	testMatrix4x4Multi();    //test code    20180628 test OK by mqx
	testVecMultiMatrix4x4();    // test code  20180628 test OK by mqx
	
}
/*
screenPt2geo
input : x,y the screen point / viewport coord
output : plon,plat the lon and lat coord
*/
void screenPt2geo(int x,int y, float* plon, float* plat)
{
	int i=0;
	GLfloat depth;
	float xx,yy,zz;
	double aa,bb,cc,vec_in[4],vec_out[4]; 
	GLint viewport[4]={0,0,WindowWidth,WindowHeight};
	double MVP[16],invMVP[16],transInvert[16];
	glReadPixels(x,y,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&depth);
	printf("[mqx test] the depth value is %f at(%d,%d) on screen.\n",depth,x,y);

	// viewport --> NDC
	xx = (float)(x )/(WindowWidth)*2.0-1.0;
	yy = (float)(y)/(WindowHeight)*2.0-1.0;
	//zz = (double)(m_scene.engineRoam.z+10.0);
	zz = depth;// -(71.0/69.0)+140.0/(zz*69.0);
	vec_in[0] = xx;
	vec_in[1] = yy;
	vec_in[2] = zz*2.0-1.0;
	vec_in[3] = 1;
	
	printf("\n[mqx test] the NDC coord(%5.3lf,%5.3lf,%5.3lf)\n",vec_in[0],vec_in[1],vec_in[2]);
	// mvp 
	multiMatrix4x4(m_scene.projection,m_scene.modelview,MVP);
	calcInverMatrix4x4(MVP,invMVP);
	printf("[mqx test]  inver matrix:\n");
	for (i=0;i<16;i++)
	{
		printf("\t%5.3lf",invMVP[i]);
		if (i%4==3)
		{
			printf("\n");
		}
	}
		
	calcMatrix4x4Transposition(invMVP,transInvert);
	vecMultiMatrix4x4(transInvert,vec_in,vec_out);
	printf("[mqx test] the win coord(%5.3lf,%5.3lf,%5.3lf)\n\n",vec_out[0],vec_out[1],vec_out[2]);

	// manual projection
	printf("[mqx test] current input coord(%5.3lf,%5.3lf,%5.3lf)\n"
		        ,m_scene.engineRoam.x,m_scene.engineRoam.y,m_scene.engineRoam.z);
	vec_in[0] = m_scene.engineRoam.x;
	vec_in[1] = m_scene.engineRoam.y;
	vec_in[2] = m_scene.engineRoam.z;
	vec_in[3] = 1.0;
	vecMultiMatrix4x4(MVP,vec_in,vec_out);
	printf("[mqx test] the window coord(%5.3lf,%5.3lf,%5.3lf)\n\n",vec_out[0],vec_out[1],vec_out[2]);

#if 0
	// 2. another way to cal inver matrix     // test the 2 ways to calc matrix equals
	multiMatrix4x4(m_scene.inver_modelview,m_scene.invert_projection,invMVP);
	printf("[mqx test] another way to cal inver matrix:\n");
	for (i=0;i<16;i++)
	{
		printf("\t%5.3lf",invMVP[i]);
		if (i%4==3)
		{
			printf("\n");
		}
	}
#endif

}

void display(void)
{
	// 清除屏幕
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// 设置视角
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, WindowWidth/WindowHeight, 1, 50);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0,0,   // eye
		              0, 0, 1,  // to
		              0, 1, 0); // up

	//glRotatef(angle, 0.0f, 0.0f, 1.0f); //旋转
	glTranslated(m_scene.engineRoam.x, m_scene.engineRoam.y,m_scene.engineRoam.z);

	glViewport(0,0,WindowWidth,WindowHeight);
	{
		glGetDoublev(GL_MODELVIEW_MATRIX,m_scene.modelview);
		glGetDoublev(GL_PROJECTION_MATRIX,m_scene.projection);
		calcInverMatrix4x4(m_scene.modelview,m_scene.inver_modelview);
		calcInverMatrix4x4(m_scene.projection,m_scene.invert_projection);

		//screenPt2geo(WindowWidth/2,WindowHeight/2,NULL,NULL);
	}
	// 绘制底面以及纹理
	glBindTexture(GL_TEXTURE_2D, texGround);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(180.0f, -90.0f, 10.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(180.0f, 90.0f, 10.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-180.0f, 90.0f, 10.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-180.0f, -90.0f, 10.0f);
	glEnd();

	glutSwapBuffers();  
}

void myIdle(void)  
{     
	angle += 1.8f;    
	if( angle >= 360.0f )      
		angle = 0.0f;   
	display();  
}   

void myKeyboard(unsigned char key,int x,int y)
{
	switch(key)
	{
	case 'x':
		m_scene.engineRoam.z += 0.2;
		break;
	case 'v':
		m_scene.engineRoam.z -= 0.2;
		break;
	case '4':
		m_scene.engineRoam.x +=0.2;
		break;
	case '6':
		m_scene.engineRoam.x -=0.2;
		break;
	case '8':
		m_scene.engineRoam.y += 0.2;
		break;
	case '5':
		m_scene.engineRoam.y -=0.2;
		break;
	default:
		break;
	}
	screenPt2geo(WindowWidth/2,WindowHeight/2,NULL,NULL);    // test code
}

int matrix_test()
{
	double arcs[MAX_MATRIX_SIZE][MAX_MATRIX_SIZE];
	double astar[MAX_MATRIX_SIZE][MAX_MATRIX_SIZE];
	int i,j;
	int n;
	int a;
	while(scanf("%d",&n)!=EOF && n)
	{
		for(i=0;i<n;i++)
		{
			for(j=0;j<n;j++)
			{
				scanf("%lf",&arcs[i][j]);
			}
		}

		a = getMatrixCofator(arcs,n);
		if(a==0)
		{
			printf("can not transform!\n");
		}
		else
		{
			calcInverMatrix(arcs,n,astar);
			for(i=0;i<n;i++)
			{
				for(j=0;j<n;j++)
				{
					printf("%.3lf ",(double)astar[i][j]/a);
				}
				printf("\n");
			}
		}
		printf("\n");
	}
	return 0;
}

void testMatrx4x4()  //20180826 test OK
{
	int i;
	double input_mat[16] = {
		sqrt(2.0)/2.0,-sqrt(2.0)/2.0,0,0,
		sqrt(2.0)/2.0,sqrt(2.0)/2.0,0,0,
		0,0,1,0,
		0,0,0,1.0
	};
	double output_mat[16] = { 0.0 };
	calcInverMatrix4x4(input_mat, output_mat);
	printf("\nwill print the test result:\n");
	for(i=0;i<16;i++)
	{
		printf("%lf\t",output_mat[i]);
		if (i%4==3)
		{
			printf("\n");
		}
	}
    printf("print test result end.\n");

}

int main(int argc, char* argv[])
{
	// GLUT初始化
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WindowWidth, WindowHeight);
	glutCreateWindow(WindowTitle);	
	glEnable(GL_DEPTH_TEST);    
	glEnable(GL_TEXTURE_2D);    // 启用纹理
	// 1. Init render env
	initParam();
	//testMatrx4x4();     //20180826 test OK
	createBmpTexture("..\\world3.bmp",&texGround);
	//end of init

	// 2. render func
	glutDisplayFunc(&display);   //注册函数 
	glutIdleFunc(&myIdle);  
	glutKeyboardFunc(myKeyboard);
	glutMainLoop(); //循环调用
	return 0;
}