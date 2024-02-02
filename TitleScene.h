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

	void Reset() { isSceneEnd = false; }


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

	Sprite* titleSprite = nullptr;

	uint32_t start = 0;

	// シーン遷移
	bool isSceneEnd = false;

	// プレイヤー
	Player* player_ = nullptr;


};
