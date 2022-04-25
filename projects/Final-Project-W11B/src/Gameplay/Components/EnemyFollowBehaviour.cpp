#include "EnemyFollowBehaviour.h"

#include "Gameplay/GameObject.h"
#include "Gameplay/Scene.h"

#include "GLM/gtc/matrix_transform.hpp"
#include "GLM/gtc/quaternion.hpp"
#include "GLM/glm.hpp"

EnemyFollowBehaviour::EnemyFollowBehaviour() :
	IComponent(),
	_toFollow(nullptr)
{ }

EnemyFollowBehaviour::~EnemyFollowBehaviour()
{
}

void EnemyFollowBehaviour::Awake()
{
	_toFollow = GetGameObject()->GetScene()->FindObjectByName("Zelda");
}

void EnemyFollowBehaviour::Update(float deltaTime)
{
	if (_toFollow != nullptr)
	{
		float _dist = glm::distance(GetGameObject()->GetPosition(), _toFollow->GetPosition());

		if (_dist < 5)
		{
			glm::vec3 _moveVector = GetGameObject()->GetPosition() - _toFollow->GetPosition();
			_moveVector = glm::normalize(-_moveVector);
			_moveVector.z = 0.0f;

			glm::mat4 rot = glm::lookAt(GetGameObject()->GetPosition(), _toFollow->GetPosition(), glm::vec3(0.0f, 0.0f, 1.0f));
			glm::quat rotX = glm::angleAxis(glm::radians(-90.0f), glm::vec3(1, 0, 0));
			glm::quat rotZ = glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 0, 1));
			GetGameObject()->SetRotation(glm::conjugate(glm::quat_cast(rot)) * rotX * rotZ);

			GetGameObject()->SetPostion(GetGameObject()->GetPosition() + (_moveVector * deltaTime));
		}
	}	
}

void EnemyFollowBehaviour::RenderImGui()
{
}

nlohmann::json EnemyFollowBehaviour::ToJson() const
{
	return { };
}

EnemyFollowBehaviour::Sptr EnemyFollowBehaviour::FromJson(const nlohmann::json& blob)
{
	EnemyFollowBehaviour::Sptr result = std::make_shared<EnemyFollowBehaviour>();
	return result;
}
