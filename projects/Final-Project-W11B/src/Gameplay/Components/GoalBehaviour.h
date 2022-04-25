#pragma once
#include "IComponent.h"
#include "Gameplay/Physics/TriggerVolume.h"
#include "Gameplay/Components/RenderComponent.h"

class GoalBehaviour : public Gameplay::IComponent {
public:
	typedef std::shared_ptr<GoalBehaviour> Sptr;
	GoalBehaviour();
	virtual ~GoalBehaviour();

	virtual void OnTriggerVolumeEntered(const std::shared_ptr<Gameplay::Physics::RigidBody>& trigger) override;

public:
	virtual void RenderImGui() override;
	virtual nlohmann::json ToJson() const override;
	static GoalBehaviour::Sptr FromJson(const nlohmann::json& blob);
	MAKE_TYPENAME(GoalBehaviour);
};