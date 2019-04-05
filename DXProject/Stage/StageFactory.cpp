#include "StageFactory.h"



StageFactory::StageFactory() {
}


StageFactory::~StageFactory() {
	clearBuilders();
}

void StageFactory::addBuilder(StageType type, StageBuilder * builder) {
	auto found = buildersMap_.find(type);
	if (found == buildersMap_.end()) {
		buildersMap_.insert(std::make_pair(type, builder));
	}
}

void StageFactory::removeBuilder(StageType type) {
	auto found = buildersMap_.find(type);
	if (found != buildersMap_.end()) {
		delete found->second;
		buildersMap_.erase(found);
	}
}

Stage * StageFactory::build(StageType type) {
	auto found = buildersMap_.find(type);
	if (found != buildersMap_.end()) {
		return found->second->build();
	}
	return nullptr;
}

void StageFactory::clearBuilders() {
	for (auto elem : buildersMap_) {
		delete elem.second;
	}
	buildersMap_.clear();
}
