#pragma once
#include "IComponent.h"
#include "Gameplay/GameObject.h"

#include "Gameplay/Components/RenderComponent.h"
#include "Gameplay/Material.h"
#include "Gameplay/Scene.h"
#include "Graphics/Textures/Texture3D.h"

struct GLFWwindow;

/// <summary>
/// 
/// </summary>
class SimpleToggle :public Gameplay::IComponent {
public:
	typedef std::shared_ptr<SimpleToggle> Sptr;

	SimpleToggle();
	virtual ~SimpleToggle();

	virtual void Awake() override;
	virtual void Update(float deltaTime) override;

	void addMatrial(Gameplay::Material::Sptr _matToStore, std::string _matStoreName);
	Gameplay::Material::Sptr searchMat(std::string _matNameToSearch);

	bool isAmbLight;

	float nvToggleTime;
	float isNvOn;

public:

	virtual void RenderImGui() override;
	MAKE_TYPENAME(SimpleToggle);
	virtual nlohmann::json ToJson() const override;
	static SimpleToggle::Sptr FromJson(const nlohmann::json& blob);

protected:
	float _cdTimer;
	RenderComponent::Sptr _renderer;	

	

	std::vector<Gameplay::Material::Sptr> _matStore;
	std::vector<std::string> _matName;
};