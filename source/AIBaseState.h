#pragma once

class Enemy;
class AIBaseState
{
	Enemy* enemy;

public:
	AIBaseState(void);
	~AIBaseState(void);
	virtual void HandleState(void);
};

