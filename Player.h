#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Input.h"
#include <imgui.h>
#include "Function.h"
#include "cassert"
#include <numbers>
#include <algorithm>

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
	void Initialize(Model* model, ViewProjection* viewProjection,uint32_t &textureHandle, const Vector3 &position);

	void Update();
	void Draw();

private:
	static inline const float kAcceleration = 0.05f;
	static inline const float kAttenuation = 0.15f;
	static inline const float kLimitVelocty = 1.0f;

	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;

	ViewProjection* viewProjection_ = nullptr;

	Vector3 velocity_ = {};
	LRDirection lrDirection_ = LRDirection::kRight;
};