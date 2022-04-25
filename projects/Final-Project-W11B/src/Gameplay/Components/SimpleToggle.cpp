#include "SimpleToggle.h"

#include "Gameplay/GameObject.h"
#include "Gameplay/Scene.h"
#include "Utils/JsonGlmHelpers.h"
#include "Utils/ImGuiHelper.h"
#include "Gameplay/InputEngine.h"
#include "Application/Application.h"

#include "Gameplay/Components/Light.h"
#include "Gameplay/Scene.h"

// Graphics
#include "Graphics/Buffers/IndexBuffer.h"
#include "Graphics/Buffers/VertexBuffer.h"
#include "Graphics/VertexArrayObject.h"
#include "Graphics/ShaderProgram.h"
#include "Graphics/Textures/Texture2D.h"
#include "Graphics/Textures/TextureCube.h"
#include "Graphics/VertexTypes.h"
#include "Graphics/Font.h"
#include "Graphics/GuiBatcher.h"
#include "Graphics/Framebuffer.h"

#include "Utils/ResourceManager/ResourceManager.h"
#include "Gameplay/Material.h"
#include "Gameplay/Components/RenderComponent.h"
#include "Gameplay/Components/ShaderContainer.h"
#include "Gameplay/Components/ShadowCamera.h"

SimpleToggle::SimpleToggle() :
	IComponent(),
	isAmbLight(false),
	nvToggleTime(-1.0f),
	isNvOn(0.0f)
{ }

SimpleToggle::~SimpleToggle() = default;

void SimpleToggle::Awake()
{
	_cdTimer = 0.0f;
	_renderer = GetComponent<RenderComponent>();
}

void SimpleToggle::Update(float deltaTime)
{
	if (Application::Get().IsFocused)
	{
		if (_cdTimer > 0.0f)
			_cdTimer -= deltaTime;

		if (InputEngine::IsKeyDown(GLFW_KEY_1)) // Specular
		{
			if (_cdTimer <= 0.0f)
			{
				Gameplay::Material::Sptr matToChange = searchMat("SPEC");
				if (matToChange != nullptr)
				{
					_renderer->SetMaterial(matToChange);
				}

				if (!isAmbLight && GetGameObject()->Get<Light>() != nullptr)
				{
					GetGameObject()->Get<Light>()->IsEnabled = true;
				}

				_cdTimer = 1.0f;
			}
		}

		if (InputEngine::IsKeyDown(GLFW_KEY_2)) // Diffuse
		{
			if (_cdTimer <= 0.0f)
			{
				Gameplay::Material::Sptr matToChange = searchMat("DIFF");
				if (matToChange != nullptr)
				{
					_renderer->SetMaterial(matToChange);
				}

				if (!isAmbLight && GetGameObject()->Get<Light>() != nullptr)
				{
					GetGameObject()->Get<Light>()->IsEnabled = false;
				}

				_cdTimer = 1.0f;
			}
		}
		
		if (InputEngine::IsKeyDown(GLFW_KEY_3)) // Ambient
		{
			if (_cdTimer <= 0.0f)
			{
				if (GetGameObject()->Get<Light>() != nullptr)
				{
					GetGameObject()->Get<Light>()->IsEnabled = false;
				}

				if (GetGameObject()->Get<ShadowCamera>() != nullptr)
				{
					GetGameObject()->Get<ShadowCamera>()->IsEnabled = false;
				}

				_cdTimer = 1.0f;
			}
		}

		if (InputEngine::IsKeyDown(GLFW_KEY_0)) // Origin
		{
			if (_cdTimer <= 0.0f)
			{
				Gameplay::Material::Sptr matToChange = searchMat("ORIG");
				if (matToChange != nullptr)
				{
					_renderer->SetMaterial(matToChange);
				}

				if (GetGameObject()->Get<Light>() != nullptr)
				{
					GetGameObject()->Get<Light>()->IsEnabled = true;
				}

				if (GetGameObject()->Get<ShadowCamera>() != nullptr)
				{
					GetGameObject()->Get<ShadowCamera>()->IsEnabled = true;
				}

				if (!isAmbLight && GetGameObject()->Get<Light>() != nullptr)
				{
					GetGameObject()->Get<Light>()->IsEnabled = false;
				}

				_cdTimer = 1.0f;
			}
		}

		if (InputEngine::IsKeyDown(GLFW_KEY_E) && nvToggleTime >= 0.0f) // Origin
		{
			if (_cdTimer <= 0.0f)
			{
					

				if (isNvOn == 0.0f)
				{
					isNvOn = 1.0f;
					nvToggleTime = 3.0f;
				}
				else
				{
					isNvOn = 0.0f;
					nvToggleTime = 3.0f;
				}
					
				

				_cdTimer = 1.0f;
			}
		}

		if (nvToggleTime > 0.1)
		{
			nvToggleTime -= deltaTime;
		}

		if (nvToggleTime <= 0.1f && nvToggleTime >= -0.5f)
		{
			nvToggleTime == 0.0f;  
		} 
	}
}

void SimpleToggle::addMatrial(Gameplay::Material::Sptr _matToStore, std::string _matStoreName)
{
	_matStore.push_back(_matToStore);
	_matName.push_back(_matStoreName);
}

Gameplay::Material::Sptr SimpleToggle::searchMat(std::string _matNameToSearch)
{
	for (int i = 0; i < _matName.size(); i++)
	{
		if (_matName[i] == _matNameToSearch)
		{
			return _matStore[i];
		}
	}
	return nullptr;
}



void SimpleToggle::RenderImGui()
{
	if (nvToggleTime >= 0.0f)
	{
		ImGui::InputFloat("IS NV ON", &isNvOn);
		ImGui::InputFloat("NV Toggle Timer", &nvToggleTime);
	}
}

nlohmann::json SimpleToggle::ToJson() const
{
	return { };
}

SimpleToggle::Sptr SimpleToggle::FromJson(const nlohmann::json& blob)
{
	SimpleToggle::Sptr result = std::make_shared<SimpleToggle>();
	return result;
}
