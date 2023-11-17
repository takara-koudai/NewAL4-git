#pragma once
#include "ImGuiManager.h"
#include "Model.h"
#include "WorldTransform.h"
#include "FollowCamera.h"

class RailCamera {
public:
	//<summary>
	// 初期化
	//</summary>

	void Initialize();

	//<summary>
	// 更新
	//</summary>

	void Update();

	const ViewProjection& GetViewProjection() { return viewProjection_; }

	const WorldTransform& GetWorldTransform() { return worldTransform_; }

private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	ViewProjection viewProjection_;
};
