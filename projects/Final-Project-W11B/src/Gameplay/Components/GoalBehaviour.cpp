#include "GoalBehaviour.h"
#include "Gameplay/Components/ComponentManager.h"
#include "Gameplay/GameObject.h"

GoalBehaviour::GoalBehaviour():
	IComponent()
{ }

GoalBehaviour::~GoalBehaviour() = default;

void GoalBehaviour::OnTriggerVolumeEntered(const std::shared_ptr<Gameplay::Physics::RigidBody>& trigger)
{
	if (trigger->GetGameObject()->Name == "Zelda")
	{
		std::cout << std::endl
			<< "¨€¨€¨[   ¨€¨€¨[ ¨€¨€¨€¨€¨€¨€¨[ ¨€¨€¨[   ¨€¨€¨[    ¨€¨€¨[    ¨€¨€¨[¨€¨€¨[¨€¨€¨€¨[   ¨€¨€¨[" << std::endl
			<< "¨^¨€¨€¨[ ¨€¨€¨X¨a¨€¨€¨X¨T¨T¨T¨€¨€¨[¨€¨€¨U   ¨€¨€¨U    ¨€¨€¨U    ¨€¨€¨U¨€¨€¨U¨€¨€¨€¨€¨[  ¨€¨€¨U" << std::endl
			<< " ¨^¨€¨€¨€¨€¨X¨a ¨€¨€¨U   ¨€¨€¨U¨€¨€¨U   ¨€¨€¨U    ¨€¨€¨U ¨€¨[ ¨€¨€¨U¨€¨€¨U¨€¨€¨X¨€¨€¨[ ¨€¨€¨U" << std::endl
			<< "  ¨^¨€¨€¨X¨a  ¨€¨€¨U   ¨€¨€¨U¨€¨€¨U   ¨€¨€¨U    ¨€¨€¨U¨€¨€¨€¨[¨€¨€¨U¨€¨€¨U¨€¨€¨U¨^¨€¨€¨[¨€¨€¨U" << std::endl
			<< "   ¨€¨€¨U   ¨^¨€¨€¨€¨€¨€¨€¨X¨a¨^¨€¨€¨€¨€¨€¨€¨X¨a    ¨^¨€¨€¨€¨X¨€¨€¨€¨X¨a¨€¨€¨U¨€¨€¨U ¨^¨€¨€¨€¨€¨U" << std::endl
			<< "   ¨^¨T¨a    ¨^¨T¨T¨T¨T¨T¨a  ¨^¨T¨T¨T¨T¨T¨a      ¨^¨T¨T¨a¨^¨T¨T¨a ¨^¨T¨a¨^¨T¨a  ¨^¨T¨T¨T¨a" << std::endl
			<< std::endl;

		trigger->GetGameObject()->SetPostion(glm::vec3(0.0f, 0.0f, 3.0f));
		trigger->SetAngularVelocity(glm::vec3(0.0f));
		trigger->SetLinearVelocity(glm::vec3(0.0f));
	}
}

void GoalBehaviour::RenderImGui()
{ }

nlohmann::json GoalBehaviour::ToJson() const
{
	return { };
}

GoalBehaviour::Sptr GoalBehaviour::FromJson(const nlohmann::json& blob)
{
	GoalBehaviour::Sptr result = std::make_shared<GoalBehaviour>();
	return result;
}
