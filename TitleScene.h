#pragma once
#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "DebugCamera.h"
#include"AxisIndicator.h"
#include <cassert>
#include "Scene.h"
#include "Player.h"

class TitleScene 
{
public:

	TitleScene();

	~TitleScene();

	void Initialize();

	void Update();

	void Draw();

	bool IsSceneEnd() {return isSceneEnd; }
	SceneType NextScene() { return SceneType::kGameScene; }


	void Reset() 
	{ 
		isSceneEnd = false; 

		back_ = false;

		// 文字動かす
		position_ = {550, -200};
		position2_ = {-200, 150};
		position3_ = {1350, 143};
		position4_ = {400, 750};
		position5_ = {400, 750}; 

		count = 0;
		count2 = 0;

		fadeColor_ = {1.0f, 1.0f, 1.0f, 1.0f};
	}

private:

	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	// デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	// ビュープロジェクション
	ViewProjection viewProjection_;

	// タイトルのテクスチャ
	uint32_t TitleTextureHandle = 0u;

	//タイトル動かす変数
	uint32_t TitleTextureSin = 0u;
	uint32_t TitleTextureGeki = 0u;
	uint32_t TitleTextureHa = 0u;
	uint32_t TitleTextureRT = 0u;
	uint32_t TitleTextureSt = 0u;


	Sprite* titleSprite = nullptr;
	Sprite* titleSpriteSin = nullptr;
	Sprite* titleSpriteGeki = nullptr;
	Sprite* titleSpriteHa = nullptr;
	Sprite* titleSpriteRT = nullptr;
	Sprite* titleSpriteSt = nullptr;


	uint32_t start = 0;

	// シーン遷移
	bool isSceneEnd = false;

	// プレイヤー
	Player* player_ = nullptr;

	//文字動かす初期位置
	Vector2 position_ = {550, -200};
	Vector2 position2_ = {-200, 150};
	Vector2 position3_ = {1350,143}; 
	Vector2 position4_ = {400,750};
	Vector2 position5_ = {400,750}; 

	//画面に入るスピード
	float speed_ = 30.0f;

	//文字を戻す
	bool back_ = false;

	//シーン遷移のカウント
	int count = 0;
	int count2 = 0;

	// C式
	Sprite* fadeSprite_ = nullptr;
	Vector4 fadeColor_ = {1.0f, 1.0f, 1.0f, 1.0f};

};
