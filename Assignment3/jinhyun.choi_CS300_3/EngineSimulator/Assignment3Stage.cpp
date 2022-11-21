#include "Assignment3Stage.hpp"

#include "Application.hpp"
#include "Graphic.hpp"
#include "Input.hpp"

Assignment3Stage::Assignment3Stage()
{
	GRAPHIC->CompileShader("Skybox", "Skybox.vert", "Skybox.frag", "TransformModel.glsl", nullptr);
}

void Assignment3Stage::Initialize()
{
	skyBox = new SkyBox(APPLICATION->GetWindowSize());
	GRAPHIC->SetSkyBox(skyBox);
}

void Assignment3Stage::Update()
{
	float time = (float)glfwGetTime();
	float dt = time - lastUpdateTime;

	UpdateCamera(dt);

	lastUpdateTime = time;
}

void Assignment3Stage::UpdateCamera(float dt)
{
	float moveSpeed = dt * 100;

	auto camera = GRAPHIC->GetCamera();
	if (INPUT->IsPressed(GLFW_KEY_W))
		camera->Move(moveSpeed*2, CameraMoveAxis::Front);
	if (INPUT->IsPressed(GLFW_KEY_S))
		camera->Move(moveSpeed * 2, CameraMoveAxis::Back);
	if (INPUT->IsPressed(GLFW_KEY_D))
		camera->Move(moveSpeed * 2, CameraMoveAxis::Right);
	if (INPUT->IsPressed(GLFW_KEY_A))
		camera->Move(moveSpeed * 2, CameraMoveAxis::Left);
	if (INPUT->IsPressed(GLFW_KEY_R))
		camera->ResetPosition();
	if (INPUT->IsPressed(GLFW_KEY_Q))
		camera->SetYaw(moveSpeed * 2);
	if (INPUT->IsPressed(GLFW_KEY_E))
		camera->SetYaw(-moveSpeed * 2);
	if (INPUT->IsPressed(GLFW_KEY_Z))
		camera->SetPitch(moveSpeed * 2);
	if (INPUT->IsPressed(GLFW_KEY_C))
		camera->SetPitch(-moveSpeed * 2);
}

void Assignment3Stage::Terminate()
{
}
