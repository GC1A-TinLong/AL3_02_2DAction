#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "cassert"

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
	void Initialize(Model*model,uint32_t textureHandle,ViewProjection *viewProjection);
	void Update();
	void Draw();

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;

	ViewProjection* viewProjection_ = nullptr;
};
