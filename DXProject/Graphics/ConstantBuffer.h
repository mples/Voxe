#pragma once
#include <d3d11.h>
#include <wrl/client.h>

#include "ConstantBufferTypes.h"
#include "../ErrorLogger.h"

template <class T>
class ConstantBuffer {
public:
	ConstantBuffer() {}

	ID3D11Buffer* get() {
		return buffer_.Get();
	}

	ID3D11Buffer* const* getAddressOf() {
		return buffer_.GetAddressOf();
	}

	HRESULT initialize(ID3D11Device* device, ID3D11DeviceContext* device_context) {
		if (buffer_.Get() != nullptr) {
			buffer_.Reset();
		}
		deviceContext_ = device_context;

		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.ByteWidth = static_cast<UINT>(sizeof(T) + (16 - (sizeof(T) % 16)));
		desc.StructureByteStride = 0;

		return device->CreateBuffer(&desc, 0, buffer_.GetAddressOf());
	}

	bool applyChanges() {
		D3D11_MAPPED_SUBRESOURCE mapped_res;
		HRESULT hr = deviceContext_->Map(buffer_.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res);
		if (FAILED(hr)) {
			ErrorLogger::log(hr, L"Error: Falied to map constant buffer.");
		}
		CopyMemory(mapped_res.pData, &data_, sizeof(T));
		deviceContext_->Unmap(buffer_.Get(), 0);
		return true;
	}

	T data_;
private:
	ConstantBuffer(const ConstantBuffer<T>& other);

	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer_;
	ID3D11DeviceContext* deviceContext_ = nullptr;

};