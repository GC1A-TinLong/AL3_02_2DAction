#pragma once
#include "MapChipField.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#define _USE_MATH_DEFINES
#include <math.h>

class Enemy {
public:
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position, uint32_t textureHandle);

	void Update();

	void Draw();

	// Setter
	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; };

private:
	enum class LRDirection {
		kRight,
		kLeft,
	};
	// Walking speed
	static inline const float kWalkSpeed = 0.05f;
	static inline const float kInitialWalkMotionAngle = -60.0f; // Initial angle
	static inline const float kFinalWalkMotionAngle = 120.0f;        // Final angle
	static inline const float kWalkMotionTime = 1.0f;                      // Animation loop time
	float walkTimer_ = 0.0f;
	// Velocity
	Vector3 velocity_ = {};

	MapChipField* mapChipField_ = nullptr;

	LRDirection lrDirection_ = LRDirection::kLeft;

	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	ViewProjection* viewProjection_ = nullptr;
};
