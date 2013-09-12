#pragma once
#include "aibasestate.h"
class AIEntryState : public AIBaseState
{
public:
	AIEntryState(void);
	~AIEntryState(void);
	void HandleState(void) override;
};

