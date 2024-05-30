#include "Player.h"

Player::Player() {}

Player::~Player() {}

void Player::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position, Rect movableArea) {
	assert(model);
	model_ = model;
	// textureHandle_ = textureHandle;
	viewProjection_ = viewProjection;
	movableArea_ = movableArea;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
}

void Player::PlayerMovement() {
	bool isLand = false;
	if (onGroung_) {
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
			onGroung_ = false;
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
			onGroung_ = true;
		}
	}
}

void Player::Update() {
	PlayerMovement();
	worldTransform_.translation_.x = std::clamp(worldTransform_.translation_.x, movableArea_.left, movableArea_.right);
	worldTransform_.translation_.y = std::clamp(worldTransform_.translation_.y, movableArea_.bottom, movableArea_.top);

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