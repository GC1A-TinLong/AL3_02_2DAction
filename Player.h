#pragma once

#define NOMINMAX
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Input.h"
#include <imgui.h>
#include "Function.h"
#include "cassert"
#include <numbers>
#include <algorithm>
#include "CameraController.h"

class MapChipField;

enum class LRDirection {
	kRight,
	kLeft,
};

class Player {
public:
	Player();

	~Player();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="model"></param>
	/// <param name="textureHandle"></param>
	/// <param name="viewProjection"></param>
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position, Rect movableArea);
	void Update();
	void Draw();

	void PlayerMovement();

	const WorldTransform& GetWorldTransform() { return worldTransform_; };
	const Vector3& GetVelocity() const { return velocity_; };

	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; };

private:
	static inline const float kAcceleration = 0.05f;
	static inline const float kAttenuation = 0.15f;
	static inline const float kMaxVelocity = 21.8f;

	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;

	ViewProjection* viewProjection_ = nullptr;

	Vector3 velocity_ = {};
	Rect movableArea_ = {};
	LRDirection lrDirection_ = LRDirection::kRight;
	float turnFirstRotationY_ = 0.0f;
	float turnTimer_ = 0;
	static inline const float kTimeTurn = 0.3f;

	static inline const float kGravityAcceleration = 0.08f;
	static inline const float kLimitFallSpeed = 1.2f;
	static inline const float kJumpAcceleration = 1.2f;
	bool onGroung_ = true;

	MapChipField* mapChipField_ = nullptr;
};