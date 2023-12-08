﻿#include "Enemy.h"
#include "MatrixTrans.h"
#define _USE_MATH_DEFINES
#include <math.h>

Vector3 Enemy::GetWorldPosition() 
{
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransformBase_.matWorld_.m[3][0];
	worldPos.y = worldTransformBase_.matWorld_.m[3][1];
	worldPos.z = worldTransformBase_.matWorld_.m[3][2];

	return worldPos;
}

void Enemy::Initialize(const std::vector<Model*>& models) 
{
	// 基底クラスの初期化
	BaseCharacter::Initialize(models);

	worldTransformBase_.Initialize();
	worldTransformBody_.Initialize();
	worldTransformL_arm_.Initialize();
	worldTransformR_arm_.Initialize();

	// 親子関係結ぶ
	worldTransformBody_.parent_ = &worldTransformBase_;
	worldTransformL_arm_.parent_ = &worldTransformBody_;
	worldTransformR_arm_.parent_ = &worldTransformBody_;

	// 腕の座標指定
	worldTransformBody_.translation_.z = 8.0f;
	worldTransformL_arm_.translation_.x = -1.0f;
	worldTransformR_arm_.translation_.x = 1.0f;
	worldTransformL_arm_.translation_.y = 1.0f;
	worldTransformR_arm_.translation_.y = 1.0f;
}

void Enemy::Update() 
{
	// 速さ
	const float kSpeed = 0.2f;
	Vector3 velocity{0.0f, 0.0f, kSpeed};

	Matrix4x4 rotateMatrix = MakeRotateMatrix(worldTransformBase_.rotation_);

	// 自機のY軸回り
	worldTransformBase_.rotation_.y += 0.02f;

	// 移動ベクトルを敵の角度だけ回転
	velocity = TransformNormal(velocity, worldTransformBase_.matWorld_);

	// 移動量
	worldTransformBase_.translation_ = Add(worldTransformBase_.translation_, velocity);
	// 行列を定数バッファに転送
	worldTransformBase_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();

	BaseCharacter::Update();
}

void Enemy::Draw(const ViewProjection& viewProjection) 
{
	models_[0]->Draw(worldTransformBody_, viewProjection);
	models_[1]->Draw(worldTransformL_arm_, viewProjection);
	models_[2]->Draw(worldTransformR_arm_, viewProjection);
}
