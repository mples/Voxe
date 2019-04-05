#pragma once

class Stage;

class StageBuilder {
public:
	virtual ~StageBuilder() {};
	virtual Stage* build() = 0;
};

template <typename T>
class TStageBuilder : public StageBuilder {
public:
	virtual Stage * build();
};

template<typename T>
Stage * TStageBuilder<T>::build() {
	return new T();
}

