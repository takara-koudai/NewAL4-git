#include "Player.h"
#include <cassert>
#include "MatrixTrans.h"
#include "ImGuiManager.h"
#define _USE_MATH_DEFINES
#include "math.h"

Player::Player() {}

Player::~Player() 
{
	for (PlayerBullet* bullet : bullets_) 
	{
		delete bullet_;
	}
}

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

	//
	//プレイヤーの座標を変えたい

	// 各パーツ位置調整
	worldTransform_.translation_.x = 1.0f;
	worldTransform_.translation_.y = 1.0f;
	worldTransform_.translation_.z = 1.0f;

	worldTransformBody_.translation_ = {0.0f, 1.0f, 0.0f};

	// 腕の座標指定
	worldTransformHead_.translation_.y = 1.5f;
	worldTransformL_arm_.translation_.x = -0.5f;
	worldTransformL_arm_.translation_.y = 1.3f;
	worldTransformR_arm_.translation_.x = 0.5f;
	worldTransformR_arm_.translation_.y = 1.3f;

}

void Player::Update()
{

	// デスフラグが立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}

		return false;
	});

	// ゲームパッドの状態を得る変数
	XINPUT_STATE joyState;
	
	//ゲームパッド状態取得
	if (Input::GetInstance()->GetJoystickState(0, joyState))
	{

	    // キャラクターの移動速さ
	    const float speed = 0.2f;
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

		
		//ダッシュ
		if (joyState.Gamepad.wButtons == XINPUT_GAMEPAD_B)
		{
			// キャラクターの移動速さ
			const float speedB = 0.8f;
			// 移動量
			Vector3 moveB = {
			    (float)joyState.Gamepad.sThumbLX / SHRT_MAX * speedB, 0.0f,
			    (float)joyState.Gamepad.sThumbLY / SHRT_MAX * speedB};
			// 移動量に速さを反映
			moveB = Normalize(moveB);
			moveB = Multiply3(speedB, moveB);

			// 回転行列
			Matrix4x4 rotateMatrixB = MakeRotateMatrix(viewProjection_->rotation_);
			// 移動ベクトルをカメラの角度だけ回転
			moveB = TransformNormal(moveB, rotateMatrixB);

			// 移動
			worldTransformBase_.translation_ = Add(worldTransformBase_.translation_, moveB);

			// playerのY軸周り角度(θy)
			worldTransformBase_.rotation_.y = std::atan2(moveB.x, moveB.z);

			//worldTransformL_arm_.rotation_.x = std::sin(floatingParameter_) * 0.75f;
		}

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

	//天球とグラウンドの外に出ないように移動制限
	const float kMoveLimitX = (70.0f);
	const float kMoveLimitY = (65.0f);
	const float kMoveLimitZ = (80.0f);

	worldTransformBase_.translation_.x = max(worldTransformBase_.translation_.x, -kMoveLimitX);
	worldTransformBase_.translation_.x = min(worldTransformBase_.translation_.x, +kMoveLimitX);
	worldTransformBase_.translation_.y = max(worldTransformBase_.translation_.y, -kMoveLimitY);
	worldTransformBase_.translation_.y = min(worldTransformBase_.translation_.y, +kMoveLimitY);
	worldTransformBase_.translation_.z = max(worldTransformBase_.translation_.z, -kMoveLimitZ);
	worldTransformBase_.translation_.z = min(worldTransformBase_.translation_.z, +kMoveLimitZ);

	// 弾発射
	Attack(worldTransformBase_.translation_);

	for (PlayerBullet* bullet : bullets_) 
	{
		bullet->Update();
	}

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

	
	if (behavior_ == Behavior::kAttack) 
	{
		models_[4]->Draw(worldTransformWeapon_, viewProjection);
	}

	// 弾描画
	for (PlayerBullet* bullet : bullets_) 
	{
		bullet->Draw(viewProjection);
	}
}

void Player::Attack(Vector3& position) 
{
	// ゲームパッドの状態を得る変数
	XINPUT_STATE joyStateBullet;
	armFlag_ = false;

	if (Input::GetInstance()->GetJoystickState(0, joyStateBullet)) {

		if (joyStateBullet.Gamepad.bRightTrigger) 
		{
			// 弾の速度
			const float kBulletSpeed = 1.0f;
			Vector3 velocity(0, 0, kBulletSpeed);

			// 速度ベクトルを自機の向きに合わせて回転させる
			velocity = TransformNormal(velocity, worldTransformBase_.matWorld_);

			// 弾を生成し、初期化
			PlayerBullet* newBullet = new PlayerBullet();
			newBullet->Initialize(models_[0], position, velocity);

			// 弾を登録する
			bullets_.push_back(newBullet);

			armFlag_ = true;
			
		}
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
	worldTransformL_arm_.rotation_.x = std::sin(floatingParameter_) * -0.75f;
	worldTransformR_arm_.rotation_.x = std::sin(floatingParameter_) * 0.75f;


	if (armFlag_ == true)
	{
		worldTransformL_arm_.rotation_.x = std::sin(floatingParameter_) - 0.85f;
		worldTransformR_arm_.rotation_.x = std::sin(floatingParameter_) - 0.85f;
	}

	// ゲームパッドの状態を得る変数
	//XINPUT_STATE joyStateArm;
	//if (Input::GetInstance()->GetJoystickState(0, joyStateArm)) {
	//	if (joyStateArm.Gamepad.wButtons == XINPUT_GAMEPAD_B) {
	//		// 腕の動き
	//		worldTransformL_arm_.rotation_.x = std::sin(floatingParameter_) + 1.25f;
	//		worldTransformR_arm_.rotation_.x = std::sin(floatingParameter_) + 1.25f;
	//	}
	//}
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
		float speed = 0.3f;

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
		
		/*
		if (joyState.Gamepad.wButtons == XINPUT_GAMEPAD_A) 
		{
			// 振る舞いリセット
			behaviorRequest_ = Behavior::kAttack;
			worldTransformWeapon_.rotation_.x = 1.0f;
			worldTransformWeapon_.translation_.y = 2.0f;
			worldTransformL_arm_.rotation_.x = 1.0f;
			worldTransformR_arm_.rotation_.x = 1.0f;
		}
		*/
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

void Player::OnCollision() 
{ 
	isDead_ = true;
}
