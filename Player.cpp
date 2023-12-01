#include "Player.h"
#include <cassert>
#include "MatrixTrans.h"
#include "ImGuiManager.h"
//#include <Xinput.h>

Player::Player() {}

Player::~Player() {}

void Player::Initialize(Model* model, uint32_t textureHandle) 
{ 
	input_ = Input::GetInstance();

	assert(model);

	playerModel_ = model;
	playerTextureHandle_ = textureHandle;

	worldTransform_.Initialize();

	worldTransform_.translation_ = {0.0f, 2.0f, 0.0f};

}

void Player::Update()
{

	// ゲームパッドの状態を得る変数
	XINPUT_STATE joyState;

	//ゲームパッド状態取得
	if (Input::GetInstance()->GetJoystickState(0, joyState))
	{
	    // キャラクターの移動速さ
	    const float speed = 0.3f;
	    // 移動量
	    Vector3 move = {
	        (float)joyState.Gamepad.sThumbLX / SHRT_MAX * speed, 0.0f,
	        (float)joyState.Gamepad.sThumbLY / SHRT_MAX * speed
		};
	    // 移動量に速さを反映
	    move = Normalize(move);
	    move = Multiply3(speed, move);
	    
	    // 回転行列
	    Matrix4x4 rotateMatrix = MakeRotateMatrix(viewProjection_->rotation_);
	    // 移動ベクトルをカメラの角度だけ回転
	    move = TransformNormal(move, rotateMatrix);
	    
	    // 移動
	    worldTransform_.translation_ = Add(worldTransform_.translation_, move);
	    
	    // playerのY軸周り角度(θy)
	    worldTransform_.rotation_.y = std::atan2(move.x, move.z);

	}

	worldTransform_.UpdateMatrix();


	//ImGui
	/* ImGui::Begin("player");

	float playerPos[] = 
	{
	    worldTransform_.translation_.x,  
		worldTransform_.translation_.y,
	    worldTransform_.translation_.z
	};

	ImGui::SliderFloat3("playerPos", playerPos, -20.0f, 25.0f);

	ImGui::End();

	worldTransform_.translation_.x = playerPos[0];
	worldTransform_.translation_.y = playerPos[1];
	worldTransform_.translation_.z = playerPos[2];*/
}

void Player::Draw(const ViewProjection& viewProjection) 
{
	playerModel_->Draw(worldTransform_, viewProjection, playerTextureHandle_);
}

Vector3 Player::GetWorldPosition() 
{
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}