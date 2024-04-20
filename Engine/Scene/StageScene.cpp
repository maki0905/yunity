#include "StageScene.h"

#include "TextureManager.h"
#include "Input.h"
#include "ImGuiManager.h"
#include "SceneManager.h"

void StageScene::Initialize()
{
	world_ = std::make_unique<World>();
	world_->Initialize();

	camera_ = std::make_unique<Camera>();
	camera_->Update();

	debugCamera_ = std::make_unique<DebugCamera>();
	isDebug_ = false;

	// レールカメラの生成
	railCamera_ = std::make_unique<RailCamera>();
	// レールカメラの初期化
	Vector3 translation = { 0.0f, 0.0f, -200.0f };
	Vector3 rotation = { 0.0f, 0.0f, 0.0f };
	railCamera_->Initialize(camera_.get(), translation, rotation);

	player_ = std::make_unique<Player>();
	player_->Initialize(camera_.get());
	//player_->SetParent(&railCamera_->GetWorldMatrix());

	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(camera_.get());

	// 敵発生データの読み込み
	LoadEnemyPopData();
	

	world_->Add(player_.get());
}

void StageScene::Update()
{

	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}

	// 敵キャラの発生
	UpdateEnemyPopCommands();

	player_->Update();

	for (std::list<Enemy*>::iterator enemyIterator =enemys_.begin(); enemyIterator != enemys_.end();) {
		if ((*enemyIterator)->IsDead()) {
			enemyIterator = enemys_.erase(enemyIterator);
			continue;
		}
		(*enemyIterator)->Update();
		if ((*enemyIterator)->IsLockon()) {
			player_->SetEnemy(*enemyIterator);
		}
		// 次のイテレーターへ
		++enemyIterator;
	}

	for (std::list<EnemyBullet*>::iterator enemyBulletIterator = enemyBullets_.begin(); enemyBulletIterator != enemyBullets_.end();) {
		if ((*enemyBulletIterator)->IsDead()) {
			enemyBulletIterator = enemyBullets_.erase(enemyBulletIterator);
			continue;
		}
		(*enemyBulletIterator)->SetPlayer(player_.get());
		(*enemyBulletIterator)->Update();
		// 次のイテレーターへ
		++enemyBulletIterator;
	}

	// 敵キャラの更新
	/*for (Enemy* enemy : enemys_) {
		enemy->Update();
		if (enemy->IsLockon()) {
			player_->SetEnemy(enemy);
		}
	}*/

	// 敵弾更新
	/*for (EnemyBullet* bullet : enemyBullets_) {
		bullet->SetPlayer(player_.get());
		bullet->Update();
	}*/

	// デスフラグの立った敵弾を削除
	/*enemyBullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});*/
	// デスフラグの立った敵を削除
	/*enemys_.remove_if([](Enemy* enemy) {
		if (enemy->IsDead()) {
			delete enemy;
			return true;
		}
		return false;
	});*/


	world_->Add(player_.get());
	for (Enemy* enemy : enemys_) {
		world_->Add(enemy);
	}
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	for (PlayerBullet* bullet : playerBullets) {
		world_->Add(bullet);
	}
	for (EnemyBullet* bullet : enemyBullets_) {
		world_->Add(bullet);
	}

	world_->Solve();

	if (Input::GetInstance()->TriggerKey(DIK_LSHIFT)) {
		isDebug_ ^= true;
	}

	if (isDebug_) {
		debugCamera_->Update(camera_.get());
		camera_->Update();
	}
	else {
		//railCamera_->Update(/*&viewProjection_*/);
		camera_->Update();
	}
}

void StageScene::DrawBack()
{
}

void StageScene::Draw3D()
{
	skydome_->Draw();
	player_->Draw();
	// 敵キャラの描画
	for (Enemy* enemy : enemys_) {
		enemy->Draw();
	}
	// 敵弾描画
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Draw();
	}
	
}

void StageScene::DrawFront()
{
	for (Enemy* enemy : enemys_) {
		enemy->DrawLockon();
	}
	player_->DrawUI();

}

void StageScene::AddEnemyBullet(EnemyBullet* enemyBullet)
{
	// リストに登録する
	enemyBullets_.push_back(enemyBullet);
}

void StageScene::LoadEnemyPopData()
{
	// ファイルを開く
	std::ifstream file;
	file.open("Resources/enemyPop.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void StageScene::UpdateEnemyPopCommands()
{
	// 待機処理
	if (waitFlag_) {
		waitTimer_--;
		if (waitTimer_ <= 0) {
			// 待機完了
			waitFlag_ = false;
		}
		return;
	}
	// 1行分の文字列を入れる変数
	std::string line;

	// コマンド実行ループ
	while (getline(enemyPopCommands, line)) {
		// 1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		// ,区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');

		// "//"から始まる行はコメント
		if (word.find("//") == 0) {
			// コメント行を飛ばす
			continue;

		}

		// POPコマンド
		if (word.find("POP") == 0) {
			// x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			// y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			// z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			// 敵を発生させる
			EnemyPop(Vector3(x, y, z));


		}
		// WAITコマンド
		else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			// 待ち時間
			int32_t waitTime = atoi(word.c_str());

			// 待機開始
			waitFlag_ = true;
			waitTimer_ = waitTime;

			// コマンドループを抜ける
			break;

		}


	}
}

void StageScene::EnemyPop(Vector3 position)
{
	// 敵キャラの生成
	Enemy* newEnemy = new Enemy();
	// 敵キャラの初期化
	newEnemy->Initialize(camera_.get(), position);
	newEnemy->SetStageScene(this);
	// 敵キャラを登録する
	enemys_.push_back(newEnemy);

	// 敵キャラに自キャラのアドレスを渡す
	for (Enemy* enemy : enemys_) {
		enemy->SetPlayer(player_.get());
	}
}
