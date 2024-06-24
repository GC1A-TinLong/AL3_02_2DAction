#pragma once
#include "Audio.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "Function.h"
#include "Input.h"
#include "Model.h"
#include "Skydome.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <vector>
#include "Player.h"
#include "MapChipField.h"
#include "CameraController.h"
#include "Enemy.h"

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

	void GenerateBlocks();
	/// <summary>
	/// Check all collisions
	/// </summary>
	void CheckAllCollisions();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	WorldTransform worldTransform_;
	ViewProjection viewProjection_;

#ifdef _DEBUG
	// Debug Camera
	DebugCamera* debugCamera_ = nullptr;
	bool isDebugCameraActive_ = false;
#endif // DEBUG

	MapChipField* mapChipField_;

	// Block model
	Model* modelBlock_ = nullptr;
	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;

	// Skydome
	Skydome* skydome_ = nullptr;
	Model* modelSkydome_ = nullptr;

	// Player
	Player* player_ = nullptr;
	uint32_t playerTexture_ = 0;
	CameraController::Rect playerMovableArea = {2.0f, 115.5f, 2.0f, 100.0f};
	Model* playerModel_ = nullptr;
	bool isPlayerHit = false;

	// Enemy
	Enemy* enemy_ = nullptr;
	Model* enemyModel_ = nullptr;
	uint32_t enemyTexture_ = 0u;

	static inline const int32_t kEnemyNum = 3;
	std::list<Enemy*> enemies_;

	// Camera
	CameraController* cameraController_ = nullptr;
	CameraController::Rect cameraMovableArea = {27.8f, 172.0f, 15.2f, 100.0f};
};
