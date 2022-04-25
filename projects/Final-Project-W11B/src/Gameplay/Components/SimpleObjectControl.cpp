#include "Gameplay/Components/SimpleObjectControl.h"
#include <GLFW/glfw3.h>
#define  GLM_SWIZZLE
#include <GLM/gtc/quaternion.hpp>

#include "Gameplay/GameObject.h"
#include "Gameplay/Scene.h"
#include "Utils/JsonGlmHelpers.h"
#include "Utils/ImGuiHelper.h"
#include "Gameplay/InputEngine.h"
#include "Application/Application.h"

#include "Gameplay/Physics/RigidBody.h"

#include "Gameplay/Components/ParticleSystem.h"

#include <iostream>

SimpleObjectControl::SimpleObjectControl() :
	IComponent(),
	_moveSpeed(3.0f),
	cameraAttached(nullptr),
	cdTimer(0.0f)
{}

SimpleObjectControl::~SimpleObjectControl() = default;

void SimpleObjectControl::setCamera(Gameplay::GameObject::Sptr cmr)
{
	cameraAttached = cmr;
}

void SimpleObjectControl::Update(float deltaTime)
{
	if (cameraAttached != nullptr)
	{
		if (Application::Get().IsFocused) {

			// ONLY when free camera is in use
			if (!InputEngine::IsMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT)) {}

			glm::vec3 _forwardVector = GetGameObject()->GetPosition() - cameraAttached->GetPosition();
			_forwardVector = glm::normalize(_forwardVector);

			glm::quat rotX;
			glm::vec3 _moveVector = glm::vec3(0.0f);

			Gameplay::Physics::RigidBody::Sptr physics = GetGameObject()->Get<Gameplay::Physics::RigidBody>();

			if (InputEngine::IsKeyDown(GLFW_KEY_W)) {
				_moveVector = _forwardVector * _moveSpeed;
			}

			if (InputEngine::IsKeyDown(GLFW_KEY_S)) {
				rotX = glm::angleAxis(glm::radians(180.0f), glm::vec3(0, 0, 1));
				_moveVector = rotX * _forwardVector * _moveSpeed;
			}

			if (InputEngine::IsKeyDown(GLFW_KEY_A)) {
				rotX = glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 0, 1));
				_moveVector = rotX * _forwardVector * _moveSpeed;
			}

			if (InputEngine::IsKeyDown(GLFW_KEY_D)) {
				rotX = glm::angleAxis(glm::radians(-90.0f), glm::vec3(0, 0, 1));
				_moveVector = rotX * _forwardVector * _moveSpeed;
			}

			if (InputEngine::IsKeyDown(GLFW_KEY_LEFT_SHIFT)) {
				_moveVector *= 2.0f;
			}


			_moveVector.z = 0.0f;
			_moveVector *= deltaTime;

			if (physics != NULL)
			{
				physics->ApplyImpulse(_moveVector);
			}
			else
			{
				GetGameObject()->SetPostion(GetGameObject()->GetPosition() + _moveVector);
			}



			if (InputEngine::IsKeyDown(GLFW_KEY_SPACE))
			{
				cdTimer = 2.0f;
			}

			if (cdTimer >= 0.0)
			{
				GetGameObject()->Get<ParticleSystem>()->IsEnabled = true;
				cdTimer -= deltaTime;
			}
			else
			{
				GetGameObject()->Get<ParticleSystem>()->IsEnabled = false;
			}
			
		}
	}
	
}



void SimpleObjectControl::RenderImGui()
{
}

nlohmann::json SimpleObjectControl::ToJson() const
{
	return { };
}

SimpleObjectControl::Sptr SimpleObjectControl::FromJson(const nlohmann::json& blob)
{
	SimpleObjectControl::Sptr result = std::make_shared<SimpleObjectControl>();
	return result;
}
