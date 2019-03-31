#include "AdapterReader.h"

std::vector<AdapterData> AdapterReader::adapters_;
AdapterData* AdapterReader::mainAdapter_ = nullptr;

AdapterData::AdapterData(IDXGIAdapter * adapter) : adapter_(adapter) {
	HRESULT hr = adapter->GetDesc(&description_);
	if (FAILED(hr)) {
		ErrorLogger::log(hr, L"Error: Failed to get adapter description.");
	}
}


AdapterReader::AdapterReader()  {
}


AdapterReader::~AdapterReader() {
}

std::vector<AdapterData>& AdapterReader::getAdapters() {
	if (adapters_.size() > 0) {
		return adapters_;
	}
	Microsoft::WRL::ComPtr<IDXGIFactory> factory;

	HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(factory.GetAddressOf()));
	if (FAILED(hr)) {
		ErrorLogger::log(hr, L"Error: Failed to create DXGIFactory.");
		exit(-1);
	}

	IDXGIAdapter* adapter;
	UINT index = 0;
	while (SUCCEEDED(factory->EnumAdapters(index, &adapter))) {
		adapters_.push_back(AdapterData(adapter));
		index += 1;
	}

	chooseMainAdapter();
	return adapters_;
}

AdapterData * AdapterReader::getMainAdapter() {
	if (adapters_.empty() && mainAdapter_ == nullptr) {
		getAdapters();
	}
	return mainAdapter_;
}

void AdapterReader::chooseMainAdapter() {
	for (AdapterData& adapter : adapters_) {
		if(mainAdapter_ == nullptr) {
			mainAdapter_ = &adapter;
		}
		else {
			if (adapter.description_.DedicatedVideoMemory > mainAdapter_->description_.DedicatedVideoMemory) {
				mainAdapter_ = &adapter;
			}
		}
	}
}
