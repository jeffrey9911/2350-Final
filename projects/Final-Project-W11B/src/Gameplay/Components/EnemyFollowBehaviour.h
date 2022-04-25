#pragma once
#include "IComponent.h"

#include "Gameplay/GameObject.h"

class EnemyFollowBehaviour : public Gameplay::IComponent {
public:
	typedef std::shared_ptr<EnemyFollowBehaviour> Sptr;

	EnemyFollowBehaviour();
	virtual ~EnemyFollowBehaviour();

	virtual void Awake() override;

	virtual void Update(float deltaTime) override;

public:
	virtual void RenderImGui() override;
	MAKE_TYPENAME(EnemyFollowBehaviour);
	virtual nlohmann::json ToJson() const override;
	static EnemyFollowBehaviour::Sptr FromJson(const nlohmann::json& blob);

protected:
	Gameplay::GameObject::Sptr _toFollow;
};