#include "OcclusionCullingSystem.h"
#include "../Engine.h"

#include <algorithm>

OcclusionCullingSystem::OcclusionCullingSystem() : IEventListener(ENGINE.getEventHandler()), device_(nullptr), deviceContext_(nullptr), activeCamera_(nullptr) {
	std::function<void(const DirectXDeviceCreated * e)> device_created = [&](const DirectXDeviceCreated * e) {
		onDirectXDeviceCreated(e);
	};
	registerEventCallback<DirectXDeviceCreated>(device_created);

	std::function<void(const CameraCreated*) > onCameraCreated = [&](const CameraCreated* e) {
		onCameraCreatedEvent(e);
	};
	std::function<void(const CameraDestroyed*) > onCameraDestroyed = [&](const CameraDestroyed* e) {
		onCameraDestroyedEvent(e);
	};
	registerEventCallback<CameraCreated>(onCameraCreated);
	registerEventCallback<CameraDestroyed>(onCameraDestroyed);
}

OcclusionCullingSystem::~OcclusionCullingSystem() {
}

void OcclusionCullingSystem::preUpdate(float dt) {
	for (PreviouslyVisible & pv : previouslyVisibleEntities_) {
		if (!pv.bv_->isInsideFrusutm()) {
			pv.mesh_->setVisiblility(false);
		}
	}

	previouslyVisibleEntities_.clear();
	meshesToQuery_.clear();

	auto it = ENGINE.getComponentManager().begin<BoundingVolumeComponent>();
	auto end = ENGINE.getComponentManager().end<BoundingVolumeComponent>();


	while (it != end) {
		if (it->isInsideFrusutm()) {
			MeshComponent * mesh = ENGINE.getComponentManager().getComponentByEntityId<MeshComponent>(it->getOwner());
			WorldCoordinateComponent * coord = ENGINE.getComponentManager().getComponentByEntityId<WorldCoordinateComponent>(it->getOwner());
			if (mesh != nullptr && coord != nullptr) {
				meshesToQuery_.push_back(OcclusionInstance(mesh, coord));
				previouslyVisibleEntities_.push_back(PreviouslyVisible(mesh, &(*it)));
			}
		}

		++it;
	}
}

void OcclusionCullingSystem::update(float dt) {
	if (device_ == nullptr || deviceContext_ == nullptr || activeCamera_ == nullptr) {
		return;
	}

	XMFLOAT4X4 view;
	XMStoreFloat4x4(&view, activeCamera_->getViewMatrix());
	XMFLOAT3 camera_front (-view._31, -view._32, view._33);

	XMFLOAT3 camera_front_abs(abs(camera_front.x), abs(camera_front.y), abs(camera_front.z));

	if (camera_front_abs.x >= camera_front_abs.y && camera_front_abs.x >= camera_front_abs.z) {
		int move = 0;
		if (camera_front.x > 0) {
			std::sort(meshesToQuery_.begin(), meshesToQuery_.end(), [](OcclusionInstance a, OcclusionInstance b) {
				return a.coord_->getCoord().x < b.coord_->getCoord().x;
			});
		}
		else {
			std::sort(meshesToQuery_.begin(), meshesToQuery_.end(), [](OcclusionInstance a, OcclusionInstance b) {
				return a.coord_->getCoord().x > b.coord_->getCoord().x;
			});
		}

	}
	else if (camera_front_abs.y >= camera_front_abs.x && camera_front_abs.y >= camera_front_abs.z) {
		int move = 0;
		if (camera_front.y > 0) {
			std::sort(meshesToQuery_.begin(), meshesToQuery_.end(), [](OcclusionInstance a, OcclusionInstance b) {
				return a.coord_->getCoord().y < b.coord_->getCoord().y;
			});
		}
		else {
			std::sort(meshesToQuery_.begin(), meshesToQuery_.end(), [](OcclusionInstance a, OcclusionInstance b) {
				return a.coord_->getCoord().y > b.coord_->getCoord().y;
			});
		}

	}
	else if (camera_front_abs.z >= camera_front_abs.x && camera_front_abs.z >= camera_front_abs.y) {
		int move = 0;
		if (camera_front.z > 0) {
			std::sort(meshesToQuery_.begin(), meshesToQuery_.end(), [](OcclusionInstance a, OcclusionInstance b) {
				return a.coord_->getCoord().z < b.coord_->getCoord().z;
			});
		}
		else {
			std::sort(meshesToQuery_.begin(), meshesToQuery_.end(), [](OcclusionInstance a, OcclusionInstance b) {
				return a.coord_->getCoord().z > b.coord_->getCoord().z;
			});
		}

	}


	ID3D11RenderTargetView * rtv[1];
	ID3D11DepthStencilView * dsv = nullptr;
	deviceContext_->OMGetRenderTargets(1, rtv, &dsv);

	deviceContext_->OMSetRenderTargets(0, NULL, depthStencilView_.Get());
	deviceContext_->RSSetState(rasterizerState_.Get());

	deviceContext_->RSSetViewports(1, &viewport_);


	deviceContext_->ClearDepthStencilView(depthStencilView_.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	deviceContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	deviceContext_->VSSetConstantBuffers(0, 1, objectBufferVS_.getAddressOf());

	for (auto occlusion_instance : meshesToQuery_) {
		D3D11_QUERY_DESC query_desc;
		query_desc.Query = D3D11_QUERY::D3D11_QUERY_OCCLUSION;
		query_desc.MiscFlags = 0u;
		ID3D11Query * query;
		HRESULT hr = device_->CreateQuery(&query_desc, &query);
		try {

			COM_ERROR_IF_FAILED(hr, L"Failed to create query object.");
		}
		catch (COMException ex) {
			ErrorLogger::log(ex);
			break;
		}
		deviceContext_->Begin(query);

		objectBufferVS_.data_.mvpMatrix_ = occlusion_instance.coord_->getWorldMatrix() * activeCamera_->getViewMatrix() * activeCamera_->getProjectionMatrix();

		objectBufferVS_.data_.modelMatrix_ = occlusion_instance.coord_->getWorldMatrix();
		objectBufferVS_.applyChanges();

		UINT offset = 0;

		deviceContext_->IASetVertexBuffers(0, 1, occlusion_instance.mesh_->getVertexBuffer().getAddressOf(), occlusion_instance.mesh_->getVertexBuffer().stridePtr(), &offset);
		deviceContext_->IASetIndexBuffer(occlusion_instance.mesh_->getIndexBuffer().get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);

		deviceContext_->DrawIndexed(occlusion_instance.mesh_->getIndexBuffer().indicesCount(), 0, 0);

		deviceContext_->End(query);
		queriesBuffer_.push(OcclusionQuery(query, occlusion_instance.coord_->getOwner()));

	}

	
	//set render target back
	deviceContext_->OMSetRenderTargets(1, rtv, dsv);

}

void OcclusionCullingSystem::postUpdate(float dt) {
	UINT64 query_data;
	while (!queriesBuffer_.empty()) {
		if (getQueryDataIfAvaible(queriesBuffer_.front(), &query_data)) {
			if (query_data > 0) {
				BoundingVolumeComponent * bv_comp = ENGINE.getComponentManager().getComponentByEntityId<BoundingVolumeComponent>(queriesBuffer_.front().ownerId_);
				if (bv_comp != nullptr) {
					if (bv_comp->isInsideFrusutm()) {
						MeshComponent * mesh = ENGINE.getComponentManager().getComponentByEntityId<MeshComponent>(queriesBuffer_.front().ownerId_);
						if (mesh != nullptr) {
							mesh->setVisiblility(true);
						}
					}
				}
			}
			else {
				MeshComponent * mesh = ENGINE.getComponentManager().getComponentByEntityId<MeshComponent>(queriesBuffer_.front().ownerId_);
				if (mesh != nullptr) {
					mesh->setVisiblility(false);
				}
			}
			queriesBuffer_.front().query_->Release();
			queriesBuffer_.pop();
			
		}
		else {
			return;
		}
	}

}

void OcclusionCullingSystem::onDirectXDeviceCreated(const DirectXDeviceCreated * e) {
	if (device_ == nullptr && deviceContext_ == nullptr) {
		device_ = e->device_;
		deviceContext_ = e->deviceContext_;
		initialize();
	}
}

void OcclusionCullingSystem::onCameraCreatedEvent(const CameraCreated * e) {
	if (activeCamera_ == nullptr) {
		activeCamera_ = dynamic_cast<IGameCamera*>(ENGINE.getEntityManager().getEntity<GameCamera>(e->id_));
		assert(activeCamera_ != nullptr && "Falied to get IGameCamera pointer.");
	}
	else {
		assert(0 && "There should bo only one active camera");
	}
}

void OcclusionCullingSystem::onCameraDestroyedEvent(const CameraDestroyed * e) {
	activeCamera_ = nullptr;
}

void OcclusionCullingSystem::initialize() {
	try {
		UINT msaa_quality;
		device_->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &msaa_quality);
		assert(msaa_quality > 0);

		D3D11_TEXTURE2D_DESC dsbd;
		dsbd.Width = 64; //TODO parametize
		dsbd.Height = 64;
		dsbd.MipLevels = 1;
		dsbd.ArraySize = 1;
		dsbd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

		if (true) {
			dsbd.SampleDesc.Count = 4;
			dsbd.SampleDesc.Quality = msaa_quality - 1;
		}
		else {
			dsbd.SampleDesc.Count = 1;
			dsbd.SampleDesc.Quality = 0;
		}
		dsbd.Usage = D3D11_USAGE_DEFAULT;
		dsbd.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		dsbd.CPUAccessFlags = 0;
		dsbd.MiscFlags = 0;

		HRESULT hr = device_->CreateTexture2D(&dsbd, nullptr, depthStencilBuffer_.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, L"Failed to create ID3D11Texture2D for depth stancil buffer.");

		hr = device_->CreateDepthStencilView(depthStencilBuffer_.Get(), nullptr, depthStencilView_.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, L"Failed to create ID3D11DepthStencilView.");

		D3D11_RASTERIZER_DESC rast_desc;
		ZeroMemory(&rast_desc, sizeof(D3D11_RASTERIZER_DESC));
		rast_desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rast_desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;

		hr = device_->CreateRasterizerState(&rast_desc, rasterizerState_.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, L"Failed to create ID3D11RasterizerState.");

		ZeroMemory(&viewport_, sizeof(D3D11_VIEWPORT));

		viewport_.TopLeftX = 0;
		viewport_.TopLeftY = 0;
		viewport_.Height = static_cast<float>(64);
		viewport_.Width = static_cast<float>(64);
		viewport_.MinDepth = 0.0f;
		viewport_.MaxDepth = 1.0f;


		objectBufferVS_.initialize(device_, deviceContext_);
	}
	catch (COMException ex) {
		ErrorLogger::log(ex);
	}

}

bool OcclusionCullingSystem::getQueryDataIfAvaible(OcclusionQuery query, UINT64 * out_data) {
	UINT64 query_data;

	bool available = S_OK == deviceContext_->GetData(query.query_, &query_data, sizeof(UINT64), 0);
	if (available) {
		*out_data = query_data;
	}
	return available;
}

UINT64 OcclusionCullingSystem::getQueryDataBlocking(OcclusionQuery query) {
	UINT64 query_data;

	while (S_OK != deviceContext_->GetData(query.query_, &query_data, sizeof(UINT64), 0)) {}
	return query_data;
}
