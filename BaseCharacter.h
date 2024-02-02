#pragma once

#include "WorldTransform.h"
#include "Model.h"
#include <vector>

class BaseCharacter 
{
public:

	// 初期化
	virtual void Initialize(const std::vector<Model*>& models);

	// 更新
	virtual void Update();

	// 描画
	virtual void Draw(const ViewProjection& viewProjection);


	const WorldTransform& GetWorldTransform() { return worldTransform_; }


protected:


	// モデルデータ配列
	std::vector<Model*> models_;
	// ワールド変換データ
	WorldTransform worldTransform_;

};