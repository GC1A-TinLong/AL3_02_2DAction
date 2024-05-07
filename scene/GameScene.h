#pragma once
#include "Audio.h"
#include "DirectXCommon.h"
#include "Function.h"
#include "Input.h"
#include "Model.h"
#include "Player.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <vector>
#include "DebugCamera.h"

const int kWindowWidth = 1280;
const int kWindowHeight = 720;

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	uint32_t textureHandle_ = 0;
	Model* model_ = nullptr;
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;

#ifdef _DEBUG
	// Debug Camera
	DebugCamera* debugCamera_ = nullptr;
	bool isDebugCameraActive_ = false;
#endif // DEBUG

	// Player
	Player* player_ = nullptr;

	Model* modelBlock_ = nullptr;
	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
