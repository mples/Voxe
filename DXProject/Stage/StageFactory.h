#pragma once
#include <unordered_map>

#include "StageTypes.h"
#include "Stage.h"
#include "StageBuilder.h"

class StageFactory {
public:
	StageFactory();
	~StageFactory();

	void addBuilder(StageType type, StageBuilder * builder);
	void removeBuilder(StageType type);
	Stage * build(StageType type);

	void clearBuilders();
private:
	std::unordered_map<StageType, StageBuilder*> buildersMap_;
};

