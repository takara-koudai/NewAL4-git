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

class EndScene 
{
public:

	EndScene();

	~EndScene();

	void Initialize();

	void Update();

	void Draw();

	bool IsSceneEnd() { return isSceneEnd; }
	SceneType NextScene2() { return SceneType::kEndeScene; }
	SceneType NextScene() { return SceneType::kTitle; }

	// 初期化用の関数
	void Reset() { isSceneEnd = false; }

private:

	
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	// デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	// ビュープロジェクション
	ViewProjection viewProjection_;

	// ゲームオーバーのテクスチャ
	uint32_t EndTextureHandle = 0u;

	Sprite* endSprite = nullptr;

	uint32_t end = 0;

	// シーン遷移
	bool isSceneEnd = false;


};
