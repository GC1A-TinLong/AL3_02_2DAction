#include "DeathParticles.h"

void DeathParticles::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {
	for (auto& deathModel : this->model_) {
		assert(model);
		deathModel = model;
	}
	viewProjection_ = viewProjection;

	for (auto& worldTransform : worldTransform_) {
		worldTransform.Initialize();
		worldTransform.translation_ = position;
	}
}

void DeathParticles::Update() {
	for (uint32_t i = 0; i < kNumParticles; i++) {
		Vector3 velocity = {kSpeed, 0, 0};
	}

	for (auto& worldTransform : worldTransform_) {
		worldTransform.UpdateMatrix();
	}
}

void DeathParticles::Draw() {
	for (int i = 0; i < kNumParticles; i++) {
		model_[i]->Draw(worldTransform_[i], *viewProjection_);
	}
}
