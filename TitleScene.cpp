#include "TitleScene.h"

TitleScene::TitleScene() {}

TitleScene::~TitleScene() {}

void TitleScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();

	worldTransform_.Initialize();
	worldTransform_.translation_ = {0.0f, 10.0f, 0.0f};
	viewProjection_.Initialize();
	model_ = Model::CreateFromOBJ("title", true);
}

void TitleScene::Update() {
	//easeOut[0].pos.x = easeOut[0].start.x + (easeOut[0].end.x - easeOut[0].start.x) * EaseOut(t);
	t += 1.0f / kEndFrame;

	worldTransform_.UpdateMatrix();
#ifdef _DEBUG
	ImGui::Begin("window");
	ImGui::DragFloat3("worldTransform_", &worldTransform_.translation_.x, 0.1f);
	ImGui::End();
#endif // _DEBUG
}
void TitleScene::Draw() {
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);
	model_->Draw(worldTransform_, viewProjection_);
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion
}

float TitleScene::EaseOut() { return sin((t * std::numbers::pi_v<float>) / 2.0f); }
