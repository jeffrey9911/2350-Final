#include "KillBehaviour.h"
#include "Gameplay/Components/ComponentManager.h"
#include "Gameplay/GameObject.h"

KillBehaviour::KillBehaviour():
	IComponent(),
	_isPlayerKilled(false)
{ }

KillBehaviour::~KillBehaviour() = default;

void KillBehaviour::OnTriggerVolumeEntered(const std::shared_ptr<Gameplay::Physics::RigidBody>& trigger)
{
	if (trigger->GetGameObject()->Name == "Zelda")
	{
		std::cout << std::endl
			<< "¨€¨€¨[   ¨€¨€¨[ ¨€¨€¨€¨€¨€¨€¨[ ¨€¨€¨[   ¨€¨€¨[    ¨€¨€¨[      ¨€¨€¨€¨€¨€¨€¨[ ¨€¨€¨€¨€¨€¨€¨€¨[¨€¨€¨€¨€¨€¨€¨€¨[" << std::endl
			<< "¨^¨€¨€¨[ ¨€¨€¨X¨a¨€¨€¨X¨T¨T¨T¨€¨€¨[¨€¨€¨U   ¨€¨€¨U    ¨€¨€¨U     ¨€¨€¨X¨T¨T¨T¨€¨€¨[¨€¨€¨X¨T¨T¨T¨T¨a¨€¨€¨X¨T¨T¨T¨T¨a" << std::endl
			<< " ¨^¨€¨€¨€¨€¨X¨a ¨€¨€¨U   ¨€¨€¨U¨€¨€¨U   ¨€¨€¨U    ¨€¨€¨U     ¨€¨€¨U   ¨€¨€¨U¨€¨€¨€¨€¨€¨€¨€¨[¨€¨€¨€¨€¨€¨[  " << std::endl
			<< "  ¨^¨€¨€¨X¨a  ¨€¨€¨U   ¨€¨€¨U¨€¨€¨U   ¨€¨€¨U    ¨€¨€¨U     ¨€¨€¨U   ¨€¨€¨U¨^¨T¨T¨T¨T¨€¨€¨U¨€¨€¨X¨T¨T¨a " << std::endl
			<< "   ¨€¨€¨U   ¨^¨€¨€¨€¨€¨€¨€¨X¨a¨^¨€¨€¨€¨€¨€¨€¨X¨a    ¨€¨€¨€¨€¨€¨€¨€¨[¨^¨€¨€¨€¨€¨€¨€¨X¨a¨€¨€¨€¨€¨€¨€¨€¨U¨€¨€¨€¨€¨€¨€¨€¨[" << std::endl
			<< "   ¨^¨T¨a    ¨^¨T¨T¨T¨T¨T¨a  ¨^¨T¨T¨T¨T¨T¨a     ¨^¨T¨T¨T¨T¨T¨T¨a ¨^¨T¨T¨T¨T¨T¨a ¨^¨T¨T¨T¨T¨T¨T¨a¨^¨T¨T¨T¨T¨T¨T¨a" << std::endl
			<< std::endl;

		_isPlayerKilled = true;
		trigger->GetGameObject()->SetPostion(glm::vec3(0.0f, 0.0f, 3.0f));
		trigger->SetAngularVelocity(glm::vec3(0.0f));
		trigger->SetLinearVelocity(glm::vec3(0.0f));
	}
	else {
		_isPlayerKilled = false;
	}
}

void KillBehaviour::RenderImGui()
{
}

nlohmann::json KillBehaviour::ToJson() const
{
	return { };
}

KillBehaviour::Sptr KillBehaviour::FromJson(const nlohmann::json& blob)
{
	KillBehaviour::Sptr result = std::make_shared<KillBehaviour>();
	return result;
}


