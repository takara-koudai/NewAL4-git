#include "TitleScene.h"
#include "TextureManager.h"

TitleScene::TitleScene() 
{

}

TitleScene::~TitleScene() 
{
	delete titleSprite;
}

void TitleScene::Initialize() 
{
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// タイトル
	TitleTextureHandle = TextureManager::Load("opening.png");
	TitleTextureSin = TextureManager::Load("sin.png");
	TitleTextureGeki = TextureManager::Load("geki.png");
	TitleTextureHa = TextureManager::Load("ha.png");
	TitleTextureRT = TextureManager::Load("RT.png");
	TitleTextureSt = TextureManager::Load("St.png");

	// タイトル描画
	titleSprite = Sprite::Create(TitleTextureHandle, {0, 0});
	titleSpriteSin = Sprite::Create(TitleTextureSin, position_);
	titleSpriteGeki = Sprite::Create(TitleTextureGeki, position2_);
	titleSpriteHa = Sprite::Create(TitleTextureHa, position3_);
	titleSpriteRT = Sprite::Create(TitleTextureRT, position4_);
	titleSpriteSt = Sprite::Create(TitleTextureSt, position5_);

	titleSpriteSin->SetPosition(position_);
	titleSpriteGeki->SetPosition(position2_);
	titleSpriteHa->SetPosition(position3_);
	titleSpriteRT->SetPosition(position4_);
	titleSpriteSt->SetPosition(position5_);

	position_;
	position2_;
	position3_;
	position4_;
	position5_;
	
	// フェード用
	uint32_t fadeTextureHandle = TextureManager::Load("fade.png");
	fadeSprite_ = Sprite::Create(fadeTextureHandle, {0, 0});
	

	// デバックカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

}

void TitleScene::Update() 
{

	count2++;

	// フェード開始
	fadeColor_.w -= 0.01f;
	fadeSprite_->SetColor(fadeColor_);

	
#pragma region 文字移動
	
	if (count2 >= 100)
	{

		if (position_.y <= 20) {
			position_.y += speed_;
			titleSpriteSin->SetPosition(position_);
		}

		if (position_.y >= 20) {
			if (position2_.x <= 445) {
				position2_.x += speed_;
				titleSpriteGeki->SetPosition(position2_);
			}
		}

		if (position2_.x >= 445) {
			if (position3_.x >= 635) {
				position3_.x -= speed_;
				titleSpriteHa->SetPosition(position3_);
			}
		}

		if (position3_.x <= 635) {
			if (position4_.y >= 350) {
				position4_.y -= speed_;
				titleSpriteRT->SetPosition(position4_);
			}
		}

		if (position4_.y <= 350) {
			if (position5_.y >= 550) {
				position5_.y -= speed_;
				titleSpriteSt->SetPosition(position5_);
			}
		}
	}
	

#pragma endregion

	// 文字通り過ぎる
	if (back_ == true) {
		// 新
		position_.y += speed_;
		titleSpriteSin->SetPosition(position_);

		// 撃
		position2_.x += speed_;
		titleSpriteGeki->SetPosition(position2_);

		// 破
		position3_.x -= speed_;
		titleSpriteHa->SetPosition(position3_);

		// 操作
		position4_.y -= speed_;
		titleSpriteRT->SetPosition(position4_);

		// スタート
		position5_.y -= speed_;
		titleSpriteSt->SetPosition(position5_);
	}
	
	XINPUT_STATE joyState;
	// ゲームパッド状態取得
	if (Input::GetInstance()->GetJoystickState(0, joyState))
	{
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) 
		{
			if (position5_.y <= 550) 
			{
				// 文字移動
				back_ = true;
			}
		}
	}

	if (back_ == true)
	{
		count++;
	}
	
	if (count >= 50) 
	{
		// 次のシーンへ
		isSceneEnd = true;
	}

	//カウントリセット
	if (isSceneEnd == true)
	{
		count = 0;
		count2 - 0;
	}

}


void TitleScene::Draw() 
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	///< summary>
	/// ここに背景スプライトの描画処理を追加できる
	///</summary>

	titleSprite->Draw();

	titleSpriteSin->Draw();
	titleSpriteGeki->Draw();
	titleSpriteHa->Draw();
	titleSpriteRT->Draw();
	titleSpriteSt->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	///< summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	///</summary>

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	///< summary>
	/// ここに前景スプライトの描画処理を追加できる
	///</summary>

	// フェードの白い画面
	fadeSprite_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}


