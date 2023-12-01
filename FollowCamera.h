#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "MatrixTrans.h"
#include "Input.h"

class FollowCamera
{
public:

	void Initialize();

	void Update();

	//void Draw();

	void SetTarget(const WorldTransform* target) { target_ = target; }

	const ViewProjection& GetViewProjection() { return viewProjection_; }

private:

	//ビュープロジェクション
	ViewProjection viewProjection_;

	const WorldTransform* target_ = nullptr;

};
