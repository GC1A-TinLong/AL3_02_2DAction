#pragma once
#include "DirectXCommon.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <imgui.h>
#include <iostream>
#include <numbers>
#include <cmath>

class TitleScene 
{
public:
	TitleScene();
	~TitleScene();

	void Initialize();
	void Update();
	void Draw();

	float EaseOut();

private:
	DirectXCommon* dxCommon_ = nullptr;

	WorldTransform worldTransform_;
	ViewProjection viewProjection_;
	Model* model_ = nullptr;

	static inline const float kEndFrame = 60.0f;

	float t = 0;
	
};
