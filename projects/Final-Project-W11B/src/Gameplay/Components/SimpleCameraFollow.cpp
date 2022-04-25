#include "Gameplay/Components/SimpleCameraFollow.h"
#include <GLFW/glfw3.h>
#define  GLM_SWIZZLE

#include "Gameplay/GameObject.h"
#include "Gameplay/Scene.h"
#include "Utils/JsonGlmHelpers.h"
#include "Utils/ImGuiHelper.h"

#include "Application/Application.h"

#include "Gameplay/InputEngine.h"

SimpleCameraFollow::SimpleCameraFollow() :
	IComponent(),
	followGObj(NULL),
	_followPos(glm::vec3(0.0f))
{ }

SimpleCameraFollow::~SimpleCameraFollow() = default;

void SimpleCameraFollow::setFollowObj(Gameplay::GameObject::Sptr gObj)
{
	followGObj = gObj;
}

void SimpleCameraFollow::Update(float deltaTime)
{
	if (followGObj != NULL)
	{
		_followPos = followGObj->GetPosition();
		GetGameObject()->LookAt(_followPos);

		glm::vec3 _camPos = GetGameObject()->GetPosition();

		float _camDist = glm::distance(glm::vec3(GetGameObject()->GetPosition().x, GetGameObject()->GetPosition().y, 0.0f),
			glm::vec3(_followPos.x, _followPos.y, 0.0f));

		if (_camDist > 2) // MAX DISTANCE
		{
			_camPos = GetGameObject()->GetPosition()
				+ glm::normalize(_followPos - GetGameObject()->GetPosition()) * deltaTime
				* _camDist;
		}

		if (_camDist < 1) // MIN DISTANCE
		{
			_camPos = GetGameObject()->GetPosition()
				+ glm::normalize(_followPos - GetGameObject()->GetPosition()) * deltaTime * -1.0f
				* (10 - _camDist);
		}

		float _diffHeight = _camPos.z - _followPos.z;
		if (_diffHeight > 2.0f) // MAX HEIGHT
		{
			_camPos.z -= deltaTime * (1.0f + _diffHeight - 2.0f);
		}

		if (_diffHeight < 1.0f) // MIN HETGHT
		{
			_camPos.z += deltaTime * (2.0f - _diffHeight);
		}

		GetGameObject()->SetPostion(_camPos);

		if (InputEngine::IsKeyDown(GLFW_KEY_R))
		{
			GetGameObject()->LookAt(_camPos + (glm::vec3(_camPos.x, _camPos.y, -_camPos.z) - _followPos));
		}
	}
}


void SimpleCameraFollow::RenderImGui()
{
}



nlohmann::json SimpleCameraFollow::ToJson() const
{
	return { };
}

SimpleCameraFollow::Sptr SimpleCameraFollow::FromJson(const nlohmann::json& blob)
{
	SimpleCameraFollow::Sptr result = std::make_shared<SimpleCameraFollow>();
	return result;
}

