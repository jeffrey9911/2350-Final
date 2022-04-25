#pragma once
#include "IComponent.h"
#include "Gameplay/GameObject.h"
#include "Gameplay/Components/ParticleSystem.h"

struct GLFWwindow;


class SimpleParticleFollow : public Gameplay::IComponent {
public:
	typedef std::shared_ptr<SimpleParticleFollow> Sptr;

	SimpleParticleFollow();
	virtual ~SimpleParticleFollow();


	virtual void Awake() override;
	virtual void Update(float deltaTime) override;

	Gameplay::GameObject::Sptr gobjFollow;

public:

	virtual void RenderImGui() override;
	MAKE_TYPENAME(SimpleParticleFollow);
	virtual nlohmann::json ToJson() const override;
	static SimpleParticleFollow::Sptr FromJson(const nlohmann::json& blob);

protected:
	glm::vec3 _followVec;
	float _followVelo;
	std::vector<ParticleSystem::ParticleData> _emitter;
	float _CDTimer;
};