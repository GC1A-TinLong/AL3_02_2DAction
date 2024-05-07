#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete player_;
	delete modelBlock_;

#ifdef _DEBUG
	delete debugCamera_;
#endif // _DEBUG

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	textureHandle_ = TextureManager::Load("uvChecker.png");
	model_ = Model::Create();
	worldTransform_.Initialize();
	viewProjection_.Initialize();

	debugCamera_ = new DebugCamera(kWindowWidth, kWindowHeight);

	player_ = new Player();
	player_->Initialize(model_, textureHandle_, &viewProjection_);

	modelBlock_ = Model::Create();
	const uint32_t kNumBlockVertical = 10;           // 縦要素数
	const uint32_t kNumBlockHorizontal = 20;         // 横要素数
	const float kBlockWidth = 2.0f;                  // ブロック1個分の横幅
	const float kBlockHeight = 2.0f;                 // 縦幅
	worldTransformBlocks_.resize(kNumBlockVertical); // 要素数を決める
	for (uint32_t i = 0; i < kNumBlockVertical; i++) {
		worldTransformBlocks_[i].resize(kNumBlockHorizontal);
	}
	// ブロック生成
	for (uint32_t y = 0; y < kNumBlockVertical; y++) {
		for (uint32_t x = 0; x < kNumBlockHorizontal; x++) {
			if (y % 2 == 0 && x % 2 == 0) {
				worldTransformBlocks_[y][x] = new WorldTransform();
				worldTransformBlocks_[y][x]->Initialize();
				worldTransformBlocks_[y][x]->translation_.x = kBlockWidth * x;
				worldTransformBlocks_[y][x]->translation_.y = kBlockHeight * y;
			} else if (y % 2 != 0 && x % 2 != 0) {
				worldTransformBlocks_[y][x] = new WorldTransform();
				worldTransformBlocks_[y][x]->Initialize();
				worldTransformBlocks_[y][x]->translation_.x = kBlockWidth * x;
				worldTransformBlocks_[y][x]->translation_.y = kBlockHeight * y;
			}
		}
	}
}

void GameScene::Update() {
	player_->Update();
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock) {
				continue;
			}
			worldTransformBlock->matWorld_ = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);
			worldTransformBlock->TransferMatrix();
		}
	}
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_SPACE)) {
		isDebugCameraActive_ = true;
	}
#endif // _DEBUG
	if (isDebugCameraActive_) {
		debugCamera_->Update();
		Matrix4x4 cameraViewMatrix = MakeViewMatrix(debugCamera_->GetViewProjection());
		viewProjection_.matView = cameraViewMatrix;
		Matrix4x4 cameraProjectionMatrix = MakeProjectionMatrix(debugCamera_->GetViewProjection());
		viewProjection_.matProjection = cameraProjectionMatrix;
	} else {
		viewProjection_.UpdateMatrix();
	}
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock) {
				continue;
			}
			modelBlock_->Draw(*worldTransformBlock, viewProjection_);
		}
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
