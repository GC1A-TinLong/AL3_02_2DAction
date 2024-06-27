#pragma once
#include "CameraController.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <numbers>

class DeathParticles {
public:
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);

	void Update();

	void Draw();

private:
	static inline const uint32_t kNumParticles = 8;
	static inline const float kDuration = 0.8f;
	static inline const float kSpeed = 0.05f;
	static inline const float kAngleUnit = (2 * std::numbers::pi_v<float> / kNumParticles); // angle per nums of particle

	std::array<WorldTransform, kNumParticles> worldTransform_;
	std::array<Model*, kNumParticles> model_;
	ViewProjection* viewProjection_ = nullptr;
};
