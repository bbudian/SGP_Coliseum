#pragma once
#include "aibasestate.h"
class AIIdleState :
	public AIBaseState
{
public:
	AIIdleState(void);
	~AIIdleState(void);
	void HandleState(void) override;
};

