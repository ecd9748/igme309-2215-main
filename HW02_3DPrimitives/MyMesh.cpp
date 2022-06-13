#include "MyMesh.h"
void MyMesh::Init(void)
{
	m_bBinded = false;
	m_uVertexCount = 0;

	m_VAO = 0;
	m_VBO = 0;

	m_pShaderMngr = ShaderManager::GetInstance();
}
void MyMesh::Release(void)
{
	m_pShaderMngr = nullptr;

	if (m_VBO > 0)
		glDeleteBuffers(1, &m_VBO);

	if (m_VAO > 0)
		glDeleteVertexArrays(1, &m_VAO);

	m_lVertex.clear();
	m_lVertexPos.clear();
	m_lVertexCol.clear();
}
MyMesh::MyMesh()
{
	Init();
}
MyMesh::~MyMesh() { Release(); }
MyMesh::MyMesh(MyMesh& other)
{
	m_bBinded = other.m_bBinded;

	m_pShaderMngr = other.m_pShaderMngr;

	m_uVertexCount = other.m_uVertexCount;

	m_VAO = other.m_VAO;
	m_VBO = other.m_VBO;
}
MyMesh& MyMesh::operator=(MyMesh& other)
{
	if (this != &other)
	{
		Release();
		Init();
		MyMesh temp(other);
		Swap(temp);
	}
	return *this;
}
void MyMesh::Swap(MyMesh& other)
{
	std::swap(m_bBinded, other.m_bBinded);
	std::swap(m_uVertexCount, other.m_uVertexCount);

	std::swap(m_VAO, other.m_VAO);
	std::swap(m_VBO, other.m_VBO);

	std::swap(m_lVertex, other.m_lVertex);
	std::swap(m_lVertexPos, other.m_lVertexPos);
	std::swap(m_lVertexCol, other.m_lVertexCol);

	std::swap(m_pShaderMngr, other.m_pShaderMngr);
}
void MyMesh::CompleteMesh(vector3 a_v3Color)
{
	uint uColorCount = m_lVertexCol.size();
	for (uint i = uColorCount; i < m_uVertexCount; ++i)
	{
		m_lVertexCol.push_back(a_v3Color);
	}
}
void MyMesh::AddVertexPosition(vector3 a_v3Input)
{
	m_lVertexPos.push_back(a_v3Input);
	m_uVertexCount = m_lVertexPos.size();
}
void MyMesh::AddVertexColor(vector3 a_v3Input)
{
	m_lVertexCol.push_back(a_v3Input);
}
void MyMesh::CompileOpenGL3X(void)
{
	if (m_bBinded)
		return;

	if (m_uVertexCount == 0)
		return;

	CompleteMesh();

	for (uint i = 0; i < m_uVertexCount; i++)
	{
		//Position
		m_lVertex.push_back(m_lVertexPos[i]);
		//Color
		m_lVertex.push_back(m_lVertexCol[i]);
	}
	glGenVertexArrays(1, &m_VAO);//Generate vertex array object
	glGenBuffers(1, &m_VBO);//Generate Vertex Buffered Object

	glBindVertexArray(m_VAO);//Bind the VAO
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);//Bind the VBO
	glBufferData(GL_ARRAY_BUFFER, m_uVertexCount * 2 * sizeof(vector3), &m_lVertex[0], GL_STATIC_DRAW);//Generate space for the VBO

	// Position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)0);

	// Color attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)(1 * sizeof(vector3)));

	m_bBinded = true;

	glBindVertexArray(0); // Unbind VAO
}
void MyMesh::Render(matrix4 a_mProjection, matrix4 a_mView, matrix4 a_mModel)
{
	// Use the buffer and shader
	GLuint nShader = m_pShaderMngr->GetShaderID("Basic");
	glUseProgram(nShader);

	//Bind the VAO of this object
	glBindVertexArray(m_VAO);

	// Get the GPU variables by their name and hook them to CPU variables
	GLuint MVP = glGetUniformLocation(nShader, "MVP");
	GLuint wire = glGetUniformLocation(nShader, "wire");

	//Final Projection of the Camera
	matrix4 m4MVP = a_mProjection * a_mView * a_mModel;
	glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(m4MVP));

	//Solid
	glUniform3f(wire, -1.0f, -1.0f, -1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);

	//Wire
	glUniform3f(wire, 1.0f, 0.0f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_POLYGON_OFFSET_LINE);
	glPolygonOffset(-1.f, -1.f);
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);
	glDisable(GL_POLYGON_OFFSET_LINE);

	glBindVertexArray(0);// Unbind VAO so it does not get in the way of other objects
}
void MyMesh::AddTri(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft)
{
	//C
	//| \
	//A--B
	//This will make the triangle A->B->C 
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);
}
void MyMesh::AddQuad(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft, vector3 a_vTopRight)
{
	//C--D
	//|  |
	//A--B
	//This will make the triangle A->B->C and then the triangle C->B->D
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);

	AddVertexPosition(a_vTopLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopRight);
}
void MyMesh::GenerateCube(float a_fSize, vector3 a_v3Color)
{
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;

	Release();
	Init();

	float fValue = a_fSize * 0.5f;
	//3--2
	//|  |
	//0--1

	vector3 point0(-fValue, -fValue, fValue); //0
	vector3 point1(fValue, -fValue, fValue); //1
	vector3 point2(fValue, fValue, fValue); //2
	vector3 point3(-fValue, fValue, fValue); //3

	vector3 point4(-fValue, -fValue, -fValue); //4
	vector3 point5(fValue, -fValue, -fValue); //5
	vector3 point6(fValue, fValue, -fValue); //6
	vector3 point7(-fValue, fValue, -fValue); //7

	//F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}

void MyMesh::GenerateCone(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();
	//variables
	float step = (2.0f * PI) / (float)a_nSubdivisions;
	vector3 point1 = vector3(0.0f, 0.0f, 0.0f);//point for circle
	vector3 point2;
	vector3 point3;
	vector3 point4;//tip of cone point
	float cosine;
	float sine;

	for (float angle = 0.0f; angle < 2.0f * PI; angle += step)
	{
		//base circle
		cosine = cos(angle);
		sine = sin(angle);
		point2 = vector3(point1.x + (cosine * a_fRadius), point1.y + (sine * a_fRadius), point1.z);
		angle += step;
		cosine = cos(angle);
		sine = sin(angle);
		point3 = vector3(point1.x + (cosine * a_fRadius), point1.y + (sine * a_fRadius), point1.z);
		point4 = vector3(point1.x, point1.y, point1.z + a_fRadius * 2);
		angle -= step;
		AddTri(point2, point1, point3);//circle base tri
		AddTri(point4, point2, point3);//cone tip point
	}

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCylinder(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	//variables
	float step = (2.0f * PI) / (float)a_nSubdivisions;
	vector3 point1 = vector3(0.0f, 0.0f, 0.0f);
	vector3 point2;
	vector3 point3;
	float cosine;
	float sine;

	//first circle
	for (float angle = 0.0f; angle < 2.0f * PI; angle += step)
	{
		//base circle
		cosine = cos(angle);
		sine = sin(angle);
		point2 = vector3(point1.x + (cosine * a_fRadius), point1.y + (sine * a_fRadius), point1.z);
		angle += step;
		cosine = cos(angle);
		sine = sin(angle);
		point3 = vector3(point1.x + (cosine * a_fRadius), point1.y + (sine * a_fRadius), point1.z);
		angle -= step;
		AddTri(point2, point1, point3);
	}
	vector3 point4 = vector3(0.0f, 0.0f, 0.0f);
	vector3 point5;
	vector3 point6;

	//second circle
	for (float angle = 0.0f; angle < 2.0f * PI; angle += step)
	{
		point4.z = a_fHeight;
		//base circle
		cosine = cos(angle);
		sine = sin(angle);
		point5 = vector3(point4.x + (cosine * a_fRadius), point4.y + (sine * a_fRadius), point4.z);
		angle += step;
		cosine = cos(angle);
		sine = sin(angle);
		point6 = vector3(point4.x + (cosine * a_fRadius), point4.y + (sine * a_fRadius), point4.z);
		angle -= step;
		AddTri(point5, point6, point4);
	}

	//sides
	for (float angle = 0.0f; angle < 2.0f * PI; angle += step)
	{
		cosine = cos(angle);
		sine = sin(angle);
		point2 = vector3(point1.x + (cosine * a_fRadius), point1.y + (sine * a_fRadius), point1.z);
		point5 = vector3(point4.x + (cosine * a_fRadius), point4.y + (sine * a_fRadius), point4.z);
		angle += step;
		cosine = cos(angle);
		sine = sin(angle);
		point3 = vector3(point1.x + (cosine * a_fRadius), point1.y + (sine * a_fRadius), point1.z);
		point6 = vector3(point4.x + (cosine * a_fRadius), point4.y + (sine * a_fRadius), point4.z);
		angle -= step;
		AddQuad(point5, point2, point6, point3);
	}
	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateTube(float a_fOuterRadius, float a_fInnerRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;

	if (a_fInnerRadius < 0.005f)
		a_fInnerRadius = 0.005f;

	if (a_fInnerRadius > a_fOuterRadius)
		std::swap(a_fInnerRadius, a_fOuterRadius);

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	//variables
	float step = (2.0f * PI) / (float)a_nSubdivisions;
	vector3 point1 = vector3(0.0f, 0.0f, 0.0f);//circle 1 point
	vector3 point2;
	vector3 point3;
	float cosine;
	float sine;
	vector3 point4 = vector3(0.0f, 0.0f, 0.0f);//circle 2 point
	vector3 point5;
	vector3 point6;
	vector3 point7;
	vector3 point8;
	vector3 point9;
	vector3 point10;
	for (float angle = 0.0f; angle < 2.0f * PI; angle += step)
	{
		//inner edge
		cosine = cos(angle);
		sine = sin(angle);
		point2 = vector3(point1.x + (cosine * a_fInnerRadius), point1.y + (sine * a_fInnerRadius), point1.z);
		point9 = vector3(point4.x + (cosine * a_fInnerRadius), point4.y + (sine * a_fInnerRadius), point4.z + a_fHeight);
		angle += step;
		cosine = cos(angle);
		sine = sin(angle);
		point3 = vector3(point1.x + (cosine * a_fInnerRadius), point1.y + (sine * a_fInnerRadius), point1.z);
		point10 = vector3(point4.x + (cosine * a_fInnerRadius), point4.y + (sine * a_fInnerRadius), point4.z + a_fHeight);
		angle -= step;
		AddQuad(point9, point2, point10, point3);
		AddQuad(point2, point9, point3, point10);

		//outer edge
		cosine = cos(angle);
		sine = sin(angle);
		point5 = vector3(point1.x + (cosine * a_fOuterRadius), point1.y + (sine * a_fOuterRadius), point1.z);
		point7 = vector3(point4.x + (cosine * a_fOuterRadius), point4.y + (sine * a_fOuterRadius), point4.z + a_fHeight);
		angle += step;
		cosine = cos(angle);
		sine = sin(angle);
		point6 = vector3(point1.x + (cosine * a_fOuterRadius), point1.y + (sine * a_fOuterRadius), point1.z);
		point8 = vector3(point4.x + (cosine * a_fOuterRadius), point4.y + (sine * a_fOuterRadius), point4.z + a_fHeight);
		angle -= step;
		AddQuad(point7, point5, point8, point6);
		AddQuad(point5, point7, point6, point8);

		//connecting the edges
		AddQuad(point5, point2, point6, point3);
		AddQuad(point9, point7, point10, point8);
	}

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateTorus(float a_fOuterRadius, float a_fInnerRadius, int a_nSubdivisionsA, int a_nSubdivisionsB, vector3 a_v3Color)
{
	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;

	if (a_fInnerRadius < 0.005f)
		a_fInnerRadius = 0.005f;

	if (a_fInnerRadius > a_fOuterRadius)
		std::swap(a_fInnerRadius, a_fOuterRadius);

	if (a_nSubdivisionsA < 3)
		a_nSubdivisionsA = 3;
	if (a_nSubdivisionsA > 360)
		a_nSubdivisionsA = 360;

	if (a_nSubdivisionsB < 3)
		a_nSubdivisionsB = 3;
	if (a_nSubdivisionsB > 360)
		a_nSubdivisionsB = 360;

	Release();
	Init();

	float step = (2.0f * PI) / (float)a_nSubdivisionsA;
	float step2 = (2.0f * PI) / (float)a_nSubdivisionsB;
	vector3 point1 = vector3(0.0f, 0.0f, 0.0f);//circle 1 point
	vector3 point2;
	vector3 point3;
	float cosine;
	float sine;
	vector3 point4 = vector3(0.0f, 0.0f, 0.0f);//circle 2 point
	vector3 point5;
	vector3 point6;
	vector3 point7;
	vector3 point8;
	vector3 point9;
	vector3 point10;
	//loop portion
	for (float angle = 0.0f; angle < 2.0f * PI; angle += step)
	{
		//inner edge
		cosine = cos(angle);
		sine = sin(angle);
		point2 = vector3(point1.x + (cosine * a_fInnerRadius), point1.y + (sine * a_fInnerRadius), point1.z);
		angle += step;
		cosine = cos(angle);
		sine = sin(angle);
		point3 = vector3(point1.x + (cosine * a_fInnerRadius), point1.y + (sine * a_fInnerRadius), point1.z);
		angle -= step;

		//outer edge
		cosine = cos(angle);
		sine = sin(angle);
		point5 = vector3(point2.x + (cosine * (a_fOuterRadius - a_fInnerRadius)/2), point2.y, point2.z + (sine * (a_fOuterRadius - a_fInnerRadius)/2));
		angle += step;
		cosine = cos(angle);
		sine = sin(angle);
		point6 = vector3(point2.x + (cosine * (a_fOuterRadius - a_fInnerRadius)/2), point2.y, point2.z + (sine * (a_fOuterRadius - a_fInnerRadius)/2));
		angle -= step;
		AddQuad(point2, point3, point5, point6);


		//circles around the loop
		for (float angle2 = 0.0f; angle2 < 2.0f * PI; angle2 += step2)
		{
			//base circle
			cosine = cos(angle2);
			sine = sin(angle2);
			point7 = vector3(point2.x + (cosine * (a_fOuterRadius - a_fInnerRadius)/2), point2.y, point2.z + (sine * (a_fOuterRadius - a_fInnerRadius)/2));
			angle2 += step2;
			cosine = cos(angle2);
			sine = sin(angle2);
			point8 = vector3(point2.x + (cosine * (a_fOuterRadius - a_fInnerRadius)/2), point2.y, point2.z + (sine * (a_fOuterRadius - a_fInnerRadius)/2));
			angle2 -= step2;
			AddTri(point7, point2, point8);
		}

		//sides
		for (float angle2 = 0.0f; angle2 < 2.0f * PI; angle2 += step2)
		{
			cosine = cos(angle2);
			sine = sin(angle2);
			point7 = vector3(point2.x + (cosine * (a_fOuterRadius - a_fInnerRadius) / 2), point2.y, point2.z + (sine * (a_fOuterRadius - a_fInnerRadius) / 2));
			point9 = vector3(point3.x + (cosine * (a_fOuterRadius - a_fInnerRadius) / 2), point3.y, point3.z + (sine * (a_fOuterRadius - a_fInnerRadius) / 2));
			angle2 += step2;
			cosine = cos(angle2);
			sine = sin(angle2);
			point8 = vector3(point2.x + (cosine * (a_fOuterRadius - a_fInnerRadius) / 2), point2.y, point2.z + (sine * (a_fOuterRadius - a_fInnerRadius) / 2));
			point10 = vector3(point3.x + (cosine * (a_fOuterRadius - a_fInnerRadius) / 2), point3.y, point3.z + (sine * (a_fOuterRadius - a_fInnerRadius) / 2));
			angle2 -= step2;
			AddQuad(point7, point9, point8, point10);
		}
	}
	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateSphere(float a_fRadius, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	//Sets minimum and maximum of subdivisions
	if (a_nSubdivisions < 1)
	{
		GenerateCube(a_fRadius * 2.0f, a_v3Color);
		return;
	}
	if (a_nSubdivisions > 6)
		a_nSubdivisions = 6;

	Release();
	Init();

	//variables
	float step = (2.0f * PI) / (float)a_nSubdivisions;
	vector3 point1 = vector3(0.0f, 0.0f, 0.0f);
	vector3 point2;
	vector3 point3;
	float cosine;
	float sine;

	//first loop
	for (float angle = 0.0f; angle < 2.0f * PI; angle += step)
	{
		//base circle
		cosine = cos(angle);
		sine = sin(angle);
		point2 = vector3(point1.x + (cosine * a_fRadius/2), point1.y + (sine * a_fRadius/2), point1.z);
		angle += step;
		cosine = cos(angle);
		sine = sin(angle);
		point3 = vector3(point1.x + (cosine * a_fRadius/2), point1.y + (sine * a_fRadius/2), point1.z);
		angle -= step;
		AddTri(point2, point1, point3);
	}
	vector3 point4 = vector3(0.0f, 0.0f, 0.0f);
	vector3 point5;
	vector3 point6;
	//second loop
	for (float angle = 0.0f; angle < 2.0f * PI; angle += step)
	{
		point4.z = -a_fRadius / 4;
		//base circle
		cosine = cos(angle);
		sine = sin(angle);
		point5 = vector3(point4.x + (cosine * a_fRadius * 5 / 8), point4.y + (sine * a_fRadius * 5 / 8), point4.z);
		angle += step;
		cosine = cos(angle);
		sine = sin(angle);
		point6 = vector3(point4.x + (cosine * a_fRadius * 5 / 8), point4.y + (sine * a_fRadius * 5 / 8), point4.z);
		angle -= step;
		AddTri(point6, point5, point4);
	}

	//sides
	for (float angle = 0.0f; angle < 2.0f * PI; angle += step)
	{
		cosine = cos(angle);
		sine = sin(angle);
		point2 = vector3(point1.x + (cosine * a_fRadius/2), point1.y + (sine * a_fRadius/2), point1.z);
		point5 = vector3(point4.x + (cosine * a_fRadius * 5 / 8), point4.y + (sine * a_fRadius * 5 / 8), point4.z);
		angle += step;
		cosine = cos(angle);
		sine = sin(angle);
		point3 = vector3(point1.x + (cosine * a_fRadius/2), point1.y + (sine * a_fRadius/2), point1.z);
		point6 = vector3(point4.x + (cosine * a_fRadius * 5 / 8), point4.y + (sine * a_fRadius * 5 / 8), point4.z);
		angle -= step;
		AddQuad(point2, point5, point3, point6);
	}
	vector3 point7 = vector3(0.0f, 0.0f, 0.0f);
	//third loop
	for (float angle = 0.0f; angle < 2.0f * PI; angle += step)
	{
		point7.z = a_fRadius/8;
		//base circle
		cosine = cos(angle);
		sine = sin(angle);
		point5 = vector3(point7.x + (cosine * a_fRadius/4), point7.y + (sine * a_fRadius/4), point7.z);
		angle += step;
		cosine = cos(angle);
		sine = sin(angle);
		point6 = vector3(point7.x + (cosine * a_fRadius/4), point7.y + (sine * a_fRadius/4), point7.z);
		angle -= step;
		AddTri(point5, point6, point7);
	}

	//sides
	for (float angle = 0.0f; angle < 2.0f * PI; angle += step)
	{
		cosine = cos(angle);
		sine = sin(angle);
		point2 = vector3(point1.x + (cosine * a_fRadius/2), point1.y + (sine * a_fRadius/2), point1.z);
		point5 = vector3(point7.x + (cosine * a_fRadius/4), point7.y + (sine * a_fRadius/4), point7.z);
		angle += step;
		cosine = cos(angle);
		sine = sin(angle);
		point3 = vector3(point1.x + (cosine * a_fRadius/2), point1.y + (sine * a_fRadius/2), point1.z);
		point6 = vector3(point7.x + (cosine * a_fRadius/4), point7.y + (sine * a_fRadius/4), point7.z);
		angle -= step;
		AddQuad(point5, point2, point6, point3);
	}
	vector3 point10 = vector3(0.0f, 0.0f, 0.0f);
	//fourth loop
	for (float angle = 0.0f; angle < 2.0f * PI; angle += step)
	{
		point10.z = -a_fRadius/2;
		//base circle
		cosine = cos(angle);
		sine = sin(angle);
		point5 = vector3(point10.x + (cosine * a_fRadius * 5 / 8), point10.y + (sine * a_fRadius * 5 / 8), point10.z);
		angle += step;
		cosine = cos(angle);
		sine = sin(angle);
		point6 = vector3(point10.x + (cosine * a_fRadius * 5 / 8), point10.y + (sine * a_fRadius * 5 / 8), point10.z);
		angle -= step;
		AddTri(point6, point5, point10);
	}

	//sides
	for (float angle = 0.0f; angle < 2.0f * PI; angle += step)
	{
		cosine = cos(angle);
		sine = sin(angle);
		point2 = vector3(point4.x + (cosine * a_fRadius * 5 / 8), point4.y + (sine * a_fRadius * 5 / 8), point4.z);
		point5 = vector3(point10.x + (cosine * a_fRadius * 5 / 8), point10.y + (sine * a_fRadius * 5 / 8), point10.z);
		angle += step;
		cosine = cos(angle);
		sine = sin(angle);
		point3 = vector3(point4.x + (cosine * a_fRadius * 5 / 8), point4.y + (sine * a_fRadius * 5 / 8), point4.z);
		point6 = vector3(point10.x + (cosine * a_fRadius * 5 / 8), point10.y + (sine * a_fRadius * 5 / 8), point10.z);
		angle -= step;
		AddQuad(point2, point5, point3, point6);
	}
	vector3 point11 = vector3(0.0f, 0.0f, 0.0f);
	//fifth loop
	for (float angle = 0.0f; angle < 2.0f * PI; angle += step)
	{
		point11.z = -a_fRadius*3/4;
		//base circle
		cosine = cos(angle);
		sine = sin(angle);
		point5 = vector3(point11.x + (cosine * a_fRadius/2), point11.y + (sine * a_fRadius/2), point11.z);
		angle += step;
		cosine = cos(angle);
		sine = sin(angle);
		point6 = vector3(point11.x + (cosine * a_fRadius/2), point11.y + (sine * a_fRadius/2), point11.z);
		angle -= step;
		AddTri(point6, point5, point11);
	}

	//sides
	for (float angle = 0.0f; angle < 2.0f * PI; angle += step)
	{
		cosine = cos(angle);
		sine = sin(angle);
		point2 = vector3(point10.x + (cosine * a_fRadius * 5 / 8), point10.y + (sine * a_fRadius * 5 / 8), point10.z);
		point5 = vector3(point11.x + (cosine * a_fRadius/2), point11.y + (sine * a_fRadius/2), point11.z);
		angle += step;
		cosine = cos(angle);
		sine = sin(angle);
		point3 = vector3(point10.x + (cosine * a_fRadius*5/8), point10.y + (sine * a_fRadius * 5 / 8), point10.z);
		point6 = vector3(point11.x + (cosine * a_fRadius/2), point11.y + (sine * a_fRadius/2), point11.z);
		angle -= step;
		AddQuad(point2, point5, point3, point6);
	}
	vector3 point12 = vector3(0.0f, 0.0f, 0.0f);
	//sixth loop
	for (float angle = 0.0f; angle < 2.0f * PI; angle += step)
	{
		point12.z = -a_fRadius*7/8;
		//base circle
		cosine = cos(angle);
		sine = sin(angle);
		point5 = vector3(point12.x + (cosine * a_fRadius/4), point12.y + (sine * a_fRadius/4), point12.z);
		angle += step;
		cosine = cos(angle);
		sine = sin(angle);
		point6 = vector3(point12.x + (cosine * a_fRadius/4), point12.y + (sine * a_fRadius/4), point12.z);
		angle -= step;
		AddTri(point6, point5, point12);
	}

	//sides
	for (float angle = 0.0f; angle < 2.0f * PI; angle += step)
	{
		cosine = cos(angle);
		sine = sin(angle);
		point2 = vector3(point11.x + (cosine * a_fRadius / 2), point11.y + (sine * a_fRadius / 2), point11.z);
		point5 = vector3(point12.x + (cosine * a_fRadius/4), point12.y + (sine * a_fRadius/4), point12.z);
		angle += step;
		cosine = cos(angle);
		sine = sin(angle);
		point3 = vector3(point11.x + (cosine * a_fRadius / 2), point11.y + (sine * a_fRadius / 2), point11.z);
		point6 = vector3(point12.x + (cosine * a_fRadius/4), point12.y + (sine * a_fRadius/4), point12.z);
		angle -= step;
		AddQuad(point2, point5, point3, point6);
	}
	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}