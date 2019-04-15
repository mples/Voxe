#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <vector>

class IndexBuffer {
public:
	IndexBuffer() {}

	ID3D11Buffer* get() {
		return buffer_.Get();
	}

	ID3D11Buffer* const* getAddressOf() {
		return buffer_.GetAddressOf();
	}

	UINT indicesCount() {
		return indicesCount_;
	}

	void reset() {
		buffer_.Reset();
		indicesCount_ = 0;
	}

	HRESULT initialize(ID3D11Device* device, DWORD* data, UINT indices_count) {
		if (buffer_.Get() != nullptr) {
			buffer_.Reset();
		}

		indicesCount_ = indices_count;

		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.ByteWidth = sizeof(DWORD) * indices_count;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA sdata;
		sdata.pSysMem = data;

		return device->CreateBuffer(&desc, &sdata, buffer_.GetAddressOf());
	}
private:
	IndexBuffer(const IndexBuffer& other);

	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer_;
	UINT indicesCount_;
};