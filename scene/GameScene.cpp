#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
//#include "Skydome.h"
//#include "Ground.h"

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	worldTransform_.Initialize();
	viewProjection_.Initialize();

	viewProjection_.translation_ = {0.0f, 10.0f, -20.0f};
	viewProjection_.UpdateMatrix();

#pragma region グラウンド
	// グラウンド
	groundModel_.reset(Model::CreateFromOBJ("ground", true));

	// グラウンド
	ground_ = std::make_unique<Ground>();

	ground_->Initialize(groundModel_.get());

// playerSprite_ = Sprite::Create(TextureHandle_, {100, 50});
#pragma endregion

#pragma region 自キャラ
	// プレイヤー
	//  自キャラの生成
	TextureHandle_ = TextureManager::Load("mario.jpg");

	// 3Dモデルの生成
	// modelPlayer_.reset(Model::CreateFromOBJ("Player", true));
	modelFighterHead_.reset(Model::CreateFromOBJ("float_Head", true));
	modelFighterBody_.reset(Model::CreateFromOBJ("float_Body", true));
	modelFighterL_arm_.reset(Model::CreateFromOBJ("float_L_arm", true));
	modelFighterR_arm_.reset(Model::CreateFromOBJ("float_R_arm", true));

	player_ = std::make_unique<Player>();

	//model_.reset(Model::Create());
	//player_->Initialize(model_.get(), TextureHandle_);

	player_->Initialize(
	    modelFighterBody_.get(), modelFighterHead_.get(), modelFighterL_arm_.get(),
	    modelFighterR_arm_.get());

#pragma endregion

#pragma region 天球
	
	skydomeModel_.reset(Model::CreateFromOBJ("skydome", true));

	skydome_ = std::make_unique<Skydome>();

	skydome_->Initialize(skydomeModel_.get());
#pragma endregion

	

	// フォローカメラ
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();

	// 自キャラのワールドトランスフォームを追従カメラにセット
	followCamera_->SetTarget(&player_->GetWorldTransform());
	player_->SetViewProjection(&followCamera_->GetViewProjection());

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	// 軸方向表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());
}

void GameScene::Update() 
{
	if (input_->TriggerKey(DIK_K) == isDebugCameraActive_ == false) {
		isDebugCameraActive_ = true;

	} else if (input_->TriggerKey(DIK_K) == isDebugCameraActive_ == true) {
		isDebugCameraActive_ = false;
	}

	// カメラ処理
	if (isDebugCameraActive_) {
		// followCamera_->Update();

		debugCamera_->Update();

		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;

		// viewProjection_.matView = followCamera_->GetViewProjection().matView;
		// viewProjection_.matProjection = followCamera_->GetViewProjection().matProjection;

		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();

	} else {

		// 追従カメラの更新
		// debugCamera_->Update();

		followCamera_->Update();

		viewProjection_.matView = followCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = followCamera_->GetViewProjection().matProjection;

		// viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		// viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;

		// ビュープロジェクション行列の更新と転送
		viewProjection_.TransferMatrix();
	}

	// 天球
	skydome_->Update();

	// グラウンド
	ground_->Update();

	// プレイヤー
	player_->Update();
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

	player_->Draw(viewProjection_);

	skydome_->Draw(viewProjection_);
	ground_->Draw(viewProjection_);

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
