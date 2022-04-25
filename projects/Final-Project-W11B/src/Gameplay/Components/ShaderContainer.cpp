#include "ShaderContainer.h"

ShaderContainer::ShaderContainer() :
	IComponent()
{ }

ShaderContainer::~ShaderContainer() = default;



void ShaderContainer::addShader(ShaderProgram::Sptr shaderToAdd, std::string shaderNameToAdd)
{
	shaderName.push_back(shaderNameToAdd);
	shaderVec.push_back(shaderToAdd);
}

ShaderProgram::Sptr ShaderContainer::getShaderByName(std::string nameToSearch)
{
	int ind = -1;
	for (int i = 0; i < shaderName.size(); i++)
	{
		if (shaderName[i] == nameToSearch)
		{
			ind = i;
			break;
		}
	}

	if (ind != -1)
	{
		return shaderVec[ind];
	}
	else
	{
		return NULL;
	}
}

void ShaderContainer::addD3Tex(Texture3D::Sptr D3TexToAdd, std::string D3TexNameToAdd)
{
	D3TexName.push_back(D3TexNameToAdd);
	D3TexVec.push_back(D3TexToAdd);
}

Texture3D::Sptr ShaderContainer::getD3TexByName(std::string nameToSearch)
{
	int ind = -1;
	for (int i = 0; i < D3TexName.size(); i++)
	{
		if (D3TexName[i] == nameToSearch)
		{
			ind = i;
			break;
		}
	}

	if (ind != -1)
	{
		return D3TexVec[ind];
	}
	else
	{
		return NULL;
	}
}

void ShaderContainer::RenderImGui()
{
}

nlohmann::json ShaderContainer::ToJson() const
{
	return { };
}

ShaderContainer::Sptr ShaderContainer::FromJson(const nlohmann::json& blob)
{
	ShaderContainer::Sptr result = std::make_shared<ShaderContainer>();
	return result;
}
