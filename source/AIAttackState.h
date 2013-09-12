#pragma once
#include "aibasestate.h"
class AIAttackState : public AIBaseState
{
public:
	AIAttackState(void);
	~AIAttackState(void);
	void HandleState(void) override;
};

