#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "ViewProjection.h"
#include "Input.h"

class Player 
{
public:

	Player();
	~Player();

	// ワールド座標を取得
	// Vector3 GetWorldPosition() { return worldTransform_.translation_; }

	Vector3 GetWorldPosition();

	const WorldTransform& GetWorldTransform() { return worldTransform_; }

	Vector3 GetWorldPosition_() { return worldTransform_.translation_; }

	void SetViewProjection(const ViewProjection* viewprojection) 
	{
		viewProjection_ = viewprojection;
	}

	void Initialize(Model* model,uint32_t textureHandle);

	void Update();

	void Draw(const ViewProjection& viewProjection);


private:

	//ワールド変換データ
	WorldTransform worldTransform_;

	//モデル
	Model* playerModel_ = nullptr;

	//テクスチャ
	uint32_t playerTextureHandle_ = 0u;

	//キーボード入力
	Input* input_ = nullptr;

	//カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;

};
