#include "RailCamera.h"
#include "MatrixTrans.h"
#include "ImGuiManager.h"

void RailCamera::Initialize() {

	// ワールドトランスフォームの初期化設定
	//worldTransform_.translation_ = position;
	//worldTransform_.translation_ = radian;

	worldTransform_.Initialize();

	// ビュープロジェクション初期化
	viewProjection_.Initialize();

	// ビュープロジェクションの初期化
	viewProjection_.farZ = 3000.0f;

	
}

void RailCamera::Update() {

	// worldTransform_.translation_.z += 0.05f;

	worldTransform_.UpdateMatrix();

	// カメラオブジェクトのワールド行列を計算する
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);

	viewProjection_.UpdateMatrix();

	// カメラの座標を画面表示する処理

	// ImGui::Begin("Camera");
	// ImGui::SliderFloat3("PlayerPos", &worldTransform_.translation_.x, -10.0f, 20.0f);
	// ImGui::SliderFloat3("Playerrotate", &worldTransform_.rotation_.x, -10.0f, 20.0f);
	// ImGui::End();
}