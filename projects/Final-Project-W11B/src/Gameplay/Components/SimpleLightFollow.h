#pragma once
#include "IComponent.h"
#include "Gameplay/GameObject.h"

struct GLFWwindow;

class SimpleLightFollow :public Gameplay::IComponent {
public:
	typedef std::shared_ptr<SimpleLightFollow> Sptr;

	SimpleLightFollow();
	virtual ~SimpleLightFollow();

	void setFollowObj(Gameplay::GameObject::Sptr gObj);

	virtual void Update(float deltaTime) override;

public:
	virtual void RenderImGui() override;
	MAKE_TYPENAME(SimpleLightFollow);
	virtual nlohmann::json ToJson() const override;
	static SimpleLightFollow::Sptr FromJson(const nlohmann::json& blob);

protected:
	Gameplay::GameObject::Sptr followGObj;
	glm::vec3 Color;
	float _Timer;
};