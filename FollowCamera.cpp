#include "FollowCamera.h"
#include "MatrixTrans.h"
#include <Input.h>
#include <Xinput.h>

void FollowCamera::Initialize()
{
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	viewProjection_.farZ = 1400.0f;
}

void FollowCamera::Update()
{

	if (target_)
	{
		// 追従対象からカメラまでのオフセット
		Vector3 offset = {0.0f, 2.0f, -30.0f};
		// カメラの角度から回転行列を計算
		Matrix4x4 rotateMatrix = MakeRotateMatrix(viewProjection_.rotation_);

		// オフセットをカメラの回転に合わせて回転
		offset = TransformNormal(offset, rotateMatrix);

		// 座標をコピーしてオフセットずらす
		viewProjection_.translation_ = Add(target_->translation_, offset);
	}

	XINPUT_STATE joyState;
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		const float kRadian = 0.02f;
		viewProjection_.rotation_.y += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * kRadian;
		//viewProjection_.rotation_.x += (float)joyState.Gamepad.sThumbRY / SHRT_MAX * kRadian;
	}

	// ビュー行列の更新
	viewProjection_.UpdateMatrix();

}
