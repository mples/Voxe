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
#include "../Components/OcclusionQuadsComponent.h"
#include "../Components/WorldCoordinateComponent.h"

#include "../Graphics/ConstantBuffer.h"
#include "../Graphics/ConstantBufferTypes.h"

#include "../Entities/GameCamera.h"
#include "../Utilities/SpatialHash.h"

class OcclusionCullingSystem : public System<OcclusionCullingSystem>, public IEventListener {

	struct OcclusionInstance {
		OcclusionInstance(OcclusionQuadsComponent * quad, WorldCoordinateComponent * c) : quads(quad), coord_(c) {}
		XMFLOAT3 getPos() {
			return XMFLOAT3(coord_->getCoord().x * static_cast<float>(TERRAIN_CHUNK_DIM), coord_->getCoord().y * static_cast<float>(TERRAIN_CHUNK_DIM), coord_->getCoord().z * static_cast<float>(TERRAIN_CHUNK_DIM));
		}
		OcclusionQuadsComponent * quads;
		WorldCoordinateComponent * coord_;
	};

	//struct OcclusionQuery {
	//	OcclusionQuery(ID3D11Query * q, MeshComponent * m) : query_(q), mesh_(m) {}
	//	OcclusionQuery(ID3D11Query * q, std::pair<Point, std::vector<OcclusionInstance>> b) : query_(q), bucket_(b), mesh_(nullptr) {}
	//	ID3D11Query * query_;
	//	MeshComponent * mesh_;
	//	std::pair<Point, std::vector<OcclusionInstance>> bucket_;
	//};
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

	void drawBucketBoundingVolume(Point p, float extends);

	D3D11_VIEWPORT viewport_;


	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState_;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer_;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView_;
	ID3D11Device * device_;
	ID3D11DeviceContext * deviceContext_;

	ConstantBuffer<CB_VS_object_buffer> objectBufferVS_;

	IGameCamera * activeCamera_;

	std::vector<OcclusionInstance> meshesInsideFrustum_;

	std::vector<OcclusionInstance> meshesToQuery_;
	std::queue<OcclusionQuery> queriesBuffer_;

	SpatialHash<OcclusionInstance> spatialMap_;

	std::unordered_map<Point, bool> bucketsVisiblilty_;

};