#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <memory>

template<class T>
class VertexBuffer {
public:
	VertexBuffer() {}

	ID3D11Buffer* get() {
		return buffer_.Get();
	}

	ID3D11Buffer* const* getAddressOf() {
		return buffer_.GetAddressOf();
	}

	UINT verticesCount() {
		return verticesCount_;
	}

	const UINT stride() {
		return stride_;
	}

	const UINT* stridePtr() {
		return &stride_;
	}

	void reset() {
		buffer_.Reset();
		verticesCount_ = 0;
	}

	HRESULT initialize(ID3D11Device* device, T* data, UINT vertices_count) {
		if (buffer_.Get() != nullptr) {
			buffer_.Reset();
		}
		verticesCount_ = vertices_count;

		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = stride_ * vertices_count;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.MiscFlags = 0;
		desc.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sdata;
		ZeroMemory(&sdata, sizeof(D3D11_SUBRESOURCE_DATA));

		sdata.pSysMem = data;

		return device->CreateBuffer(&desc, &sdata, buffer_.GetAddressOf());
	}

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer_;
	UINT stride_ = sizeof(T);
	UINT verticesCount_ = 0;
};