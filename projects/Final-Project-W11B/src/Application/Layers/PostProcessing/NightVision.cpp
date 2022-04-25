#include "NightVision.h"

#include "Utils/JsonGlmHelpers.h"
#include "Utils/ImGuiHelper.h"
#include "../RenderLayer.h"
#include "Application/Application.h"

#include "Gameplay/Components/SimpleToggle.h"

NightVision::NightVision():
	PostProcessingLayer::Effect(),
	_shader(nullptr)
{
	Name = "Night Vision Toggled";
	_format = RenderTargetType::ColorRgb8;

	_shader = ResourceManager::CreateAsset<ShaderProgram>(std::unordered_map<ShaderPartType, std::string>{
		{ ShaderPartType::Vertex, "shaders/vertex_shaders/fullscreen_quad.glsl" },
		{ ShaderPartType::Fragment, "shaders/fragment_shaders/post_effects/night_vision.glsl" }
	});
}

NightVision::~NightVision() = default;

void NightVision::Apply(const Framebuffer::Sptr& gBuffer)
{
	const auto& nvToggle = Application::Get().CurrentScene()->FindObjectByName("nvToggle");
	
	_shader->Bind();
	if (nvToggle != nullptr)
	{
		_shader->SetUniform("loadTime", nvToggle->Get<SimpleToggle>()->nvToggleTime);
		_shader->SetUniform("isToggleOn", nvToggle->Get<SimpleToggle>()->isNvOn);
	} 
} 

void NightVision::RenderImGui()
{
	const auto& nvToggle = Application::Get().CurrentScene()->FindObjectByName("nvToggle");

	if (nvToggle != nullptr)
	{
		ImGui::InputFloat("Load Time", &nvToggle->Get<SimpleToggle>()->nvToggleTime);
		ImGui::InputFloat("Is Toggle On", &nvToggle->Get<SimpleToggle>()->isNvOn);
	}
}

NightVision::Sptr NightVision::FromJson(const nlohmann::json& data)
{
	return NightVision::Sptr();
}

nlohmann::json NightVision::ToJson() const
{
	return nlohmann::json();
}
