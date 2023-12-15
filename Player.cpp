#include "Player.h"
#include <cassert>
#include "MatrixTrans.h"
#include "ImGuiManager.h"
#define _USE_MATH_DEFINES
#include "math.h"
//#include <Xinput.h>

Player::Player() {}

Player::~Player() {}

void Player::Initialize(const std::vector<Model*>& models) { 
	input_ = Input::GetInstance();

	// 基底クラスの初期化
	BaseCharacter::Initialize(models);

	worldTransformBase_.Initialize();
	worldTransformBody_.Initialize();
	worldTransformHead_.Initialize();
	worldTransformL_arm_.Initialize();
	worldTransformR_arm_.Initialize();

	worldTransformWeapon_.Initialize();


	// 親子関係結ぶ
	worldTransformBody_.parent_ = &worldTransformBase_; // ボディの親をベースにする
	worldTransformHead_.parent_ = &worldTransformBody_;
	worldTransformL_arm_.parent_ = &worldTransformBody_;
	worldTransformR_arm_.parent_ = &worldTransformBody_;
	worldTransformWeapon_.parent_ = &worldTransformBody_;


	// 各パーツ位置調整
	worldTransform_.translation_.x = 1.0f;
	worldTransform_.translation_.y = 1.0f;
	worldTransform_.translation_.z = 1.0f;

	worldTransformBody_.translation_ = {0.0f, 1.0f, 0.0f};


	// 腕の座標指定
	worldTransformL_arm_.translation_.x = -0.5f;
	worldTransformHead_.translation_.y = 1.5f;
	worldTransformR_arm_.translation_.x = 0.5f;
	worldTransformL_arm_.translation_.y = 1.3f;
	worldTransformR_arm_.translation_.y = 1.3f;


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
	    worldTransformBase_.translation_ = Add(worldTransformBase_.translation_, move);
	    
			    // playerのY軸周り角度(θy)
	    worldTransformBase_.rotation_.y = std::atan2(move.x, move.z);
				
	}


	if (behaviorRequest_) {
		// 振る舞いを変更する
		behavior_ = behaviorRequest_.value();

		// 各振る舞いごとの初期化を実行

		// リクエストがあったら初期化と次の行動に以降
		switch (behavior_) {
		case Player::Behavior::kRoot:
			// 通常行動
			BehaviorRootInitialize();

			break;
		case Player::Behavior::kAttack:
			// 攻撃行動
			BehaviorAttackInitialize();

			break;
		}

		// 振る舞いをリセット
		behaviorRequest_ = std::nullopt;
	}

	// Behaviorの更新処理
	switch (behavior_) 
	{
	case Behavior::kRoot:

		BehaviorRootUpdate();

		break;

	case Behavior::kAttack:

		BehaviorAttackUpdate();

		break;
	}

	//worldTransform_.UpdateMatrix();
	
	//UpdateFloatingGimmick();

	BaseCharacter::Update();

	// 行列を定数バッファに転送
	worldTransformBody_.UpdateMatrix();
	worldTransformBase_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();
	worldTransformWeapon_.UpdateMatrix();

	
}

void Player::Draw(const ViewProjection& viewProjection) 
{
	models_[0]->Draw(worldTransformBody_, viewProjection);
	models_[1]->Draw(worldTransformHead_, viewProjection);
	models_[2]->Draw(worldTransformL_arm_, viewProjection);
	models_[3]->Draw(worldTransformR_arm_, viewProjection);

	//models_[4]->Draw(worldTransformWeapon_, viewProjection);

	if (behavior_ == Behavior::kAttack) 
	{
		models_[4]->Draw(worldTransformWeapon_, viewProjection);
	}
}


void Player::InitializeFloatingGimmick() 
{
	floatingParameter_ = 0.0f;
}

void Player::UpdateFloatingGimmick() 
{
	// 浮遊移動のサイクル<frame>
	const uint16_t period = 120;

	// 1フレームでのパラメーター加算値
	const float step = 2.0f * (float)M_PI / period;

	// パラメーターを1ステップ分加算
	floatingParameter_ += step;

	// 2πを超えたら0に戻す
	floatingParameter_ = (float)std::fmod(floatingParameter_, 2.0f * M_PI);

	// 浮遊の振幅<m>
	const float floatingAmplitude = 0.5f;

	// 浮遊を座標に反映
	worldTransformBody_.translation_.y = std::sin(floatingParameter_) * floatingAmplitude;

	// 腕の動き
	worldTransformL_arm_.rotation_.x = std::sin(floatingParameter_) * 0.75f;
	worldTransformR_arm_.rotation_.x = std::sin(floatingParameter_) * 0.75f;
}

#pragma region 行動

void Player::BehaviorRootInitialize() 
{
	worldTransformL_arm_.rotation_.x = 0.0f;
	worldTransformR_arm_.rotation_.x = 0.0f;
	worldTransformWeapon_.rotation_.x = 0.0f;

	// 浮遊初期化
	InitializeFloatingGimmick();

	worldTransformBody_.Initialize();
	worldTransformHead_.Initialize();
	worldTransformL_arm_.Initialize();
	worldTransformR_arm_.Initialize();
	worldTransformWeapon_.Initialize();
}

void Player::BehaviorRootUpdate() 
{
	XINPUT_STATE joyState;
	// ゲームパッド状態取得
	if (Input::GetInstance()->GetJoystickState(0, joyState)) 
	{
		// 速さ
		const float speed = 0.3f;

		// 移動量
		Vector3 move
		{
		    (float)joyState.Gamepad.sThumbLX / SHRT_MAX, 0.0f,
		    (float)joyState.Gamepad.sThumbLY / SHRT_MAX
		};

		// 移動量の速さを反映
		move = Normalize(move);
		move = Multiply3(speed, move);

		// 回転行列
		Matrix4x4 rotateMatrix = MakeRotateMatrix(viewProjection_->rotation_);

		if (joyState.Gamepad.wButtons == XINPUT_GAMEPAD_A) 
		{
			// 振る舞いリセット
			behaviorRequest_ = Behavior::kAttack;
			worldTransformWeapon_.rotation_.x = 1.0f;
			worldTransformWeapon_.translation_.y = 2.0f;
			worldTransformL_arm_.rotation_.x = 1.0f;
			worldTransformR_arm_.rotation_.x = 1.0f;
		}
	}

	// 浮遊ギミックの更新処理
	UpdateFloatingGimmick();
}

#pragma endregion

#pragma region 攻撃

void Player::BehaviorAttackInitialize() 
{
	worldTransformL_arm_.rotation_.x = (float)M_PI;
	worldTransformR_arm_.rotation_.x = (float)M_PI;
	worldTransformWeapon_.rotation_.x = 0.0f;
	attackFrame = 0;
}

void Player::BehaviorAttackUpdate() 
{
	if (attackFrame < 10) 
	{
		// 腕
		worldTransformL_arm_.rotation_.x -= 0.05f;
		worldTransformR_arm_.rotation_.x -= 0.05f;

		// 武器
		worldTransformWeapon_.rotation_.x -= 0.05f;

	} else if (worldTransformWeapon_.rotation_.x <= 2.0f * (float)M_PI / 4) 
	{
		// 腕
		worldTransformL_arm_.rotation_.x += 0.1f;
		worldTransformR_arm_.rotation_.x += 0.1f;
		// 武器
		worldTransformWeapon_.rotation_.x += 0.1f;
	}
	else 
	{
		// 振る舞いリセット
		behaviorRequest_ = Behavior::kRoot;
	}

	attackFrame++;
}
#pragma endregion


Vector3 Player::GetWorldPosition() 
{
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransformBase_.matWorld_.m[3][0];
	worldPos.y = worldTransformBase_.matWorld_.m[3][1];
	worldPos.z = worldTransformBase_.matWorld_.m[3][2];

	return worldPos;
}