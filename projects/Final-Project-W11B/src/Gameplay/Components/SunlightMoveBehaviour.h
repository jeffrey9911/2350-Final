#pragma once
#include "IComponent.h"
#include "Gameplay/Physics/RigidBody.h"

class SunlightMoveBehaviour : public Gameplay::IComponent {
public:
	typedef std::shared_ptr<SunlightMoveBehaviour> Sptr;

	glm::vec3 Center;
	float     Angle;
	float     Radius;
	float     Speed;

	SunlightMoveBehaviour();
	virtual ~SunlightMoveBehaviour();

	virtual void Update(float deltaTime) override;

public:
	virtual void RenderImGui() override;
	MAKE_TYPENAME(SunlightMoveBehaviour);
	virtual nlohmann::json ToJson() const override;
	static SunlightMoveBehaviour::Sptr FromJson(const nlohmann::json& blob);

protected:
};