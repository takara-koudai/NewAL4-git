#include "Audio.h"
#include "AxisIndicator.h"
#include "DirectXCommon.h"
#include "GameScene.h"
#include "ImGuiManager.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include "WinApp.h"
#include "TitleScene.h"
#include "EndScene.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	WinApp* win = nullptr;
	DirectXCommon* dxCommon = nullptr;
	// 汎用機能
	Input* input = nullptr;
	Audio* audio = nullptr;
	AxisIndicator* axisIndicator = nullptr;
	PrimitiveDrawer* primitiveDrawer = nullptr;
	GameScene* gameScene = nullptr;

	// ゲームウィンドウの作成
	win = WinApp::GetInstance();
	win->CreateGameWindow(L"LE2C_タナカ_コウダイ_新撃破");

	// DirectX初期化処理
	dxCommon = DirectXCommon::GetInstance();
	dxCommon->Initialize(win);

#pragma region 汎用機能初期化
	// ImGuiの初期化
	ImGuiManager* imguiManager = ImGuiManager::GetInstance();
	imguiManager->Initialize(win, dxCommon);

	// 入力の初期化
	input = Input::GetInstance();
	input->Initialize();

	// オーディオの初期化
	audio = Audio::GetInstance();
	audio->Initialize();

	// テクスチャマネージャの初期化
	TextureManager::GetInstance()->Initialize(dxCommon->GetDevice());
	TextureManager::Load("white1x1.png");

	// スプライト静的初期化
	Sprite::StaticInitialize(dxCommon->GetDevice(), WinApp::kWindowWidth, WinApp::kWindowHeight);

	// 3Dモデル静的初期化
	Model::StaticInitialize();

	// 軸方向表示初期化
	axisIndicator = AxisIndicator::GetInstance();
	axisIndicator->Initialize();

	primitiveDrawer = PrimitiveDrawer::GetInstance();
	primitiveDrawer->Initialize();
#pragma endregion

	// ゲームシーンの初期化
	gameScene = new GameScene();
	gameScene->Initialize();

	SceneType sceneNo = SceneType::kTitle;

	//タイトルの初期化
	TitleScene* titleScene = new TitleScene();
	titleScene->Initialize();

	//終わりの初期化
	EndScene* endScene = new EndScene();
	endScene->Initialize();


	// メインループ
	while (true) {
		// メッセージ処理
		if (win->ProcessMessage()) {
			break;
		}

		// ImGui受付開始
		imguiManager->Begin();
		// 入力関連の毎フレーム処理
		input->Update();

		// ゲームシーンの毎フレーム処理
		//gameScene->Update();

		switch (sceneNo) 
		{
		case SceneType::kTitle:
			titleScene->Update();

			if (titleScene->IsSceneEnd() == true)
			{
				//タイトルから次のシーン
				sceneNo = titleScene->NextScene();
				
				//ゲームシーンのリセット
				endScene->IsSceneEnd();
				gameScene->Reset();

			}

			break;
		case SceneType::kGameScene:

			// ゲームシーンの毎フレーム処理
			gameScene->Update();

			//終わり画面に
			if (gameScene->IsSceneEnd() == true)
			{
				sceneNo = gameScene->NextScene();
			}
			
			break;
		case SceneType::kEndeScene:

			endScene->Update();

			//タイトル画面に
			if (endScene->IsSceneEnd() == true)
			{
				sceneNo = endScene->NextScene();

				gameScene->IsSceneEnd();
				gameScene->Reset();
				titleScene->Reset();
				endScene->Reset();

			}

			break;
		}

		// 軸表示の更新
		axisIndicator->Update();
		// ImGui受付終了
		imguiManager->End();

		// 描画開始
		dxCommon->PreDraw();

		switch (sceneNo) 
		{
		case SceneType::kTitle:

			titleScene->Draw();

			break;
		case SceneType::kGameScene:

			// ゲームシーンの描画
			gameScene->Draw();

			break;
		case SceneType::kEndeScene:

			endScene->Draw();

			break;
		
		}

		
		// 軸表示の描画
		axisIndicator->Draw();
		// プリミティブ描画のリセット
		primitiveDrawer->Reset();
		// ImGui描画
		imguiManager->Draw();
		// 描画終了
		dxCommon->PostDraw();
	}

	// 各種解放
	delete gameScene;
	// 3Dモデル解放
	Model::StaticFinalize();
	audio->Finalize();
	// ImGui解放
	imguiManager->Finalize();

	// ゲームウィンドウの破棄
	win->TerminateGameWindow();

	return 0;
}