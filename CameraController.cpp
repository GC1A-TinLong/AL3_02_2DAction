#include "CameraController.h"
#include "Player.h"

void CameraController::Initialize(ViewProjection* viewProjection, Rect moveableArea) {
	viewProjection_ = viewProjection;
	movableArea_ = moveableArea;
}

void CameraController::Update() {
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	viewProjection_->translation_ = targetWorldTransform.translation_ + targetOffset_;

	viewProjection_->translation_.x = std::clamp(viewProjection_->translation_.x, movableArea_.left, movableArea_.right);
	viewProjection_->translation_.y = std::clamp(viewProjection_->translation_.y, movableArea_.bottom, movableArea_.top);

	viewProjection_->UpdateMatrix();
}

void CameraController::Reset() {
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	viewProjection_->translation_ = targetWorldTransform.translation_ + targetOffset_;
}
