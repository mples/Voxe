#include "GameCamera.h"
#include "../Engine.h"
#include "../Events/CameraCreated.h"
#include "../Events/CameraDestroyed.h"

#include "../Systems/FrustumCullingSystem.h"
#include "../Systems/OcclusionCullingSystem.h"
#include "../Events/TerrainChunkDestroyedEvent.h"
#include "../Events/VoxelChangeRequest.h"

#include "../Voxel/VoxelRayCast.h"

XMMATRIX GameCamera::IDENTITY_MATRIX = XMMatrixIdentity();

BoundingFrustum GameCamera::NOT_INITIALIZED_FRUSTUM = BoundingFrustum();

GameCamera::GameCamera() : cameraComponent_(nullptr) {
	cameraComponent_ = addComponent<CameraComponent>();
	ENGINE.sendEvent<CameraCreated>(id_);

	context_.addActionMapping(Action::MOVE_FRONT, KeyboardEvent(KeyboardEvent::Type::PRESS, 'W'));
	context_.addActionMapping(Action::MOVE_BACK, KeyboardEvent(KeyboardEvent::Type::PRESS, 'S'));
	context_.addActionMapping(Action::MOVE_RIGHT, KeyboardEvent(KeyboardEvent::Type::PRESS, 'D'));
	context_.addActionMapping(Action::MOVE_LEFT, KeyboardEvent(KeyboardEvent::Type::PRESS, 'A'));
	context_.addActionMapping(Action::MOVE_DOWN, KeyboardEvent(KeyboardEvent::Type::PRESS, 'Z'));
	context_.addActionMapping(Action::MOVE_UP, KeyboardEvent(KeyboardEvent::Type::PRESS, VK_SPACE));

	context_.addRangeMapping(Range::LOOK_X, RawAxis(AxisType::RAW_INPUT_MOUSE_X));
	context_.addRangeMapping(Range::LOOK_Y, RawAxis(AxisType::RAW_INPUT_MOUSE_Y));

	static float camera_speed = 0.5f;

	INPUT.addFrontContext(&context_);
	InputCallback callback = [&](MappedInput& input) {
			auto move_left = input.actions_.find(Action::MOVE_LEFT);
			if (move_left != input.actions_.end()) {
				cameraComponent_->adjustPos(cameraComponent_->getLeftVector() * camera_speed * input.dt_);
				input.actions_.erase(move_left);
			}
			auto move_right = input.actions_.find(Action::MOVE_RIGHT);
			if (move_right != input.actions_.end()) {
				cameraComponent_->adjustPos(cameraComponent_->getRightVector() * camera_speed * input.dt_);
				input.actions_.erase(move_right);
			}
			auto move_front = input.actions_.find(Action::MOVE_FRONT);
			if (move_front != input.actions_.end()) {
				cameraComponent_->adjustPos(cameraComponent_->getForwardVector() * camera_speed * input.dt_);
				input.actions_.erase(move_front);
			}
			auto move_back = input.actions_.find(Action::MOVE_BACK);
			if (move_back != input.actions_.end()) {
				cameraComponent_->adjustPos(cameraComponent_->getBackwardVector() * camera_speed * input.dt_);
				input.actions_.erase(move_back);
			}
			auto move_up = input.actions_.find(Action::MOVE_UP);
			if (move_up != input.actions_.end()) {
				cameraComponent_->adjustPos(0.0f, camera_speed * input.dt_, 0.0f);
				input.actions_.erase(move_up);
			}
			auto move_down = input.actions_.find(Action::MOVE_DOWN);
			if (move_down != input.actions_.end()) {
				cameraComponent_->adjustPos(0.0f, -camera_speed * input.dt_, 0.0f);
				input.actions_.erase(move_down);
			}

			auto look_x = input.ranges_.find(Range::LOOK_X);
			if (look_x != input.ranges_.end()) {
				cameraComponent_->adjustRot(0.0f, look_x->second* 0.001 * input.dt_, 0.0f);

				input.ranges_.erase(look_x);
			}

			auto look_y = input.ranges_.find(Range::LOOK_Y);
			if (look_y != input.ranges_.end()) {
				cameraComponent_->adjustRot(look_y->second * 0.001 * input.dt_, 0.0f, 0.0f);

				input.ranges_.erase(look_y);
			}
	};
	INPUT.addCallback(callback, InputCallbackPriority::HIGH);
}

GameCamera::GameCamera(float fov_degrees, float aspect_ratio, float near_plane, float far_plane, XMFLOAT3 look_at, XMFLOAT3 pos, XMFLOAT3 rot) : cameraComponent_(nullptr) {
	cameraComponent_ = addComponent<CameraComponent>(fov_degrees, aspect_ratio, near_plane, far_plane, look_at, pos, rot);
	ENGINE.sendEvent<CameraCreated>(id_);


	context_.addActionMapping(Action::MOVE_FRONT, KeyboardEvent(KeyboardEvent::Type::PRESS, 'W'));
	context_.addActionMapping(Action::MOVE_BACK, KeyboardEvent(KeyboardEvent::Type::PRESS, 'S'));
	context_.addActionMapping(Action::MOVE_RIGHT, KeyboardEvent(KeyboardEvent::Type::PRESS, 'D'));
	context_.addActionMapping(Action::MOVE_LEFT, KeyboardEvent(KeyboardEvent::Type::PRESS, 'A'));
	context_.addActionMapping(Action::MOVE_DOWN, KeyboardEvent(KeyboardEvent::Type::PRESS, 'Z'));
	context_.addActionMapping(Action::MOVE_UP, KeyboardEvent(KeyboardEvent::Type::PRESS, VK_SPACE));
	context_.addActionMapping(Action::CULL, KeyboardEvent(KeyboardEvent::Type::PRESS, 'C'));

	context_.addRangeMapping(Range::LOOK_X, RawAxis(AxisType::RAW_INPUT_MOUSE_X));
	context_.addRangeMapping(Range::LOOK_Y, RawAxis(AxisType::RAW_INPUT_MOUSE_Y));

	static float camera_speed = 0.05f;

	INPUT.addFrontContext(&context_);
	InputCallback callback = [&](MappedInput& input) {
		auto cull = input.actions_.find(Action::CULL);
		if (cull != input.actions_.end()) {
			/*static bool cull_flag = true;
			if(cull_flag) {
				ENGINE.getSystemManager().deactivateSystem<OcclusionCullingSystem>();
				cull_flag = false;
			}
			else {
				ENGINE.getSystemManager().activateSystem<OcclusionCullingSystem>();
				cull_flag = true;
			}*/
			XMVECTOR camera_front_vec = cameraComponent_->getForwardVector();
			XMFLOAT3 camera_f;
			XMStoreFloat3(&camera_f, camera_front_vec);
			XMINT3 chunk_coord;
			XMINT3 voxel_coord;
			if (VoxelRayCast::rayCast(cameraComponent_->getPos(), camera_f, 10000.0f, chunk_coord, voxel_coord)) {
				ENGINE.sendEvent<VoxelChangeRequest>(chunk_coord, voxel_coord, BlockType::AIR);
			}
			
			input.actions_.erase(cull);
		}

		char s[256];
		sprintf(s, "Current dt:  %f\n", input.dt_);
		OutputDebugStringA(s);

		auto move_left = input.actions_.find(Action::MOVE_LEFT);
		if (move_left != input.actions_.end()) {
			cameraComponent_->adjustPos(cameraComponent_->getLeftVector() * camera_speed * input.dt_);
			input.actions_.erase(move_left);
		}
		auto move_right = input.actions_.find(Action::MOVE_RIGHT);
		if (move_right != input.actions_.end()) {
			cameraComponent_->adjustPos(cameraComponent_->getRightVector() * camera_speed * input.dt_);
			input.actions_.erase(move_right);
		}
		auto move_front = input.actions_.find(Action::MOVE_FRONT);
		if (move_front != input.actions_.end()) {
			cameraComponent_->adjustPos(cameraComponent_->getForwardVector() * camera_speed * input.dt_);
			input.actions_.erase(move_front);
		}
		auto move_back = input.actions_.find(Action::MOVE_BACK);
		if (move_back != input.actions_.end()) {
			cameraComponent_->adjustPos(cameraComponent_->getBackwardVector() * camera_speed * input.dt_);
			input.actions_.erase(move_back);
		}
		auto move_up = input.actions_.find(Action::MOVE_UP);
		if (move_up != input.actions_.end()) {
			cameraComponent_->adjustPos(0.0f, camera_speed * input.dt_, 0.0f);
			input.actions_.erase(move_up);
		}
		auto move_down = input.actions_.find(Action::MOVE_DOWN);
		if (move_down != input.actions_.end()) {
			cameraComponent_->adjustPos(0.0f, -camera_speed * input.dt_, 0.0f);
			input.actions_.erase(move_down);
		}

		auto look_x = input.ranges_.find(Range::LOOK_X);
		if (look_x != input.ranges_.end()) {
			cameraComponent_->adjustRot(0.0f, look_x->second* 0.001 * input.dt_, 0.0f);

			input.ranges_.erase(look_x);
		}

		auto look_y = input.ranges_.find(Range::LOOK_Y);
		if (look_y != input.ranges_.end()) {
			cameraComponent_->adjustRot(look_y->second * 0.001 * input.dt_, 0.0f, 0.0f);

			input.ranges_.erase(look_y);
		}
	};
	INPUT.addCallback(callback, InputCallbackPriority::HIGH);
}

GameCamera::~GameCamera() {
	ENGINE.sendEvent<CameraDestroyed>(id_);
}

XMMATRIX & GameCamera::getViewMatrix() {
	if (cameraComponent_ != nullptr) {
		return cameraComponent_->getViewMatrix();
	}
	else {
		return IDENTITY_MATRIX;
	}
}

XMMATRIX & GameCamera::getProjectionMatrix() {
	if (cameraComponent_ != nullptr) {
		return cameraComponent_->getProjectionMatrix();
	}
	else {
		return IDENTITY_MATRIX;
	}
}

BoundingFrustum & GameCamera::getLocalSpaceFrustum() {
	if (cameraComponent_ != nullptr) {
		return cameraComponent_->getLocalSpaceFrustum();
	}
	else {
		return NOT_INITIALIZED_FRUSTUM;
	}
}

BoundingFrustum & GameCamera::getWorldSpaceFrustum() {
	if (cameraComponent_ != nullptr) {
		return cameraComponent_->getWorldSpaceFrustum();
	}
	else {
		return NOT_INITIALIZED_FRUSTUM;
	}
}

XMFLOAT3 GameCamera::getCameraPosition() {
	return cameraComponent_->getPos();
}

void GameCamera::setProjData(float fov_degrees, float aspect_ratio, float near_plane, float far_plane) {
	cameraComponent_->setProjData(fov_degrees, aspect_ratio, near_plane, far_plane);
}

