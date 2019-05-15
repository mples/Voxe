#pragma once
#include "ISystem.h"

#include <unordered_map>
#include <cassert>
#include <map>

class SystemManager {
public:
	SystemManager();
	~SystemManager();

	template<class T, class ...ARGS>
	T* addSystem(ARGS&& ... args) {
		const unsigned int type_id = T::SYSTEM_TYPE_ID;

		auto found = systemsMap_.find(type_id);
		if (found != systemsMap_.end()) {
			return found->second;
		}

		ISystem* system = new T(std::forward<ARGS>(args)...);
		systemsMap_[type_id] = system;
		systemUpdateOrder_.insert(std::make_pair(system->priority_, system));
	} 

	template<class T>
	inline T* getSystem() const {
		auto found = systemsMap_.find(T::SYSTEM_TYPE_ID);

		return (found == systemsMap_.end()) ? dynamic_cast<T*>(found->second) : nullptr;
	}

	template<class T>
	void activateSystem() {
		const unsigned int type_id = T::SYSTEM_TYPE_ID;

		auto found = systemsMap_.find(type_id);
		if (found != systemsMap_.end()) {
			found->second->active_ = true;
		}
		else {
			assert(0 && "Trying to enable not existing system");
		}
	}

	template<class T>
	void deactivateSystem() {
		const unsigned int type_id = T::SYSTEM_TYPE_ID;

		auto found = systemsMap_.find(type_id);
		if (found != systemsMap_.end()) {
			found->second->active_ = false;
		}
		else {
			assert(0 && "Trying to disenable not existing system");
		}
	}

	template<class T>
	void setSystemUpdateInterval(float update_interval) {
		const unsigned int type_id = T::SYSTEM_TYPE_ID;

		auto found = systemsMap_.find(type_id);
		if (found != systemsMap_.end()) {
			found->second->updateInterval_ = update_interval;
		}
		else {
			assert(0 && "Trying to set system interval to system that does not existing system");
		}
	}

	template<class T>
	void setSystemPriority(SystemPriorityLevel sys_prio) {
		const unsigned int type_id = T::SYSTEM_TYPE_ID;

		auto found = systemsMap_.find(type_id);
		if (found != systemsMap_.end()) {
			found->second->priority_ = sys_prio;
		}
		else {
			assert(0 && "Trying to set system priority to system that does not existing system");
		}
	}

private:
	void update(float dt);

	SystemManager(const SystemManager& other) = delete;
	SystemManager& operator=(const SystemManager& other) = delete;

	std::unordered_map<unsigned int, ISystem*> systemsMap_;
	std::multimap<int, ISystem*> systemUpdateOrder_;

};