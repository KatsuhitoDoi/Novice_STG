#include <Novice.h>

const char kWindowTitle[] = "Katsuhito_D_タイトル";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	//変数宣言・初期化

	//プレイヤー
	float playerPosX = 640.0f;
	float playerPosY = 560.0f;
	float playerRadius = 16.0f;
	float playerSpeed = 5.0f;

	//弾
	float bulletPosX[9] = {0.0f};
	float bulletPosY[9] = { 0.0f };
	float bulletRadius = { 8.0f };
	float bulletSpeed = { 10.0f };
	int isBulletShot = { false };

	//敵
	float enemyPosX = 640.0f;
	float enemyPosY = 100.0f;
	float enemyRadius = 20.0f;
	float enemySpeed = 2.0f;
	int enemyRespawnTimer = 100;
	int isEnemyAlive = true;

	//敵に向かう弾
	float bullet2enemyX = 0.0f;
	float bullet2enemyY = 0.0f;
	float bullet2enemy = 0.0f;

	//画像の読み込み
	
	//爆発
	int explosionGH =
		Novice::LoadTexture("./Resources/explode.png");
	
	//プレイヤー
	int playerGH =
		Novice::LoadTexture("./Resources/fighter.png");

	//エネミー
	int enemyGH =
		Novice::LoadTexture("./Resources/monster.png");

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		//プレイヤーの操作

		//Aキーまたは←キーを押したら、左に移動
		if (Novice::CheckHitKey(DIK_A) || Novice::CheckHitKey(DIK_LEFT)) {
			playerPosX -= playerSpeed;
		}

		//Dキーまたは→キーを押したら、右に移動
		if (Novice::CheckHitKey(DIK_D) || Novice::CheckHitKey(DIK_RIGHT)) {
			playerPosX += playerSpeed;
		}

		//Wキーまたは↑キーを押したら、上に移動
		if (Novice::CheckHitKey(DIK_W) || Novice::CheckHitKey(DIK_UP)) {
			playerPosY -= playerSpeed;
		}

		//Sキーまたは↓キーを押したら、下に移動
		if (Novice::CheckHitKey(DIK_S) || Novice::CheckHitKey(DIK_DOWN)) {
			playerPosY += playerSpeed;
		}

		//自キャラが画面外に出ないようにする処理

		//X軸
		if (playerPosX >= 1230) {
			playerPosX = 1230;
		}

		if (playerPosX <= 15) {
			playerPosX = 15;
		}

		//Y軸
		if (playerPosY >= 670) {
			playerPosY = 670;
		}

		if (playerPosY <= 15) {
			playerPosY = 15;
		}

		//弾の発射処理
		for (int i = 0; i < 9; i++) {
			if (isBulletShot == false) {
				if (keys[DIK_SPACE] && preKeys[DIK_SPACE] == 0) {
					isBulletShot = true;
					bulletPosX[i] = playerPosX;
					bulletPosY[i] = playerPosY;
				}
			}
		}

		

		//弾道計算
		for (int i = 0; i < 9; i++) {
			if (isBulletShot == 1) {
				bulletPosY[i] -= bulletSpeed;
				if (bulletPosY[i] <= 0) {
					bulletPosY[i] = -200;
					isBulletShot = false;
				}
			}
		}

		

		//敵の移動
		if (isEnemyAlive) {
			//敵を動かす（上から下に）
			enemyPosY += enemySpeed;

			//下で反射
			if (enemyPosY + enemyRadius >= 700) {
				enemySpeed *= -1;
			}

			//上で反射
			if (enemyPosY - enemyRadius <= 0) {
				enemySpeed *= -1;
			}
		}

		//敵と弾の衝突判定
		for (int i = 0; i < 9; i++) {
			if (isEnemyAlive && isBulletShot) {
				bullet2enemyX = enemyPosX - bulletPosX[i];
				bullet2enemyY = enemyPosY - bulletPosY[i];
				if ((enemyRadius + bulletRadius) * (enemyRadius + bulletRadius)
					>= (bullet2enemyX * bullet2enemyX) + (bullet2enemyY * bullet2enemyY)) {
					isEnemyAlive = false;
					isBulletShot = false;
				}
			}
		}

		

		//復活用タイマー（敵が死んでいたらタイマースタート）
		if(!isEnemyAlive){
			enemyRespawnTimer--;
			if (enemyRespawnTimer <= 0) {
				isEnemyAlive = true;
				enemyRespawnTimer = 100;
			}
		}


		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		//プレイヤーの描画（丸）
		/*Novice::DrawEllipse(
			playerPosX, playerPosY,
			playerRadius, playerRadius,
			0.0f,
			WHITE,
			kFillModeSolid);*/

		//プレイヤーの描画（画像）
		Novice::DrawSprite(
			static_cast<int>(playerPosX - playerRadius),
			static_cast<int>(playerPosY - playerRadius),
			playerGH,
			2.0f, 2.0f,
			0.0f,
			0xFFFFFFFF);


		//弾の描画（三角）
		for (int i = 0; i < 9; i++) {
			if (isBulletShot == 1) {
				Novice::DrawTriangle(
					(int)bulletPosX[i],
					(int)bulletPosY[i] - (int)bulletRadius,

					(int)bulletPosX[i] - (int)bulletRadius,
					(int)bulletPosY[i] + (int)bulletRadius,

					(int)bulletPosX[i] + (int)bulletRadius,
					(int)bulletPosY[i] + (int)bulletRadius,
					0xFFFF00FF, kFillModeSolid);
			}
		}

 		
		
		//敵の描画（四角形）
		/*if (isEnemyAlive) {
			Novice::DrawBox(
				static_cast<int>(enemyPosX - enemyRadius),
				static_cast<int>(enemyPosY - enemyRadius),
				static_cast<int>(enemyRadius * 2),
				static_cast<int>(enemyRadius * 2),
				0.0f,
				0xFF8888FF,
				kFillModeSolid);
		}*/
		
		//敵の描画（画像）
		if (isEnemyAlive) {
			Novice::DrawSprite(
				static_cast<int>(enemyPosX - enemyRadius),
				static_cast<int>(enemyPosY - enemyRadius),
				enemyGH,
				2.0f, 2.0f,
				0.0f,
				0xFFFFFFFF);
		}



		//爆発の描画
		if (!isEnemyAlive) {
			enemyRespawnTimer--;
			if (enemyRespawnTimer <= 100 || enemyRespawnTimer >= 75) {
				Novice::DrawSprite(
					static_cast<int>(enemyPosX - enemyRadius),
					static_cast<int>(enemyPosY - enemyRadius),
					explosionGH,
					2.0f, 2.0f,
					0.0f,
					0xFFFFFFFF
				);

			}
		}

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
