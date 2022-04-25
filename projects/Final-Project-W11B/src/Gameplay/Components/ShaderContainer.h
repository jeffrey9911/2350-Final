#pragma once
#include "IComponent.h"
#include "Gameplay/GameObject.h"

#include "Gameplay/Components/RenderComponent.h"
#include "Gameplay/Material.h"
#include "Graphics/Textures/Texture3D.h"

#include <vector>

struct GLFWwindow;

class ShaderContainer :public Gameplay::IComponent {
public:
	typedef std::shared_ptr<ShaderContainer> Sptr;

	ShaderContainer();
	virtual ~ShaderContainer();
	
	std::vector<std::string> shaderName;
	std::vector<ShaderProgram::Sptr> shaderVec;

	std::vector<std::string> D3TexName;
	std::vector < Texture3D::Sptr> D3TexVec;

	void addShader(ShaderProgram::Sptr shaderToAdd, std::string shaderNameToAdd);
	ShaderProgram::Sptr getShaderByName(std::string nameToSearch);

	void addD3Tex(Texture3D::Sptr D3TexToAdd, std::string D3TexNameToAdd);
	Texture3D::Sptr getD3TexByName(std::string nameToSearch);

public:
	virtual void RenderImGui() override;
	MAKE_TYPENAME(ShaderContainer);
	virtual nlohmann::json ToJson() const override;
	static ShaderContainer::Sptr FromJson(const nlohmann::json& blob);

};