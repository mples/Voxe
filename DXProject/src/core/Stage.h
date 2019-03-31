#pragma once

class Stage {
public:
	Stage() {};
	virtual ~Stage() {};

	virtual void init() = 0;
	virtual void update(float dt) = 0;
	virtual void shutdown() = 0;
};

