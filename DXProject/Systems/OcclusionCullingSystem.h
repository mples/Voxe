#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <vector>
#include <queue>

#include "../ECS/System.h"
#include "../ECS/Event/IEventListener.h"
#include "../Events/DirectXDeviceCreated.h"
#include "../Events/CameraCreated.h"
#include "../Events/CameraDestroyed.h"
#include "../Components/MeshComponent.h"
#include "../Components/WorldCoordinateComponent.h"

#include "../Graphics/ConstantBuffer.h"
#include "../Graphics/ConstantBufferTypes.h"

#include "../Entities/GameCamera.h"

class OcclusionCullingSystem : public System<OcclusionCullingSystem>, public IEventListener {
	struct OcclusionInstance {
		OcclusionInstance(MeshComponent * m, WorldCoordinateComponent * c) : mesh_(m), coord_(c) {}
		MeshComponent * mesh_;
		WorldCoordinateComponent * coord_;
	};

	struct OcclusinQuery {
		OcclusinQuery(ID3D11Query * q, MeshComponent * m) : query_(q), mesh_(m) {}
		ID3D11Query * query_;
		MeshComponent * mesh_;
	};
	
public:
	OcclusionCullingSystem();
	~OcclusionCullingSystem();

	virtual void preUpdate(float dt) override;

	virtual void update(float dt) override;

	virtual void postUpdate(float dt) override;

private:
	void onDirectXDeviceCreated(const DirectXDeviceCreated * e);
	void onCameraCreatedEvent(const CameraCreated * e);
	void onCameraDestroyedEvent(const CameraDestroyed * e);

	void initialize();

	bool getQueryDataIfAvaible(OcclusinQuery query, UINT64 * out_data);

	UINT64 getQueryDataBlocking(OcclusinQuery query);

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState_;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer_;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView_;
	ID3D11Device * device_;
	ID3D11DeviceContext * deviceContext_;

	ConstantBuffer<CB_VS_object_buffer> objectBufferVS_;

	IGameCamera * activeCamera_;

	std::vector<OcclusionInstance> meshesInsideFrustum_;
	std::queue<OcclusinQuery> queriesBuffer_;
};