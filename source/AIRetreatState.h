#pragma once
#include "aibasestate.h"
class AIRetreatState : public AIBaseState
{
public:
	AIRetreatState(void);
	~AIRetreatState(void);
	void HandleState(void) override;
};

