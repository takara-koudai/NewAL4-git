#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "ViewProjection.h"
#include "Input.h"
#include "BaseCharacter.h"
#include <optional>
#include "PlayerBullet.h"

class Player : public BaseCharacter
{
public:

	Player();
	~Player();

	// ワールド座標を取得
	// Vector3 GetWorldPosition() { return worldTransform_.translation_; }

	Vector3 GetWorldPosition();

	const WorldTransform& GetWorldTransform() { return worldTransformBase_; }

	//Vector3 GetWorldPosition_() { return worldTransform_.translation_; }

	void SetViewProjection(const ViewProjection* viewprojection) 
	{
		viewProjection_ = viewprojection;
	}

	//void Initialize(Model* model,uint32_t textureHandle);

	//void Initialize(Model* modelBody, Model* modelHead, Model* modelL_arm, Model* modelR_arm);

	void Initialize(const std::vector<Model*>& models) override;

	void Update() override ; 

	void Draw(const ViewProjection& viewProjection) override;
	
	// 攻撃
	void Attack(Vector3& position);

	// 弾リスト
	const std::list<PlayerBullet*>& GetBullet() const { return bullets_; }

	// 浮遊ギミック初期化
	void InitializeFloatingGimmick();
	// 浮遊ギミック更新
	void UpdateFloatingGimmick();
	
	// 浮遊ギミックの媒介変数
	float floatingParameter_ = 0.0f;


#pragma region 攻撃（ハンマー）


	//通常行動初期化
	void BehaviorRootInitialize();
	//通常行動更新
	void BehaviorRootUpdate();


	//攻撃行動初期化
	void BehaviorAttackInitialize();
	//攻撃行動更新
	void BehaviorAttackUpdate();

	//振る舞い
	enum class Behavior {
		kRoot, //通常攻撃
		kAttack //攻撃中
	};


	//振る舞い
	Behavior behavior_ = Behavior::kRoot;

	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	//当たり判定
	void OnCollision();

	//リセット
	bool Reset() { return isDead_; }

#pragma endregion

private:

	//ワールド変換データ
	//WorldTransform worldTransform_;

	WorldTransform worldTransformBase_;
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformHead_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;

	// 武器モデル
	WorldTransform worldTransformWeapon_;

	//3Dモデル
	Model* modelFighterBody_ = nullptr;
	Model* modelFighterHead_ = nullptr;
	Model* modelFighterL_arm_ = nullptr;
	Model* modelFighterR_arm_ = nullptr;

	//モデル
	Model* playerModel_ = nullptr;

	//テクスチャ
	uint32_t playerTextureHandle_ = 0u;

	//キーボード入力
	Input* input_ = nullptr;

	//カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;

	// 攻撃フレーム
	int attackFrame;

	bool isDead_ = false;

	// 弾
	PlayerBullet* bullet_ = nullptr;

	std::list<PlayerBullet*> bullets_;

	bool armFlag_ = false;
	
};
