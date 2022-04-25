#pragma once
#include "IComponent.h"
#include "Gameplay/GameObject.h"

struct GLFWwindow;

class SimpleCameraFollow : public Gameplay::IComponent {
public:
	typedef std::shared_ptr<SimpleCameraFollow> Sptr;

	SimpleCameraFollow();
	virtual ~SimpleCameraFollow();

	void setFollowObj(Gameplay::GameObject::Sptr gObj);

	virtual void Update(float deltaTime) override;

public:
	virtual void RenderImGui() override;
	MAKE_TYPENAME(SimpleCameraFollow);
	virtual nlohmann::json ToJson() const override;
	static SimpleCameraFollow::Sptr FromJson(const nlohmann::json& blob);

protected:
	Gameplay::GameObject::Sptr followGObj;
	glm::vec3 _followPos;
};