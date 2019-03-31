#pragma once

#include<d3d11.h>
#include<wrl/client.h>
#include<vector>

#include"../ErrorLogger.h"

class AdapterData {
public:
	AdapterData(IDXGIAdapter* adapter);
	IDXGIAdapter* adapter_;
	DXGI_ADAPTER_DESC description_;
};

class AdapterReader {
public:
	AdapterReader();
	~AdapterReader();
	static std::vector<AdapterData>& getAdapters();
	static AdapterData* getMainAdapter();
private:
	static void chooseMainAdapter();
	static std::vector<AdapterData> adapters_;
	static AdapterData* mainAdapter_;
};

