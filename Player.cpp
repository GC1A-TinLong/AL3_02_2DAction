#include "Player.h"

Player::Player() {}

Player::~Player() {}

void Player::Initialize(Model* model, ViewProjection* viewProjection, uint32_t& textureHandle, const Vector3& position) {
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	viewProjection_ = viewProjection;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
	// worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
}

void Player::Update() {
	// LR Movement
	if (Input::GetInstance()->PushKey(DIK_D) || Input::GetInstance()->PushKey(DIK_A)) {
		Vector3 acceleration = {};
		if (Input::GetInstance()->PushKey(DIK_D)) {
			if (lrDirection_ != LRDirection::kRight) {	// player direction
				lrDirection_ = LRDirection::kRight;
			}

			if (velocity_.x < 0.0f) {	// when pushing opposing input, attenuate velocity
				velocity_.x *= (1.0f - kAttenuation);
			}
			acceleration.x += kAcceleration;
		} else if (Input::GetInstance()->PushKey(DIK_A)) { // player direction
			if (lrDirection_ != LRDirection::kLeft) {
				lrDirection_ = LRDirection::kLeft;
			}

			if (velocity_.x > 0.0f) { // when pushing opposing input, attenuate velocity
				velocity_.x *= (1.0f - kAttenuation);
			}
			acceleration.x -= kAcceleration;
		}
		velocity_.x = velocity_.x + acceleration.x;
		velocity_.x = std::clamp(velocity_.x, -kLimitVelocty, kLimitVelocty);
	} else {
		velocity_.x *= (1.0f - kAttenuation);
		if (abs(velocity_.x) < 0.05f) { velocity_.x = 0; }
	}
	// Player's Left & Right angle table
	float destinationRotationYTable[] = {
	    std::numbers::pi_v<float> / 2.0f,
	    std::numbers::pi_v<float> * 3.0f / 2.0f,
	};
	// Get angle from status
	float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
	worldTransform_.rotation_.y = destinationRotationY;

	worldTransform_.translation_ = worldTransform_.translation_ + velocity_;
	worldTransform_.UpdateMatrix();

	ImGui::InputFloat3("Velocity", &velocity_.x, "%.3f", ImGuiInputTextFlags_ReadOnly);
}

void Player::Draw() { model_->Draw(worldTransform_, *viewProjection_, textureHandle_); }