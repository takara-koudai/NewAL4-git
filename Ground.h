#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"

class Ground 
{
public:

	void Initialize(Model* model);

	void Update();

	void Draw(const ViewProjection& viewProjection);


private:

	// ワールド変換データ
	WorldTransform worldTransform_;

	//ViewProjection viewProjection_;

	// モデル
	Model* GroundModel_ = nullptr;



};
