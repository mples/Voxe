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
	//std::vector<ISystem*> main_thread;
	//std::vector<ISystem*> other_thread;

	for (auto system_it : systemUpdateOrder_) {
		system_it.second->sinceLastUpdateTime_ += dt;

		system_it.second->updateReady_ = (system_it.second->updateInterval_ <= 0.0f) || (system_it.second->updateInterval_ > 0.0f && system_it.second->sinceLastUpdateTime_ > system_it.second->updateInterval_);
		ISystem * system_ptr = system_it.second;

		/*if (system_ptr->getTypeId() == RenderSystem::SYSTEM_TYPE_ID || system_ptr->getTypeId() == OcclusionCullingSystem::SYSTEM_TYPE_ID) {
			main_thread.push_back(system_ptr);
		}
		else {
			other_thread.push_back(system_ptr);
		}

		for (ISystem* ptr : other_thread) {
			if (system_it.second->active_ && system_it.second->updateReady_) {
				JOB_SYSTEM.execute([&, ptr]() {
					ptr->preUpdate(dt);

					ptr->update(dt);
					ptr->sinceLastUpdateTime_ = 0.0f;


					ptr->postUpdate(dt);
				});
			}
		}

		for (ISystem* ptr : main_thread) {
			if (system_it.second->active_ && system_it.second->updateReady_) {
				ptr->preUpdate(dt);

				ptr->update(dt);
				ptr->sinceLastUpdateTime_ = 0.0f;


				ptr->postUpdate(dt);
			}
		}

		JOB_SYSTEM.wait();*/

		if (system_it.second->active_ && system_it.second->updateReady_) {
			system_ptr->preUpdate(dt);

			system_ptr->update(dt);
			system_ptr->sinceLastUpdateTime_ = 0.0f;


			system_ptr->postUpdate(dt);
		}
		/*if (system_ptr->getTypeId() == RenderSystem::SYSTEM_TYPE_ID || system_ptr->getTypeId() == OcclusionCullingSystem::SYSTEM_TYPE_ID || system_ptr->getTypeId() == TerrainMeshGenerationSystem::SYSTEM_TYPE_ID) {
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

		}*/

	}
	//JOB_SYSTEM.wait();
}
