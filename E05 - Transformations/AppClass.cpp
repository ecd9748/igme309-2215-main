#include "AppClass.h"
void Application::InitVariables(void)
{
	////Change this to your name and email
	m_sProgrammer = "Evan Davis - ecd9748@rit.edu";
	vector3 v3Position(0.0f, 0.0f, 10.0f);
	vector3 v3Target = ZERO_V3;
	vector3 v3Upward = AXIS_Y;
	m_pCameraMngr->SetPositionTargetAndUpward(v3Position, v3Target, v3Upward);

	//Allocate the memory for the Meshes
	m_pMesh = new MyMesh();
	m_pMesh->GenerateCube(1.0f, C_BLACK);
		
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();

	//Update Entity Manager
	m_pEntityMngr->Update();

	//Add objects to render list
	m_pEntityMngr->AddEntityToRenderList(-1, true);
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	//Calculate the model, view and projection matrix
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();
	matrix4 m4View = m_pCameraMngr->GetViewMatrix();

	matrix4 m4Scale = glm::scale(IDENTITY_M4, vector3(2.0f, 2.0f, 2.0f));
	static float value = 0.0f;
	matrix4 m4Translate = glm::translate(IDENTITY_M4, vector3(value, 2.0f, 3.0f));
	matrix4 m4Translate2 = glm::translate(IDENTITY_M4, vector3(value, 4.0f, 3.0f));
	matrix4 m4Translate3 = glm::translate(IDENTITY_M4, vector3(value, 6.0f, 3.0f));
	matrix4 m4Translate4 = glm::translate(IDENTITY_M4, vector3(value, 8.0f, 3.0f));

	matrix4 m4Translate5 = glm::translate(IDENTITY_M4, vector3(value - 2, 2.0f, 3.0f));
	matrix4 m4Translate6 = glm::translate(IDENTITY_M4, vector3(value - 2, 4.0f, 3.0f));
	matrix4 m4Translate7 = glm::translate(IDENTITY_M4, vector3(value - 2, 6.0f, 3.0f));
	matrix4 m4Translate8 = glm::translate(IDENTITY_M4, vector3(value - 2, 8.0f, 3.0f));

	matrix4 m4Translate9 = glm::translate(IDENTITY_M4, vector3(value + 2, 2.0f, 3.0f));
	matrix4 m4Translate10 = glm::translate(IDENTITY_M4, vector3(value + 2, 4.0f, 3.0f));
	matrix4 m4Translate11 = glm::translate(IDENTITY_M4, vector3(value + 2, 6.0f, 3.0f));
	matrix4 m4Translate12 = glm::translate(IDENTITY_M4, vector3(value + 2, 8.0f, 3.0f));

	matrix4 m4Translate13 = glm::translate(IDENTITY_M4, vector3(value - 4, 2.0f, 3.0f));
	matrix4 m4Translate14 = glm::translate(IDENTITY_M4, vector3(value - 4, 4.0f, 3.0f));
	matrix4 m4Translate15 = glm::translate(IDENTITY_M4, vector3(value - 4, 8.0f, 3.0f));
	matrix4 m4Translate16 = glm::translate(IDENTITY_M4, vector3(value + 4, 2.0f, 3.0f));
	matrix4 m4Translate17 = glm::translate(IDENTITY_M4, vector3(value + 4, 4.0f, 3.0f));
	matrix4 m4Translate18 = glm::translate(IDENTITY_M4, vector3(value + 4, 8.0f, 3.0f));

	matrix4 m4Translate19 = glm::translate(IDENTITY_M4, vector3(value - 6, 2.0f, 3.0f));
	matrix4 m4Translate20 = glm::translate(IDENTITY_M4, vector3(value - 6, 4.0f, 3.0f));
	matrix4 m4Translate21 = glm::translate(IDENTITY_M4, vector3(value - 6, 6.0f, 3.0f));
	matrix4 m4Translate22 = glm::translate(IDENTITY_M4, vector3(value - 6, 8.0f, 3.0f));
	matrix4 m4Translate23 = glm::translate(IDENTITY_M4, vector3(value + 6, 2.0f, 3.0f));
	matrix4 m4Translate24 = glm::translate(IDENTITY_M4, vector3(value + 6, 4.0f, 3.0f));
	matrix4 m4Translate25 = glm::translate(IDENTITY_M4, vector3(value + 6, 6.0f, 3.0f));
	matrix4 m4Translate26 = glm::translate(IDENTITY_M4, vector3(value + 6, 8.0f, 3.0f));
	matrix4 m4Translate27 = glm::translate(IDENTITY_M4, vector3(value + 6, 0.0f, 3.0f));
	matrix4 m4Translate28 = glm::translate(IDENTITY_M4, vector3(value - 6, 0.0f, 3.0f));

	matrix4 m4Translate29 = glm::translate(IDENTITY_M4, vector3(value + 4, 10.0f, 3.0f));
	matrix4 m4Translate30 = glm::translate(IDENTITY_M4, vector3(value - 4, 10.0f, 3.0f));
	matrix4 m4Translate31 = glm::translate(IDENTITY_M4, vector3(value + 6, 12.0f, 3.0f));
	matrix4 m4Translate32 = glm::translate(IDENTITY_M4, vector3(value - 6, 12.0f, 3.0f));
	matrix4 m4Translate33 = glm::translate(IDENTITY_M4, vector3(value + 4, -2.0f, 3.0f));
	matrix4 m4Translate34 = glm::translate(IDENTITY_M4, vector3(value - 4, -2.0f, 3.0f));
	matrix4 m4Translate35 = glm::translate(IDENTITY_M4, vector3(value + 2, -2.0f, 3.0f));
	matrix4 m4Translate36 = glm::translate(IDENTITY_M4, vector3(value - 2, -2.0f, 3.0f));

	matrix4 m4Translate37 = glm::translate(IDENTITY_M4, vector3(value + 8, 6.0f, 3.0f));
	matrix4 m4Translate38 = glm::translate(IDENTITY_M4, vector3(value - 8, 6.0f, 3.0f));
	matrix4 m4Translate39 = glm::translate(IDENTITY_M4, vector3(value + 8, 4.0f, 3.0f));
	matrix4 m4Translate40 = glm::translate(IDENTITY_M4, vector3(value - 8, 4.0f, 3.0f));
	matrix4 m4Translate41 = glm::translate(IDENTITY_M4, vector3(value + 10, 4.0f, 3.0f));
	matrix4 m4Translate42 = glm::translate(IDENTITY_M4, vector3(value - 10, 4.0f, 3.0f));
	matrix4 m4Translate43 = glm::translate(IDENTITY_M4, vector3(value + 10, 2.0f, 3.0f));
	matrix4 m4Translate44 = glm::translate(IDENTITY_M4, vector3(value - 10, 2.0f, 3.0f));
	matrix4 m4Translate45 = glm::translate(IDENTITY_M4, vector3(value + 10, 0.0f, 3.0f));
	matrix4 m4Translate46 = glm::translate(IDENTITY_M4, vector3(value - 10, 0.0f, 3.0f));
	value += 0.01f;

	matrix4 m4Model = m4Translate * m4Scale;
	matrix4 m4Model2 = m4Translate2 * m4Scale;
	matrix4 m4Model3 = m4Translate3 * m4Scale;
	matrix4 m4Model4 = m4Translate4 * m4Scale;

	matrix4 m4Model5 = m4Translate5 * m4Scale;
	matrix4 m4Model6 = m4Translate6 * m4Scale;
	matrix4 m4Model7 = m4Translate7 * m4Scale;
	matrix4 m4Model8 = m4Translate8 * m4Scale;

	matrix4 m4Model9 = m4Translate9 * m4Scale;
	matrix4 m4Model10 = m4Translate10 * m4Scale;
	matrix4 m4Model11 = m4Translate11 * m4Scale;
	matrix4 m4Model12 = m4Translate12 * m4Scale;

	matrix4 m4Model13 = m4Translate13 * m4Scale;
	matrix4 m4Model14 = m4Translate14 * m4Scale;
	matrix4 m4Model15 = m4Translate15 * m4Scale;
	matrix4 m4Model16 = m4Translate16 * m4Scale;
	matrix4 m4Model17 = m4Translate17 * m4Scale;
	matrix4 m4Model18 = m4Translate18 * m4Scale;

	matrix4 m4Model19 = m4Translate19 * m4Scale;
	matrix4 m4Model20 = m4Translate20 * m4Scale;
	matrix4 m4Model21 = m4Translate21 * m4Scale;
	matrix4 m4Model22 = m4Translate22 * m4Scale;
	matrix4 m4Model23 = m4Translate23 * m4Scale;
	matrix4 m4Model24 = m4Translate24 * m4Scale;
	matrix4 m4Model25 = m4Translate25 * m4Scale;
	matrix4 m4Model26 = m4Translate26 * m4Scale;
	matrix4 m4Model27 = m4Translate27 * m4Scale;
	matrix4 m4Model28 = m4Translate28 * m4Scale;

	matrix4 m4Model29 = m4Translate29 * m4Scale;
	matrix4 m4Model30 = m4Translate30 * m4Scale;
	matrix4 m4Model31 = m4Translate31 * m4Scale;
	matrix4 m4Model32 = m4Translate32 * m4Scale;
	matrix4 m4Model33 = m4Translate33 * m4Scale;
	matrix4 m4Model34 = m4Translate34 * m4Scale;
	matrix4 m4Model35 = m4Translate35 * m4Scale;
	matrix4 m4Model36 = m4Translate36 * m4Scale;

	matrix4 m4Model37 = m4Translate37 * m4Scale;
	matrix4 m4Model38 = m4Translate38 * m4Scale;
	matrix4 m4Model39 = m4Translate39 * m4Scale;
	matrix4 m4Model40 = m4Translate40 * m4Scale;
	matrix4 m4Model41 = m4Translate41 * m4Scale;
	matrix4 m4Model42 = m4Translate42 * m4Scale;
	matrix4 m4Model43 = m4Translate43 * m4Scale;
	matrix4 m4Model44 = m4Translate44 * m4Scale;
	matrix4 m4Model45 = m4Translate45 * m4Scale;
	matrix4 m4Model46 = m4Translate46 * m4Scale;

	m_pMesh->Render(m4Projection, m4View, m4Model);
	m_pMesh->Render(m4Projection, m4View, m4Model2);
	m_pMesh->Render(m4Projection, m4View, m4Model3);
	m_pMesh->Render(m4Projection, m4View, m4Model4);
	m_pMesh->Render(m4Projection, m4View, m4Model5);
	m_pMesh->Render(m4Projection, m4View, m4Model6);
	m_pMesh->Render(m4Projection, m4View, m4Model7);
	m_pMesh->Render(m4Projection, m4View, m4Model8);
	m_pMesh->Render(m4Projection, m4View, m4Model9);
	m_pMesh->Render(m4Projection, m4View, m4Model10);
	m_pMesh->Render(m4Projection, m4View, m4Model11);
	m_pMesh->Render(m4Projection, m4View, m4Model12);

	m_pMesh->Render(m4Projection, m4View, m4Model13);
	m_pMesh->Render(m4Projection, m4View, m4Model14);
	m_pMesh->Render(m4Projection, m4View, m4Model15);
	m_pMesh->Render(m4Projection, m4View, m4Model16);
	m_pMesh->Render(m4Projection, m4View, m4Model17);
	m_pMesh->Render(m4Projection, m4View, m4Model18);

	m_pMesh->Render(m4Projection, m4View, m4Model19);
	m_pMesh->Render(m4Projection, m4View, m4Model20);
	m_pMesh->Render(m4Projection, m4View, m4Model21);
	m_pMesh->Render(m4Projection, m4View, m4Model22);
	m_pMesh->Render(m4Projection, m4View, m4Model23);
	m_pMesh->Render(m4Projection, m4View, m4Model24);
	m_pMesh->Render(m4Projection, m4View, m4Model25);
	m_pMesh->Render(m4Projection, m4View, m4Model26);
	m_pMesh->Render(m4Projection, m4View, m4Model27);
	m_pMesh->Render(m4Projection, m4View, m4Model28);

	m_pMesh->Render(m4Projection, m4View, m4Model29);
	m_pMesh->Render(m4Projection, m4View, m4Model30);
	m_pMesh->Render(m4Projection, m4View, m4Model31);
	m_pMesh->Render(m4Projection, m4View, m4Model32);
	m_pMesh->Render(m4Projection, m4View, m4Model33);
	m_pMesh->Render(m4Projection, m4View, m4Model34);
	m_pMesh->Render(m4Projection, m4View, m4Model35);
	m_pMesh->Render(m4Projection, m4View, m4Model36);

	m_pMesh->Render(m4Projection, m4View, m4Model37);
	m_pMesh->Render(m4Projection, m4View, m4Model38);
	m_pMesh->Render(m4Projection, m4View, m4Model39);
	m_pMesh->Render(m4Projection, m4View, m4Model40);
	m_pMesh->Render(m4Projection, m4View, m4Model41);
	m_pMesh->Render(m4Projection, m4View, m4Model42);
	m_pMesh->Render(m4Projection, m4View, m4Model43);
	m_pMesh->Render(m4Projection, m4View, m4Model44);
	m_pMesh->Render(m4Projection, m4View, m4Model45);
	m_pMesh->Render(m4Projection, m4View, m4Model46);
	// draw a skybox
	m_pModelMngr->AddSkyboxToRenderList();

	//render list call
	m_uRenderCallCount = m_pModelMngr->Render();

	//clear the render list
	m_pModelMngr->ClearRenderList();

	//draw gui
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	//Release meshes
	SafeDelete(m_pMesh);

	//release GUI
	ShutdownGUI();
}