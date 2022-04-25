#pragma once
#include "IComponent.h"
#include "Gameplay/Physics/TriggerVolume.h"
#include "Gameplay/Components/RenderComponent.h"
#include "Gameplay/Physics/TriggerVolume.h"

class KillBehaviour : public Gameplay::IComponent {
public:
	typedef std::shared_ptr<KillBehaviour> Sptr;
	KillBehaviour();
	virtual ~KillBehaviour();

	virtual void OnTriggerVolumeEntered(const std::shared_ptr<Gameplay::Physics::RigidBody>& trigger) override;

public:
	virtual void RenderImGui() override;
	virtual nlohmann::json ToJson() const override;
	static KillBehaviour::Sptr FromJson(const nlohmann::json& blob);
	MAKE_TYPENAME(KillBehaviour);

protected:
	bool _isPlayerKilled;
};