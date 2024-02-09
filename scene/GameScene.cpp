#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include <fstream>
#include <iostream>
#include <variant>


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

	modelWeapon_.reset(Model::CreateFromOBJ("hammer", true));
	
	// 自キャラモデル
	std::vector<Model*> playerModels = {
	    modelFighterBody_.get(), modelFighterHead_.get(), modelFighterL_arm_.get(),
	    modelFighterR_arm_.get(), modelWeapon_.get()
	};

	player_ = std::make_unique<Player>();

	// 自キャラの初期化
	player_->Initialize(playerModels);
		
#pragma endregion

#pragma region 敵

	// 敵のモデル生成
	enemyFighterBody_.reset(Model::CreateFromOBJ("needle_Body", true));
	enemyFighterL_arm_.reset(Model::CreateFromOBJ("needle_L_arm", true));
	enemyFighterR_arm_.reset(Model::CreateFromOBJ("needle_R_arm", true));

	//enemyFighterBody_.reset(Model::CreateFromOBJ("needle_Body2", true));
	//enemyFighterL_arm_.reset(Model::CreateFromOBJ("needle_L_arm2", true));
	//enemyFighterR_arm_.reset(Model::CreateFromOBJ("needle_R_arm2", true));

	// 敵のモデル
	std::vector<Model*> enemyModels = {
	    enemyFighterBody_.get(), enemyFighterL_arm_.get(), enemyFighterR_arm_.get()};

	// 敵の生成
	enemy_ = std::make_unique<Enemy>();

	// 敵の初期化
	enemy_->Initialize(enemyModels);
	
	enemy_->SetPlayer(player_.get());
	
#pragma endregion

#pragma region 天球
	
	skydomeModel_.reset(Model::CreateFromOBJ("skydome", true));

	skydome_ = std::make_unique<Skydome>();

	skydome_->Initialize(skydomeModel_.get());
	
#pragma endregion
	

	// フェード用
	uint32_t fadeTextureHandle = TextureManager::Load("fade.png");
	fadeSprite_ = Sprite::Create(fadeTextureHandle, {0, 0} ,{1,1,1,1});
	
	// フェードアウト
	uint32_t OutfadeTextureHandle = TextureManager::Load("fade.png");
	OutfadeSprite_ = Sprite::Create(fadeTextureHandle, {0, 0}, {0,0,0,0});


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

	//フェード開始
	fadeColor_.w -= 0.01f;
	fadeSprite_->SetColor(fadeColor_);


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
				
		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();

	} else {

		// 追従カメラの更新
		// debugCamera_->Update();

		followCamera_->Update();

		viewProjection_.matView = followCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = followCamera_->GetViewProjection().matProjection;
				
		// ビュープロジェクション行列の更新と転送
		viewProjection_.TransferMatrix();
	}

	// 天球
	skydome_->Update();

	// グラウンド
	ground_->Update();

	// プレイヤー
	player_->Update();

	// 敵キャラの更新
	enemy_->Update();

	//当たり判定
	AllOnCollision();

	// フェードアウト
	if (Fadecount >= 1) {

		OutfadeColor_.w += 0.01f;
		OutfadeSprite_->SetColor(OutfadeColor_);
	}

	if (OutfadeColor_.w >= 1.0f) 
	{
		isSceneEnd = true;
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

	//プレイヤー
	player_->Draw(viewProjection_);

	// 敵の描画
	enemy_->Draw(viewProjection_);

	//天球
	skydome_->Draw(viewProjection_);

	//地面
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

	//フェードの白い画面
	fadeSprite_->Draw();

	OutfadeSprite_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

//当たり判定
void GameScene::AllOnCollision() 
{ 
	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullet();

	Vector3 posA, PosB;

	posA = enemy_->GetWorldPosition();

	for (PlayerBullet* playerBullet : playerBullets) {
		PosB = playerBullet->GetWorldPosition();

		float dx;
		float dy;
		float dz;
		float distance;
		float radius = 0.5f;

		dx = (PosB.x - posA.x) * (PosB.x - posA.x);
		dy = (PosB.y - posA.y) * (PosB.y - posA.y);
		dz = (PosB.z - posA.z) * (PosB.z - posA.z);

		distance = dx + dy + dz;

		if (distance <= (radius + radius) * (radius + radius)) {
			// 敵の描画消す
			enemy_->OnCollision();

			// 次のシーンへ移動
			//isSceneEnd = true;
			Fadecount += 1;

			enemy_->Reset();
		}
	}
	
}
//諸々のリセット関数
void GameScene::Reset()
{ 
	isSceneEnd = false;

	// 自キャラモデル
	std::vector<Model*> playerModels = {
	    modelFighterBody_.get(), modelFighterHead_.get(), modelFighterL_arm_.get(),
	    modelFighterR_arm_.get(), modelWeapon_.get()
	};

	// 自キャラの初期化
	player_->Initialize(playerModels);

	// フェード用
	uint32_t fadeTextureHandle = TextureManager::Load("fade.png");
	fadeSprite_ = Sprite::Create(fadeTextureHandle, {0, 0}, {1, 1, 1, 1});
	fadeColor_ = {1.0f, 1.0f, 1.0f, 1.0f};

	// フェードアウト
	uint32_t OutfadeTextureHandle = TextureManager::Load("fade.png");
	OutfadeSprite_ = Sprite::Create(fadeTextureHandle, {0, 0}, {0, 0, 0, 0});
	OutfadeColor_ = {0.0f, 0.0f, 0.0f, 0.0f};


	Fadecount = 0;

}


