#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <vector>
#include <queue>

#include "../Voxel/BlockType.h"
#include "../ECS/System.h"
#include "../ECS/Event/IEventListener.h"
#include "../Events/DirectXDeviceCreated.h"
#include "../Events/CameraCreated.h"
#include "../Events/CameraDestroyed.h"
#include "../Components/MeshComponent.h"
#include "../Components/WorldCoordinateComponent.h"
#include "../Components/BoundingVolumeComponent.h"

#include "../Graphics/ConstantBuffer.h"
#include "../Graphics/ConstantBufferTypes.h"

#include "../Entities/GameCamera.h"

class OcclusionCullingSystem : public System<OcclusionCullingSystem>, public IEventListener {

	struct OcclusionInstance {
		OcclusionInstance(MeshComponent * m, WorldCoordinateComponent * c) : mesh_(m), coord_(c) {}
		XMFLOAT3 getPos() {
			return XMFLOAT3(coord_->getCoord().x * static_cast<float>(TERRAIN_CHUNK_DIM), coord_->getCoord().y * static_cast<float>(TERRAIN_CHUNK_DIM), coord_->getCoord().z * static_cast<float>(TERRAIN_CHUNK_DIM));
		}
		MeshComponent * mesh_;
		WorldCoordinateComponent * coord_;
	};

	struct PreviouslyVisible {
		PreviouslyVisible(MeshComponent * m, BoundingVolumeComponent * bv) : mesh_(m), bv_(bv) {}

		MeshComponent * mesh_;
		BoundingVolumeComponent * bv_;
	};
	struct OcclusionQuery {
		OcclusionQuery(ID3D11Query * q, EntityId id) : query_(q), ownerId_(id) {}
		ID3D11Query * query_;
		EntityId ownerId_;
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

	bool getQueryDataIfAvaible(OcclusionQuery query, UINT64 * out_data);

	UINT64 getQueryDataBlocking(OcclusionQuery query);

	D3D11_VIEWPORT viewport_;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState_;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer_;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView_;
	ID3D11Device * device_;
	ID3D11DeviceContext * deviceContext_;

	ConstantBuffer<CB_VS_object_buffer> objectBufferVS_;

	IGameCamera * activeCamera_;

	std::vector<OcclusionInstance> meshesToQuery_;
	std::queue<OcclusionQuery> queriesBuffer_;
	std::vector<PreviouslyVisible> previouslyVisibleEntities_;

	int bufferWidth_ = 128;
	int bufferHeight_ = 128;
};