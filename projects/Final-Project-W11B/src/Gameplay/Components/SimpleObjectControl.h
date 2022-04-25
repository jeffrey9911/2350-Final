#pragma once
#include "IComponent.h"
#include "Gameplay/GameObject.h"

struct GLFWwindow;

/// <summary>
/// Moving Component
/// </summary>
class SimpleObjectControl : public Gameplay::IComponent {
public:
	typedef std::shared_ptr<SimpleObjectControl> Sptr;

	SimpleObjectControl();
	virtual ~SimpleObjectControl();

	void setCamera(Gameplay::GameObject::Sptr cmr);

	virtual void Update(float deltaTime) override;

public:
	
	virtual void RenderImGui() override;
	MAKE_TYPENAME(SimpleObjectControl);
	virtual nlohmann::json ToJson() const override;
	static SimpleObjectControl::Sptr FromJson(const nlohmann::json& blob);

protected:
	float _moveSpeed;
	Gameplay::GameObject::Sptr cameraAttached;
	float cdTimer;
};