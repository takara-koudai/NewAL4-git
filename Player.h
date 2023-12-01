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

	const WorldTransform& GetWorldTransform() { return worldTransformBase_; }

	//Vector3 GetWorldPosition_() { return worldTransform_.translation_; }

	void SetViewProjection(const ViewProjection* viewprojection) 
	{
		viewProjection_ = viewprojection;
	}

	//void Initialize(Model* model,uint32_t textureHandle);

	void Initialize(Model* modelBody, Model* modelHead, Model* modelL_arm, Model* modelR_arm);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	// 浮遊ギミック初期化
	void InitializeFloatingGimmick();
	// 浮遊ギミック更新
	void UpdateFloatingGimmick();

	// 浮遊ギミックの媒介変数
	float floatingParameter_ = 0.0f;

private:

	//ワールド変換データ
	//WorldTransform worldTransform_;

	WorldTransform worldTransformBase_;
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformHead_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;

	//3Dモデル
	Model* modelFighterBody_ = nullptr;
	Model* modelFighterHead_ = nullptr;
	Model* modelFighterL_arm_ = nullptr;
	Model* modelFighterR_arm_ = nullptr;


	//モデル
	Model* playerModel_ = nullptr;

	//テクスチャ
	uint32_t playerTextureHandle_ = 0u;

	//キーボード入力
	Input* input_ = nullptr;

	//カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;

};
