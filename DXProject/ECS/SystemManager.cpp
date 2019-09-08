#include "SystemManager.h"
#include "../Utilities/JobSystem.h"
#include "../Systems/RenderSystem.h"
#include "../Systems/OcclusionCullingSystem.h"
#include "../Systems/TerrainMeshGeneratorSystem.h"

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
		ISystem * system_ptr = system_it.second;

		if (system_ptr->getTypeId() == RenderSystem::SYSTEM_TYPE_ID ) {
			if (system_it.second->active_ && system_it.second->updateReady_) {
				system_ptr->preUpdate(dt);

				system_ptr->update(dt);
				system_ptr->sinceLastUpdateTime_ = 0.0f;


				system_ptr->postUpdate(dt);
			}
		}
		else 
		{
			if (system_it.second->active_ && system_it.second->updateReady_) {
				JOB_SYSTEM.execute([&, system_ptr]() {
					system_ptr->preUpdate(dt);

					system_ptr->update(dt);
					system_ptr->sinceLastUpdateTime_ = 0.0f;


					system_ptr->postUpdate(dt);
				});
			}

		}

	}
	JOB_SYSTEM.wait();
}
