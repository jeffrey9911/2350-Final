#include "SimpleParticleFollow.h"
#include "Gameplay/Physics/RigidBody.h"
#include "Gameplay/Components/ParticleSystem.h"
#include <GLM/gtc/random.hpp>

SimpleParticleFollow::SimpleParticleFollow() :
	IComponent(),
	gobjFollow(nullptr),
	_CDTimer(5.0f)
{ }

SimpleParticleFollow::~SimpleParticleFollow() = default;

void SimpleParticleFollow::Awake()
{
}

void SimpleParticleFollow::Update(float deltaTime)
{
	_CDTimer -= deltaTime;
	if (gobjFollow != nullptr)
	{
		_followVec = gobjFollow->GetPosition();
		_followVelo = glm::length(gobjFollow->Get<Gameplay::Physics::RigidBody>()->GetLinearVelocity());
		_emitter = GetGameObject()->Get<ParticleSystem>()->GetEmitters();

		GetGameObject()->SetPostion(_followVec);
	}
}

void SimpleParticleFollow::RenderImGui()
{
}

nlohmann::json SimpleParticleFollow::ToJson() const
{
	return { };
}

SimpleParticleFollow::Sptr SimpleParticleFollow::FromJson(const nlohmann::json& blob)
{
	SimpleParticleFollow::Sptr result = std::make_shared<SimpleParticleFollow>();
	return result;
}
