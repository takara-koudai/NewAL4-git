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
#include "Enemy.h"
#include "Scene.h"
#include "PlayerBullet.h"

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

	//当たり判定
	void AllOnCollision();
		
	Vector3 GetEnemyPopPos() { return enemyPopPos; }
	void SetEnemyPopPos(Vector3 pos) { enemyPopPos = pos; }

	bool IsSceneEnd() { return isSceneEnd; }
	void Reset();

	SceneType NextScene() { return SceneType::kEndeScene; }
	
	
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

	// プレイヤー
	std::unique_ptr<Player> player_;


	// 敵キャラ
	std::unique_ptr<Enemy> enemy_;

	//敵を複数に
	std::list<std::unique_ptr<Enemy>> enemies;

	// 待機タイマ
	int32_t waitTimer_ = 0;
	bool isWait = true;

	std::stringstream enemyPopCommands;
	Vector3 enemyPopPos = {};
		
	// 天球
	std::unique_ptr<Skydome> skydome_;

	std::unique_ptr<Model> skydomeModel_;
	
	// グラウンド
	std::unique_ptr<Ground> ground_;

	std::unique_ptr<Model> groundModel_;

	// フォローカメラ
	std::unique_ptr<FollowCamera> followCamera_;

	//3Dモデル
	//自機
	std::unique_ptr<Model> modelFighterBody_;
	std::unique_ptr<Model> modelFighterHead_;
	std::unique_ptr<Model> modelFighterL_arm_;
	std::unique_ptr<Model> modelFighterR_arm_;
		
	//  エネミーモデルデータ
	std::unique_ptr<Model> modelEnemy_;
	// エネミーモデル
	std::unique_ptr<Model> enemyFighterBody_;
	std::unique_ptr<Model> enemyFighterHead_;
	std::unique_ptr<Model> enemyFighterL_arm_;
	std::unique_ptr<Model> enemyFighterR_arm_;

	// 武器モデル
	std::unique_ptr<Model> modelWeapon_;
		
	//敵を倒したカウント
	int count = 0;

	//時間制限のカウント
	int endCount = 0;

	
	bool isSceneEnd = false;


	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
