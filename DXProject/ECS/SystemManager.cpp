#include "SystemManager.h"

SystemManager::SystemManager() {
}

SystemManager::~SystemManager() {
	for (auto it : systemsMap_) {
		delete it.second;
	}
	systemsMap_.clear();
	systemUpdateOrder_.clear();
}

void SystemManager::update(float dt) {
	for (auto system_it : systemUpdateOrder_) {
		system_it.second->sinceLastUpdateTime_ += dt;

		system_it.second->updateReady_ = (system_it.second->updateInterval_ <= 0.0f) || (system_it.second->updateInterval_ > 0.0f && system_it.second->sinceLastUpdateTime_ > system_it.second->updateInterval_);

		if (system_it.second->active_ && system_it.second->updateReady_) {
			system_it.second->preUpdate(dt);

			system_it.second->update(dt);
			system_it.second->sinceLastUpdateTime_ = 0.0f;


			system_it.second->postUpdate(dt);
		}

	}
}
