#include "SunlightMoveBehaviour.h"
#include "Gameplay/GameObject.h"

#include "Utils/JsonGlmHelpers.h"
#include "Utils/ImGuiHelper.h"
#include "GLM/gtc/quaternion.hpp"

#include "Gameplay/Components/ShadowCamera.h"

SunlightMoveBehaviour::SunlightMoveBehaviour():
	IComponent(),
	Center(0.0f),
	Angle(0.0f),
	Radius(80.0f),
	Speed(6.0f)
{ }

SunlightMoveBehaviour::~SunlightMoveBehaviour() = default;

void SunlightMoveBehaviour::Update(float deltaTime)
{
	glm::vec3 pos = Center + glm::vec3(
		glm::sin(glm::radians(Angle)),
		glm::cos(glm::radians(Angle / 2.0f)) / Radius,
		glm::cos(glm::radians(Angle))
	) * Radius;
	Angle += Speed * deltaTime;
	if (Angle >= 360.0f)
		Angle -= 360.0f;

	GetGameObject()->SetPostion(pos);
	
	GetGameObject()->LookAt(Center);

	GetGameObject()->Get<ShadowCamera>()->Intensity = (pos.z / Radius) >= 0.0f ? (pos.z / Radius) : 0.0f;
	
}

void SunlightMoveBehaviour::RenderImGui()
{
	LABEL_LEFT(ImGui::DragFloat3, "Center ", &Center.x, 0.01f);
	LABEL_LEFT(ImGui::DragFloat, "Angle  ", &Angle, 1.0f);
	LABEL_LEFT(ImGui::DragFloat, "Radius ", &Radius, 1.00f);
	LABEL_LEFT(ImGui::DragFloat, "Speed  ", &Speed, 1.00f);
}

nlohmann::json SunlightMoveBehaviour::ToJson() const
{
	return { };
}

SunlightMoveBehaviour::Sptr SunlightMoveBehaviour::FromJson(const nlohmann::json& blob)
{
	SunlightMoveBehaviour::Sptr result = std::make_shared<SunlightMoveBehaviour>();
	return result;
}
