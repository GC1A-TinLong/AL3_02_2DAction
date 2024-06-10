#include "Player.h"
#include "MapChipField.h"
#include "DebugText.h"

Player::Player() {}

Player::~Player() {}

enum Corner { kBottomRight, kBottomLeft, kTopRight, kTopLeft, kNumCorner };

void Player::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position, CameraController::Rect movableArea) {
	assert(model);
	model_ = model;
	// textureHandle_ = textureHandle;
	viewProjection_ = viewProjection;
	movableArea_ = movableArea;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
}

void Player::PlayerMovement() { MovementInput(); }

void Player::MovementInput() {
	bool isLand = false;
	if (onGround_) {
		// LR Movement
		if (Input::GetInstance()->PushKey(DIK_D) || Input::GetInstance()->PushKey(DIK_A)) {
			Vector3 acceleration = {};
			if (Input::GetInstance()->PushKey(DIK_D)) {
				if (lrDirection_ != LRDirection::kRight) { // player direction
					lrDirection_ = LRDirection::kRight;
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = kTimeTurn;
				}

				if (velocity_.x < 0.0f) { // when pushing opposing input, attenuate velocity
					velocity_.x *= (1.0f - kAttenuation);
				}
				acceleration.x += kAcceleration;
			} else if (Input::GetInstance()->PushKey(DIK_A)) { // player direction
				if (lrDirection_ != LRDirection::kLeft) {
					lrDirection_ = LRDirection::kLeft;
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = kTimeTurn;
				}

				if (velocity_.x > 0.0f) { // when pushing opposing input, attenuate velocity
					velocity_.x *= (1.0f - kAttenuation);
				}
				acceleration.x -= kAcceleration;
			}
			velocity_.x = velocity_.x + acceleration.x;
			velocity_.x = std::clamp(velocity_.x, -kMaxVelocity, kMaxVelocity);
		} else {
			velocity_.x *= (1.0f - kAttenuation);
			if (fabsf(velocity_.x) < 0.05f) {
				velocity_.x = 0;
			}
		}
		// Jump
		if (Input::GetInstance()->PushKey(DIK_SPACE)) {
			velocity_.y += kJumpAcceleration;
		}
		if (velocity_.y > 0.0f) {
			onGround_ = false;
		}
		worldTransform_.translation_ = worldTransform_.translation_ + velocity_;
	} else {
		worldTransform_.translation_ = worldTransform_.translation_ + velocity_;
		// Fall speed
		velocity_.y += -kGravityAcceleration;
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
		// is falling? Collision with ground
		if (velocity_.y < 0.0f) {
			// if translation Y is lower than the ground, landed
			if (worldTransform_.translation_.y <= 2.0f) {
				worldTransform_.translation_.y = 2.0f; // prevent going underground
				isLand = true;
			}
		}
		if (isLand) {
			worldTransform_.translation_.y = 2.0f; // prevent going underground
			velocity_.x *= (1.0f - kAttenuation);
			velocity_.y = 0.0f; // reseting fall speed
			onGround_ = true;
		}
	}
}

void Player::MoveByMapCollision(CollisionMapInfo& info) { 
	MapCollision(info);
	worldTransform_.translation_ += velocity_;
}

void Player::WhenHitCeiling(const CollisionMapInfo& info) {
	if (info.isCollideCeiling) {
		DebugText::GetInstance()->ConsolePrintf("hit ceiling\n");
		velocity_.y = 0;
	}
}

Vector3 CornerPosition(const Vector3& center, Corner corner) {
	Vector3 offsetTable[kNumCorner] = {
	    {+Player::kWidth / 2.0f, -Player::kHeight / 2.0f, 0},
	    {-Player::kWidth / 2.0f, -Player::kHeight / 2.0f, 0},
	    {+Player::kWidth / 2.0f, +Player::kHeight / 2.0f, 0},
	    {-Player::kWidth / 2.0f, +Player::kHeight / 2.0f, 0},
	};
	return center + offsetTable[static_cast<uint32_t>(corner)];
}

void Player::IsCollideMapTop(CollisionMapInfo& info) {
	if (info.velocity.y <= 0) {
		return;
	}
	std::array<Vector3, kNumCorner> positionNew{};
	for (uint32_t i = 0; i < positionNew.size(); i++) {
		positionNew[i] = CornerPosition(worldTransform_.translation_ + info.velocity, static_cast<Corner>(i));
	}

	MapChipType mapChipType{};
	// check directly above top
	bool hit = false;
	// check top left collision
	IndexSet indexSet{};
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kTopLeft]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	// check top right collision
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kTopRight]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	if (hit) {
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(
			{worldTransform_.translation_.x - Player::kWidth / 2.0f, worldTransform_.translation_.y - Player::kHeight / 2.0f, 0}
		);
		Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.velocity.y = std::max(0.0f, velocity_.y);
		// record it when hitting the ceiling
		info.isCollideCeiling = true;
	}
}
void Player::IsCollideMapBottom(CollisionMapInfo& info) {
	std::array<Vector3, 4> positionNew;
	for (uint32_t i = 0; i < positionNew.size(); i++) {
		positionNew[i] = CornerPosition(worldTransform_.translation_ + info.velocity, static_cast<Corner>(i));
	}
}
void Player::IsCollideMapLeft(CollisionMapInfo& info) {
	std::array<Vector3, 4> positionNew;
	for (uint32_t i = 0; i < positionNew.size(); i++) {
		positionNew[i] = CornerPosition(worldTransform_.translation_ + info.velocity, static_cast<Corner>(i));
	}
}
void Player::IsCollideMapRight(CollisionMapInfo& info) {
	std::array<Vector3, 4> positionNew;
	for (uint32_t i = 0; i < positionNew.size(); i++) {
		positionNew[i] = CornerPosition(worldTransform_.translation_ + info.velocity, static_cast<Corner>(i));
	}
}

void Player::MapCollision(CollisionMapInfo& info) { 
	IsCollideMapTop(info);
	IsCollideMapBottom(info);
	IsCollideMapLeft(info);
	IsCollideMapRight(info);
}

void Player::Update() {
	PlayerMovement();
	worldTransform_.translation_.x = std::clamp(worldTransform_.translation_.x, movableArea_.left, movableArea_.right);
	worldTransform_.translation_.y = std::clamp(worldTransform_.translation_.y, movableArea_.bottom, movableArea_.top);

	CollisionMapInfo collisionMapInfo; // Initializing collision info
	collisionMapInfo.velocity = velocity_;

	MapCollision(collisionMapInfo);
	MoveByMapCollision(collisionMapInfo);
	WhenHitCeiling(collisionMapInfo);

	if (turnTimer_ > 0.0f) {
		turnTimer_ -= 1.0f / 60.0f;
		// Player's Left & Right angle table
		float destinationRotationYTable[] = {
		    std::numbers::pi_v<float> / 2.0f,
		    std::numbers::pi_v<float> * 3.0f / 2.0f,
		};
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
		float easing = 1 - turnTimer_ / kTimeTurn;
		float nowRotationY = std::lerp(turnFirstRotationY_, destinationRotationY, easing);
		// Get angle from status
		worldTransform_.rotation_.y = nowRotationY;
	}

	worldTransform_.UpdateMatrix();

#ifdef _DEBUG
	ImGui::InputFloat3("Velocity", &velocity_.x, "%.3f", ImGuiInputTextFlags_ReadOnly);
	ImGui::InputFloat3("Translation", &worldTransform_.translation_.x, "%.3f", ImGuiInputTextFlags_ReadOnly);
#endif // _DEBUG
}

void Player::Draw() { model_->Draw(worldTransform_, *viewProjection_, textureHandle_); }