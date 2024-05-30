#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <algorithm>

struct Rect {
	float left = 0.0f;
	float right = 1.0f;
	float bottom = 0.0f;
	float top = 1.0f;
};

class Player;

class CameraController {
public:
	void Initialize(ViewProjection* viewProjection, Rect moveableArea);

	void Update();

	void SetTarget(Player* target) { target_ = target; };
	void SetMovableArea(Rect area) { movableArea_ = area; };

	void Reset();

private:
	ViewProjection *viewProjection_ = nullptr;
	WorldTransform worldTransform_;

	Player* target_ = nullptr;
	Vector3 targetOffset_ = {0, 0, -40.0f};
	Rect movableArea_ = {0, 100.0f, 0, 100.0f};

	Vector3 targetPosition = {};
};
