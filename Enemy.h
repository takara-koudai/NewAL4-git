#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "BaseCharacter.h"
#include <vector>

class Enemy : public BaseCharacter
{
public:

	Vector3 GetWorldPosition();

	// 初期化
	void Initialize(const std::vector<Model*>& models) override;

	// 更新
	void Update() override;

	// 描画
	void Draw(const ViewProjection& viewProjection) override;

private:

	// ワールド変換データ
	WorldTransform worldTransformBase_;
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;


	// カメラのビュープロジェクション
	const ViewProjection* viewprojection_ = nullptr;


	// 3Dモデル
	Model* enemyFighterBody_ = nullptr;
	Model* enemyFighterL_arm_ = nullptr;
	Model* enemyFighterR_arm_ = nullptr;


	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

};
