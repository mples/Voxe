#pragma once

using SystemTypeId = unsigned int;

enum class SystemPriorityLevel {
	LOW = 0,
	MEDIUM = 1,
	HIGH = 2
};

class ISystem {
	friend class SystemManager;
public:
	ISystem() : priority_(SystemPriorityLevel::MEDIUM), updateInterval_(0.0f), active_(true), updateReady_(false), sinceLastUpdateTime_(0.0f) {}
	virtual ~ISystem() {};

	virtual void preUpdate(float dt) = 0;
	virtual void update(float dt) = 0;
	virtual void postUpdate(float dt) = 0;

	virtual inline SystemTypeId getTypeId() = 0;
private:
	SystemPriorityLevel priority_;
	float sinceLastUpdateTime_;
	//negative update interval makes system updates every frame
	float updateInterval_;
	bool active_;
	bool updateReady_;
};