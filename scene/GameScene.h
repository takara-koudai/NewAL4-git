#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Player.h"
#include <memory>
#include "DebugCamera.h"
#include "Skydome.h"
#include "Ground.h"
#include "FollowCamera.h"
#include "MatrixTrans.h"

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

	// ワールドトランスフォーム
	WorldTransform worldTransform_;

	// ビュープロジェクション
	ViewProjection viewProjection_;

	// デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;
	// デバッグカメラ
	bool isDebugCameraActive_ = false;

	// プレイヤーのテクスチャ
	uint32_t TextureHandle_ = 0;

	// スプライト
	Sprite* playerSprite_ = nullptr;

	// Model* model_ = nullptr;
	std::unique_ptr<Model> model_;

	// Player* player_ = nullptr;
	std::unique_ptr<Player> player_;

	// 天球
	std::unique_ptr<Skydome> skydome_;

	std::unique_ptr<Model> skydomeModel_;

	// グラウンド
	std::unique_ptr<Ground> ground_;

	std::unique_ptr<Model> groundModel_;

	// フォローカメラ
	std::unique_ptr<FollowCamera> followCamera_;

	// std::unique_ptr<Model> followCameraModel_;

	// レールカメラ
	// std::unique_ptr<RailCamera> railCamera_;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
