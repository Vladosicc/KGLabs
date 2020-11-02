#include "Render.h"

#include <sstream>
#include <iostream>

#include <windows.h>
#include <GL\GL.h>
#include <GL\GLU.h>

#include "MyOGL.h"

#include "Camera.h"
#include "Light.h"
#include "Primitives.h"


#include "GUItextRectangle.h"

#define PI 3.14159265

#define TOP_RIGHT 1.0f,1.0f
#define TOP_LEFT 0.0f,1.0f
#define BOTTOM_RIGHT 1.0f,0.0f
#define BOTTOM_LEFT 0.0f,0.0f

bool textureMode = true;
bool lightMode = true;
bool changeTexture = false;
bool alpha = false;

//����� ��� ��������� ������
class CustomCamera : public Camera
{
public:
	//��������� ������
	double camDist;
	//���� �������� ������
	double fi1, fi2;

	
	//������� ������ �� ���������
	CustomCamera()
	{
		camDist = 15;
		fi1 = 1;
		fi2 = 1;
	}

	
	//������� ������� ������, ������ �� ����� ��������, ���������� �������
	void SetUpCamera()
	{
		//�������� �� ������� ������ ������
		lookPoint.setCoords(0, 0, 0);

		pos.setCoords(camDist*cos(fi2)*cos(fi1),
			camDist*cos(fi2)*sin(fi1),
			camDist*sin(fi2));

		if (cos(fi2) <= 0)
			normal.setCoords(0, 0, -1);
		else
			normal.setCoords(0, 0, 1);

		LookAt();
	}

	void CustomCamera::LookAt()
	{
		//������� ��������� ������
		gluLookAt(pos.X(), pos.Y(), pos.Z(), lookPoint.X(), lookPoint.Y(), lookPoint.Z(), normal.X(), normal.Y(), normal.Z());
	}



}  camera;   //������� ������ ������


//����� ��� ��������� �����
class CustomLight : public Light
{
public:
	CustomLight()
	{
		//��������� ������� �����
		pos = Vector3(1, 1, 3);
	}

	
	//������ ����� � ����� ��� ���������� �����, ���������� �������
	void  DrawLightGhismo()
	{
		glDisable(GL_LIGHTING);

		
		glColor3d(0.9, 0.8, 0);
		Sphere s;
		s.pos = pos;
		s.scale = s.scale*0.08;
		s.Show();
		
		if (OpenGL::isKeyPressed('G'))
		{
			glColor3d(0, 0, 0);
			//����� �� ��������� ����� �� ����������
			glBegin(GL_LINES);
			glVertex3d(pos.X(), pos.Y(), pos.Z());
			glVertex3d(pos.X(), pos.Y(), 0);
			glEnd();

			//������ ���������
			Circle c;
			c.pos.setCoords(pos.X(), pos.Y(), 0);
			c.scale = c.scale*1.5;
			c.Show();
		}

	}

	void SetUpLight()
	{
		GLfloat amb[] = { 0.2, 0.2, 0.2, 0 };
		GLfloat dif[] = { 1.0, 1.0, 1.0, 0 };
		GLfloat spec[] = { .7, .7, .7, 0 };
		GLfloat position[] = { pos.X(), pos.Y(), pos.Z(), 1. };

		// ��������� ��������� �����
		glLightfv(GL_LIGHT0, GL_POSITION, position);
		// �������������� ����������� �����
		// ������� ��������� (���������� ����)
		glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
		// ��������� ������������ �����
		glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
		// ��������� ���������� ������������ �����
		glLightfv(GL_LIGHT0, GL_SPECULAR, spec);

		glEnable(GL_LIGHT0);
	}


} light;  //������� �������� �����




//������ ���������� ����
int mouseX = 0, mouseY = 0;

void mouseEvent(OpenGL *ogl, int mX, int mY)
{
	int dx = mouseX - mX;
	int dy = mouseY - mY;
	mouseX = mX;
	mouseY = mY;

	//������ ���� ������ ��� ������� ����� ������ ����
	if (OpenGL::isKeyPressed(VK_RBUTTON))
	{
		camera.fi1 += 0.01*dx;
		camera.fi2 += -0.01*dy;
	}

	
	//������� ���� �� ���������, � ����� ��� ����
	if (OpenGL::isKeyPressed('G') && !OpenGL::isKeyPressed(VK_LBUTTON))
	{
		LPPOINT POINT = new tagPOINT();
		GetCursorPos(POINT);
		ScreenToClient(ogl->getHwnd(), POINT);
		POINT->y = ogl->getHeight() - POINT->y;

		Ray r = camera.getLookRay(POINT->x, POINT->y);

		double z = light.pos.Z();

		double k = 0, x = 0, y = 0;
		if (r.direction.Z() == 0)
			k = 0;
		else
			k = (z - r.origin.Z()) / r.direction.Z();

		x = k*r.direction.X() + r.origin.X();
		y = k*r.direction.Y() + r.origin.Y();

		light.pos = Vector3(x, y, z);
	}

	if (OpenGL::isKeyPressed('G') && OpenGL::isKeyPressed(VK_LBUTTON))
	{
		light.pos = light.pos + Vector3(0, 0, 0.02*dy);
	}

	
}

void mouseWheelEvent(OpenGL *ogl, int delta)
{

	if (delta < 0 && camera.camDist <= 1)
		return;
	if (delta > 0 && camera.camDist >= 100)
		return;

	camera.camDist += 0.01*delta;

}

void keyDownEvent(OpenGL *ogl, int key)
{
	if (key == 'L')
	{
		lightMode = !lightMode;
	}

	if (key == 'T')
	{
		textureMode = !textureMode;
	}

	if (key == 'Q')
	{
		changeTexture = !changeTexture;
	}

	if (key == 'B')
	{
		alpha = !alpha;
	}



	if (key == 'R')
	{
		camera.fi1 = 1;
		camera.fi2 = 1;
		camera.camDist = 15;

		light.pos = Vector3(1, 1, 3);
	}

	if (key == 'F')
	{
		light.pos = camera.pos;
	}
}

void keyUpEvent(OpenGL *ogl, int key)
{
	
}

GLuint texId[2];

void UploadTextureInTexId(const char* name, const int NumberOfTexId)
{
	//������ ����������� ���������  (R G B)
	RGBTRIPLE* texarray;
	
	//������ ��������, (������*������*4      4, ���������   ����, �� ������� ������������ �� 4 ����� �� ������� �������� - R G B A)
	char* texCharArray;
	int texW, texH;
	OpenGL::LoadBMP(name, &texW, &texH, &texarray);
	OpenGL::RGBtoChar(texarray, texW, texH, &texCharArray);


	//���������� �� ��� ��������
	glGenTextures(1, &texId[NumberOfTexId]);
	//������ ��������, ��� ��� ����� ����������� � ���������, ����� ����������� �� ����� ��
	glBindTexture(GL_TEXTURE_2D, texId[NumberOfTexId]);

	//��������� �������� � �����������, � ���������� ��� ������  ��� �� �����
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texW, texH, 0, GL_RGBA, GL_UNSIGNED_BYTE, texCharArray);

	//�������� ������
	free(texCharArray);
	free(texarray);

	//������� ����
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

//����������� ����� ������ ��������
void initRender(OpenGL *ogl)
{
	//��������� �������

	//4 ����� �� �������� �������
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	//��������� ������ ��������� �������
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	//�������� ��������
	glEnable(GL_TEXTURE_2D);
	

	//������ ����������� ���������  (R G B)
	RGBTRIPLE *texarray;

	//������ ��������, (������*������*4      4, ���������   ����, �� ������� ������������ �� 4 ����� �� ������� �������� - R G B A)
	char *texCharArray;
	int texW, texH;
	
	UploadTextureInTexId("texture.bmp", 0);
	UploadTextureInTexId("texture2.bmp", 1);


	//������ � ���� ����������� � "������"
	ogl->mainCamera = &camera;
	ogl->mainLight = &light;

	// ������������ �������� : �� ����� ����� ����� 1
	glEnable(GL_NORMALIZE);

	// ���������� ������������� ��� �����
	glEnable(GL_LINE_SMOOTH); 


	//   ������ ��������� ���������
	//  �������� GL_LIGHT_MODEL_TWO_SIDE - 
	//                0 -  ������� � ���������� �������� ���������(�� ���������), 
	//                1 - ������� � ���������� �������������� ������� ��������       
	//                �������������� ������� � ���������� ��������� ����������.    
	//  �������� GL_LIGHT_MODEL_AMBIENT - ������ ������� ���������, 
	//                �� ��������� �� ���������
	// �� ��������� (0.2, 0.2, 0.2, 1.0)

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 0);

	camera.fi1 = -1.3;
	camera.fi2 = 0.8;
}

double* ConvertInPoint(double x, double y, double z)
{
	double Point[] = { x,y,z };
	return Point;
}

void NormalizeVector(double* vec)
{
	double modVector = -sqrt(pow(vec[0], 2) + pow(vec[1], 2) + pow(vec[2], 2));

	for (int i = 0; i < 3; ++i)
	{
		vec[i] /= modVector;
	}

}

double* Normal(double A[3], double B[3], double C[3])
{
	double vector1[] = { B[0] - A[0], B[1] - A[1] ,B[2] - A[2] };

	double vector2[] = { C[0] - A[0], C[1] - A[1],C[2] - A[2] };

	double vector_normali[] = { vector1[1] * vector2[2] - vector2[1] * vector1[2], -vector1[0] * vector2[2] + vector2[0] * vector1[2], vector1[0] * vector2[1] - vector2[0] * vector1[1] };

	double lenght = sqrt(vector_normali[0] * vector_normali[0] + vector_normali[1] * vector_normali[1] + vector_normali[2] * vector_normali[2]);

	vector_normali[0] /= lenght;
	vector_normali[1] /= lenght;
	vector_normali[2] /= lenght;

	return vector_normali;
}

double* FindNormal(double x, double y, double z, double x1, double y1, double z1, double x2, double y2, double z2, int FlagSwap = 0) // A - "�����"
{
	double vectorA[3], vectorB[3];
	double a[3] = { x,y,z }, b[3] = { x1,y1,z1 }, c[3] = { x2,y2,z2 };

	for (int i = 0; i < 3; ++i) // �������� ������ A � B
	{
		vectorA[i] = a[i] - c[i];
		vectorB[i] = b[i] - c[i];
	}

	double VectorNormal[3];

	//VectorNormal[0] = a[1] * b[2] - a[2] * b[1];
	//VectorNormal[1] = a[2] * b[0] - a[0] * b[2];
	//VectorNormal[2] = a[0] * b[1] - a[1] * b[0];

	VectorNormal[0] = vectorA[1] * vectorB[2] - vectorB[1] * vectorA[2];
	VectorNormal[1] = -vectorA[0] * vectorB[2] + vectorB[0] * vectorA[2];
	VectorNormal[2] = vectorA[0] * vectorB[1] - vectorB[0] * vectorA[1];

	NormalizeVector(VectorNormal);

	if (FlagSwap != 0)
	{
		for (int i = 0; i < 3; ++i) // �������� ������ A � B
		{
			VectorNormal[i] *= -1;
		}
	}

	return VectorNormal;
}

double* FindNormal(double* a, double* b, double* c, int FlagSwap = 0) // A - "�����"
{
	double vectorA[3], vectorB[3];

	for (int i = 0; i < 3; ++i) // �������� ������ A � B
	{
		vectorA[i] = a[i] - c[i];
		vectorB[i] = b[i] - c[i];
	}

	double VectorNormal[3];

	//VectorNormal[0] = a[1] * b[2] - a[2] * b[1];
	//VectorNormal[1] = a[2] * b[0] - a[0] * b[2];
	//VectorNormal[2] = a[0] * b[1] - a[1] * b[0];

	VectorNormal[0] = vectorA[1] * vectorB[2] - vectorB[1] * vectorA[2];
	VectorNormal[1] = -vectorA[0] * vectorB[2] + vectorB[0] * vectorA[2];
	VectorNormal[2] = vectorA[0] * vectorB[1] - vectorB[0] * vectorA[1];

	NormalizeVector(VectorNormal);

	if (FlagSwap != 0)
	{
		for (int i = 0; i < 3; ++i) // �������� ������ A � B
		{
			VectorNormal[i] *= -1;
		}
	}

	return VectorNormal;
}

double NewCoordY(double oldCoord)
{
	double NewCoord = oldCoord / 11.563;
	return NewCoord;
}

double NewCoordX(double oldCoord)
{
	double NewCoord = oldCoord / 9.8;
	return NewCoord;
}

#pragma region Figura
void RandomColor()
{
	glColor3d((std::rand() % 9) / 10., (std::rand() % 9) / 10., (std::rand() % 9) / 10.);
}

#pragma region 50
void sqr()
{
	double a[] = { 0,0,6 };
	double b[] = { 5,4,6 };
	double c[] = { 4,2,6 };
	glBegin(GL_TRIANGLES);
	glNormal3dv(FindNormal(a, b, c));
	glTexCoord2d(NewCoordY(a[1]),NewCoordX(a[0]));
	glVertex3dv(a);
	glTexCoord2d(NewCoordY(b[1]), NewCoordX(b[0]));
	glVertex3dv(b);
	glTexCoord2d(NewCoordY(c[1]), NewCoordX(c[0]));
	glVertex3dv(c);
	glEnd();
}
void sqr1()
{
	double a[] = { 2,8,6 };
	double b[] = { 5,4,6 };
	double c[] = { 9,3,6 };
	glBegin(GL_TRIANGLES);
	glNormal3dv(FindNormal(a, b, c, 1));
	glTexCoord2d(NewCoordY(a[1]), NewCoordX(a[0]));
	glVertex3dv(a);
	glTexCoord2d(NewCoordY(b[1]), NewCoordX(b[0]));
	glVertex3dv(b);
	glTexCoord2d(NewCoordY(c[1]), NewCoordX(c[0]));
	glVertex3dv(c);
	glEnd();
}

void sqrbot()
{
	double a[] = { 0,0,2 };
	double b[] = { 5,4,2 };
	double c[] = { 4,2,2 };
	glBegin(GL_TRIANGLES);
	glNormal3dv(FindNormal(a, b, c,1));
	glTexCoord2d(NewCoordY(a[1]), NewCoordX(a[0]));
	glVertex3dv(a);
	glTexCoord2d(NewCoordY(b[1]), NewCoordX(b[0]));
	glVertex3dv(b);
	glTexCoord2d(NewCoordY(c[1]), NewCoordX(c[0]));
	glVertex3dv(c);
	glEnd();
}
void sqr1bot()
{
	double a[] = { 2,8,2 };
	double b[] = { 5,4,2 };
	double c[] = { 9,3,2 };
	glBegin(GL_TRIANGLES);
	glNormal3dv(FindNormal(a, b, c));
	glTexCoord2d(NewCoordY(a[1]), NewCoordX(a[0]));
	glVertex3dv(a);
	glTexCoord2d(NewCoordY(b[1]), NewCoordX(b[0]));
	glVertex3dv(b);
	glTexCoord2d(NewCoordY(c[1]), NewCoordX(c[0]));
	glVertex3dv(c);
	glEnd();
}
void sqr2()
{
	double a[] = { 4,2,6 };
	double b[] = { 5,4,6 };
	double c[] = { 9,3,6 };
	double d[] = { 8,-3,6 };
	glBegin(GL_QUADS);
	glNormal3dv(FindNormal(a, b, c));
	glVertex3dv(a);
	glVertex3dv(b);
	glVertex3dv(c);
	glVertex3dv(d);
	glEnd();
}
void figura()
{
	sqr();
	sqr1();
}
void figura2()
{
	glDisable(GL_BLEND);
	glColor3d(0, 0.9, 0.9);
	sqrbot();
	glColor3d(0, 0.9, 0.9);
	sqr1bot();
}
void st1()
{
	double a[] = { 4,2,6 };
	double b[] = { 8,-3,6 };
	double d[] = { 4,2,2 };
	double c[] = { 8,-3,2 };
	glNormal3dv(FindNormal(a, b, c));
	glTexCoord2d(TOP_LEFT);
	glVertex3dv(a);
	glTexCoord2d(TOP_RIGHT);
	glVertex3dv(b);
	glTexCoord2d(BOTTOM_RIGHT);
	glVertex3dv(c);
	glTexCoord2d(BOTTOM_LEFT);
	glVertex3dv(d);
}
void st2()
{
	double a[] = { 4,2,6 };
	double b[] = { 0,0,6 };
	double d[] = { 4,2,2 };
	double c[] = { 0,0,2 };
	glNormal3dv(FindNormal(a, b, c,1));
	glTexCoord2d(TOP_LEFT);
	glVertex3dv(a);
	glTexCoord2d(TOP_RIGHT);
	glVertex3dv(b);
	glTexCoord2d(BOTTOM_RIGHT);
	glVertex3dv(c);
	glTexCoord2d(BOTTOM_LEFT);
	glVertex3dv(d);
}
void st3()
{
	double a[] = { 5,4,6 };
	double b[] = { 0,0,6 };
	double d[] = { 5,4,2 };
	double c[] = { 0,0,2 };
	glNormal3dv(FindNormal(a, b, c));
	glTexCoord2d(TOP_LEFT);
	glVertex3dv(a);
	glTexCoord2d(TOP_RIGHT);
	glVertex3dv(b);
	glTexCoord2d(BOTTOM_RIGHT);
	glVertex3dv(c);
	glTexCoord2d(BOTTOM_LEFT);
	glVertex3dv(d);
}
void st4()
{
	double a[] = { 5,4,6 };
	double b[] = { 2,8,6 };
	double d[] = { 5,4,2 };
	double c[] = { 2,8,2 };
	glNormal3dv(FindNormal(a, b, c,1));
	glTexCoord2d(TOP_LEFT);
	glVertex3dv(a);
	glTexCoord2d(TOP_RIGHT);
	glVertex3dv(b);
	glTexCoord2d(BOTTOM_RIGHT);
	glVertex3dv(c);
	glTexCoord2d(BOTTOM_LEFT);
	glVertex3dv(d);
}
void st5() // ��������
{
	double a3[] = { 9,3,6 };
	double b3[] = { 2,8,6 };
	//double d[] = { 9,3,2 };
	//double c[] = { 2,8,2 };
	//glVertex3dv(a);
	//glVertex3dv(b);
	//glVertex3dv(c);
	//glVertex3dv(d);
	double x01 = 5.5, y01 = 5.5, r = 4.3;
	std::vector <double> coords_x1;
	std::vector <double> coords_y1;
	coords_x1.push_back(b3[0]);
	coords_y1.push_back(b3[1]);
	for (double i = -54.48; i < 180 - 54.48; i += 0.09)
	{
		double y, x;
		coords_x1.push_back(x01 + r * sin(i * PI / 180));
		coords_y1.push_back(y01 + r * cos(i * PI / 180));
	}
	coords_x1.push_back(a3[0]);
	coords_y1.push_back(a3[1]);
	glBegin(GL_QUADS);
	for (double i = 0; i < coords_x1.size() - 1; i++)
	{
		glNormal3dv(FindNormal(coords_x1[i], coords_y1[i], 6, coords_x1[i], coords_y1[i], 2, coords_x1[i + 1], coords_y1[i + 1], 2));
		glTexCoord2d(i/(coords_x1.size() - 1), 1);
		glVertex3d(coords_x1[i], coords_y1[i], 6);
		glTexCoord2d(i / (coords_x1.size() - 1), 0);
		glVertex3d(coords_x1[i], coords_y1[i], 2);
		glTexCoord2d((i + 1) / (coords_x1.size() - 1), 0);
		glVertex3d(coords_x1[i + 1], coords_y1[i + 1], 2);
		glTexCoord2d((i + 1) / (coords_x1.size() - 1), 1);
		glVertex3d(coords_x1[i + 1], coords_y1[i + 1], 6);
	}
	glEnd();
	//����
	/*glBegin(GL_POLYGON);
	glColor3d(0, 0.9, 0.9);
	glNormal3dv(FindNormal(coords_x1[0], coords_y1[0], 6, coords_x1[1], coords_y1[1], 6, coords_x1[1 + 1], coords_y1[1 + 1], 6,1));
	for (int i = 0; i < coords_x1.size() - 1; i++)
	{
		glTexCoord2d(NewCoordY(coords_y1[i]), NewCoordX(coords_x1[i]));
		glVertex3d(coords_x1[i], coords_y1[i], 6);
		glTexCoord2d(NewCoordY(coords_y1[i+1]), NewCoordX(coords_x1[i+1]));
		glVertex3d(coords_x1[i + 1], coords_y1[i + 1], 6);
	}
	glEnd();*/
	//------
	//���
	glBegin(GL_POLYGON);
	glNormal3dv(FindNormal(coords_x1[0], coords_y1[0], 2, coords_x1[1], coords_y1[1], 2, coords_x1[1 + 1], coords_y1[1 + 1], 2));
	for (int i = 0; i < coords_x1.size() - 1; i++)
	{
		glTexCoord2d(NewCoordY(coords_y1[i]), NewCoordX(coords_x1[i]));
		glVertex3d(coords_x1[i], coords_y1[i], 2);
		glTexCoord2d(NewCoordY(coords_y1[i + 1]), NewCoordX(coords_x1[i + 1]));
		glVertex3d(coords_x1[i + 1], coords_y1[i + 1], 2);
	}
	glEnd();
	//------

	//�������----------------------------------------------------

	double a2[] = { 9,3,6 };
	double b2[] = { 8,-3,6 };
	glColor3d(1, 0.2, 1);
	//double x0 = 8.5, y0 = 0;
	double x0 = 11.607, y0 = -0.51785;
	//double r = 3.04;  // ������ "a" �� y
	r = sqrt(pow(9 - x0, 2) + pow(3 - y0, 2));  // ������ "a" �� y
	double rb = 1.49; //������ "b" �� x
	std::vector <double> coords_x;
	std::vector <double> coords_y;
	coords_x.push_back(a2[0]);
	coords_y.push_back(a2[1]);
	double povorot = -(atan(a2[0] - b2[0] / a2[1] - b2[1]) * 180 / PI); // ������� ��� ���������� ���� ��������
	//double povorot = -9.462;
	double alpha_div2 = 41;
	for (double i = 90 - alpha_div2 - 6; i <= 90 + alpha_div2; i += 0.09)
	{
		double y1, x1;
		x1 = r * cos(i * PI / 180); // ������ ������ � ������ ���������
		y1 = r * sin(i * PI / 180);
		coords_x.push_back(x1 * cos(povorot * PI / 180) + y1 * sin(povorot * PI / 180) + x0); // ������� ��� �������� ������� ������������ ������ ��������� � ������� ����� ������� � ����� M
		coords_y.push_back(-x1 * sin(povorot * PI / 180) + y1 * cos(povorot * PI / 180) + y0);
	}
	/*coords_x.push_back(b2[0]);
	coords_y.push_back(b2[1]);*/
	glBegin(GL_QUADS);
	for (double i = 0; i < coords_x.size() - 1; i++)
	{
		glNormal3dv(FindNormal(coords_x[i], coords_y[i], 6, coords_x[i], coords_y[i], 2, coords_x[i + 1], coords_y[i + 1], 2));
		glTexCoord2d(i / (coords_x.size() - 1), 1); glVertex3d(coords_x[i], coords_y[i], 6);
		glTexCoord2d(i / (coords_x.size() - 1), 0); glVertex3d(coords_x[i], coords_y[i], 2);
		glTexCoord2d((i + 1) / (coords_x.size() - 1), 0); glVertex3d(coords_x[i + 1], coords_y[i + 1], 2);
		glTexCoord2d((i + 1) / (coords_x.size() - 1), 1); glVertex3d(coords_x[i + 1], coords_y[i + 1], 6);
	}
	glEnd();
	double b[] = { 4,2,6 };
	double a[] = { 5,4,6 };
	double b1[] = { 4,2,2 };
	double a1[] = { 5,4,2 };
	glColor3d(0, 0.9, 0.9);

	//���
	glBegin(GL_TRIANGLES);
	glNormal3dv(FindNormal(coords_x[0], coords_y[0], 2, coords_x[1], coords_y[1], 2, coords_x[1 + 1], coords_y[1 + 1], 2));
	for (int i = 0; i < (coords_x.size() - 1) / 2; i++)
	{
		glTexCoord2d(NewCoordY(a[1]), NewCoordX(a[0])); glVertex3dv(a1);
		glTexCoord2d(NewCoordY(coords_y[i]), NewCoordX(coords_x[i])); glVertex3d(coords_x[i], coords_y[i], 2);
		glTexCoord2d(NewCoordY(coords_y[i + 1]), NewCoordX(coords_x[i + 1])); glVertex3d(coords_x[i + 1], coords_y[i + 1], 2);
	}
	for (int i = (coords_x.size() - 1) / 2; i < coords_x.size() - 1; i++)
	{
		glTexCoord2d(NewCoordY(b[1]), NewCoordX(b[0])); glVertex3dv(b1);
		glTexCoord2d(NewCoordY(coords_y[i]), NewCoordX(coords_x[i])); glVertex3d(coords_x[i], coords_y[i], 2);
		glTexCoord2d(NewCoordY(coords_y[i + 1]), NewCoordX(coords_x[i + 1])); glVertex3d(coords_x[i + 1], coords_y[i + 1], 2);
	}
	glTexCoord2d(NewCoordY(a[1]), NewCoordX(a[0])); glVertex3dv(a1);
	glTexCoord2d(NewCoordY(b[1]), NewCoordX(b[0])); glVertex3dv(b1);
	glTexCoord2d(NewCoordY(coords_y[(coords_y.size() - 1) / 2]), NewCoordX(coords_x[(coords_y.size() - 1) / 2])); glVertex3d(coords_x[(coords_x.size() - 1) / 2], coords_y[(coords_y.size() - 1) / 2], 2);
	glEnd();

	if (alpha)
	{
		glEnable(GL_BLEND); glBlendFunc(GL_ONE, GL_ONE);
	}

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glColor4d(0.5, 0.5, 0, 0.1);
	//���� � �����
	glBegin(GL_POLYGON);
	glNormal3dv(FindNormal(coords_x1[0], coords_y1[0], 6, coords_x1[1], coords_y1[1], 6, coords_x1[1 + 1], coords_y1[1 + 1], 6, 1));
	for (int i = 0; i < coords_x1.size() - 1; i++)
	{
		glTexCoord2d(NewCoordY(coords_y1[i]), NewCoordX(coords_x1[i]));
		glVertex3d(coords_x1[i], coords_y1[i], 6);
		glTexCoord2d(NewCoordY(coords_y1[i + 1]), NewCoordX(coords_x1[i + 1]));
		glVertex3d(coords_x1[i + 1], coords_y1[i + 1], 6);
	}
	glEnd();
	//------

#pragma region Triangles
	glBegin(GL_TRIANGLES);
	glNormal3dv(FindNormal(coords_x[0], coords_y[0], 6, coords_x[1], coords_y[1], 6, coords_x[1 + 1], coords_y[1 + 1], 6, 1));
	for (int i = 0; i < (coords_x.size() - 1) / 2; i++)
	{
		glTexCoord2d(NewCoordY(a[1]), NewCoordX(a[0])); glVertex3dv(a);
		glTexCoord2d(NewCoordY(coords_y[i]), NewCoordX(coords_x[i])); glVertex3d(coords_x[i], coords_y[i], 6);
		glTexCoord2d(NewCoordY(coords_y[i+1]), NewCoordX(coords_x[i+1])); glVertex3d(coords_x[i + 1], coords_y[i + 1], 6);
	}
	for (int i = (coords_x.size() - 1) / 2; i < coords_x.size() - 1; i++)
	{
		glTexCoord2d(NewCoordY(b[1]), NewCoordX(b[0])); glVertex3dv(b);
		glTexCoord2d(NewCoordY(coords_y[i]), NewCoordX(coords_x[i])); glVertex3d(coords_x[i], coords_y[i], 6);
		glTexCoord2d(NewCoordY(coords_y[i + 1]), NewCoordX(coords_x[i + 1])); glVertex3d(coords_x[i + 1], coords_y[i + 1], 6);
	}
	glTexCoord2d(NewCoordY(a[1]), NewCoordX(a[0])); glVertex3dv(a);
	glTexCoord2d(NewCoordY(b[1]), NewCoordX(b[0])); glVertex3dv(b);
	glTexCoord2d(NewCoordY(coords_y[(coords_y.size() - 1) / 2]), NewCoordX(coords_x[(coords_y.size() - 1) / 2])); glVertex3d(coords_x[(coords_x.size() - 1) / 2], coords_y[(coords_y.size() - 1) / 2], 6);
	//���
	//glNormal3dv(FindNormal(coords_x[0], coords_y[0], 2, coords_x[1], coords_y[1], 2, coords_x[1 + 1], coords_y[1 + 1], 2));
	//for (int i = 0; i < (coords_x.size() - 1) / 2; i++)
	//{
	//	glTexCoord2d(NewCoordY(a[1]), NewCoordX(a[0])); glVertex3dv(a1);
	//	glTexCoord2d(NewCoordY(coords_y[i]), NewCoordX(coords_x[i])); glVertex3d(coords_x[i], coords_y[i], 2);
	//	glTexCoord2d(NewCoordY(coords_y[i + 1]), NewCoordX(coords_x[i + 1])); glVertex3d(coords_x[i + 1], coords_y[i + 1], 2);
	//}
	//for (int i = (coords_x.size() - 1) / 2; i < coords_x.size() - 1; i++)
	//{
	//	glTexCoord2d(NewCoordY(b[1]), NewCoordX(b[0])); glVertex3dv(b1);
	//	glTexCoord2d(NewCoordY(coords_y[i]), NewCoordX(coords_x[i])); glVertex3d(coords_x[i], coords_y[i], 2);
	//	glTexCoord2d(NewCoordY(coords_y[i + 1]), NewCoordX(coords_x[i + 1])); glVertex3d(coords_x[i + 1], coords_y[i + 1], 2);
	//}
	//glTexCoord2d(NewCoordY(a[1]), NewCoordX(a[0])); glVertex3dv(a1);
	//glTexCoord2d(NewCoordY(b[1]), NewCoordX(b[0])); glVertex3dv(b1);
	//glTexCoord2d(NewCoordY(coords_y[(coords_y.size() - 1) / 2]), NewCoordX(coords_x[(coords_y.size() - 1) / 2])); glVertex3d(coords_x[(coords_x.size() - 1) / 2], coords_y[(coords_y.size() - 1) / 2], 2);
	//glEnd();
	glEnd();
#pragma endregion

#pragma region Polygon
	/*glBegin(GL_POLYGON);
	glVertex3dv(b);
	glVertex3dv(a);
	for (int i = 0; i < coords_x.size() - 1; i++)
	{
		glVertex3d(coords_x[i], coords_y[i], 6);
		glVertex3d(coords_x[i + 1], coords_y[i + 1], 6);
	}
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3dv(b1);
	glVertex3dv(a1);
	for (int i = 0; i < coords_x.size() - 1; i++)
	{
		glVertex3d(coords_x[i], coords_y[i], 2);
		glVertex3d(coords_x[i + 1], coords_y[i + 1], 2);
	}
	glEnd();*/
#pragma endregion
}

void Stena()
{
	glBegin(GL_QUADS);
	glColor3d(1, 0, 1);
	st1();
	st2();
	st3();
	st4();
	glEnd();
	st5();
}
#pragma endregion

void PrintFigura()
{
	figura2();
	Stena();
	figura();
}
#pragma endregion



void Render(OpenGL *ogl)
{

	glBindTexture(GL_TEXTURE_2D, texId[0]);

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

	glEnable(GL_DEPTH_TEST);
	if (textureMode)
		glEnable(GL_TEXTURE_2D);

	if (lightMode)
		glEnable(GL_LIGHTING);

	if (changeTexture)
		glBindTexture(GL_TEXTURE_2D, texId[1]);


	//��������������
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	//��������� ���������
	GLfloat amb[] = { 0.3, 0.3, 0.3, 0.3 };
	GLfloat dif[] = { 0.7, 0.7, 0.7, 0.4 };
	GLfloat spec[] = { 0.9, 0.8, 0.4, 0.5 };
	GLfloat sh = 0.1f * 256;


	//�������
	glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
	//��������
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
	//����������
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec); \
		//������ �����
		glMaterialf(GL_FRONT, GL_SHININESS, sh);

	//���� ���� �������, ��� ����������� (����������� ���������)
	glShadeModel(GL_SMOOTH);
	//===================================
	//������� ���  


	//������ ��������� ���������� ��������
	//double A[2] = { -4, -4 };
	//double B[2] = { 4, -4 };
	//double C[2] = { 4, 4 };
	//double D[2] = { -4, 4 };

	//glColor3d(0.6, 0.6, 0.6);
	//glBegin(GL_QUADS);

	//glNormal3d(0, 0, 1);
	//glTexCoord2d(0, 0);
	//glVertex2dv(A);
	//glTexCoord2d(1, 0);
	//glVertex2dv(B);
	//glTexCoord2d(1, 1);
	//glVertex2dv(C);
	//glTexCoord2d(0, 1);
	//glVertex2dv(D);

	//glEnd();
	//����� ��������� ���������� ��������

	PrintFigura();

   //��������� ������ ������

	
	glMatrixMode(GL_PROJECTION);	//������ �������� ������� ��������. 
	                                //(���� ��������� ��������, ����� �� ������������.)
	glPushMatrix();   //��������� ������� ������� ������������� (������� ��������� ������������� ��������) � ���� 				    
	glLoadIdentity();	  //��������� ��������� �������
	glOrtho(0, ogl->getWidth(), 0, ogl->getHeight(), 0, 1);	 //������� ����� ������������� ��������

	glMatrixMode(GL_MODELVIEW);		//������������� �� �����-��� �������
	glPushMatrix();			  //��������� ������� ������� � ���� (��������� ������, ����������)
	glLoadIdentity();		  //���������� �� � ������

	glDisable(GL_LIGHTING);



	GuiTextRectangle rec;		   //������� ����� ��������� ��� ������� ������ � �������� ������.
	rec.setSize(300, 200);
	rec.setPosition(10, ogl->getHeight() - 200 - 10);


	std::stringstream ss;
	ss << "T - ���/���� �������" << std::endl;
	ss << "Q - ����� �������" << std::endl;
	ss << "B - ���/���� �����-���������" << std::endl;
	ss << "L - ���/���� ���������" << std::endl;
	ss << "F - ���� �� ������" << std::endl;
	ss << "G - ������� ���� �� �����������" << std::endl;
	ss << "G+��� ������� ���� �� ���������" << std::endl;
	ss << "�����. �����: (" << light.pos.X() << ", " << light.pos.Y() << ", " << light.pos.Z() << ")" << std::endl;
	ss << "�����. ������: (" << camera.pos.X() << ", " << camera.pos.Y() << ", " << camera.pos.Z() << ")" << std::endl;
	ss << "��������� ������: R="  << camera.camDist << ", fi1=" << camera.fi1 << ", fi2=" << camera.fi2 << std::endl;
	
	rec.setText(ss.str().c_str());
	rec.Draw();

	glMatrixMode(GL_PROJECTION);	  //��������������� ������� �������� � �����-��� �������� �� �����.
	glPopMatrix();


	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

}