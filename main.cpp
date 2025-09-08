#include <Novice.h>
#include<time.h>
const char kWindowTitle[] = "LE2D_07_オオツカ_マモル";

const int kWindowWidth = 1280;
const int kWindowHeight = 720;


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };
	int timer = 60 * 30;

	//================================================================
	// 変数と初期化
	//================================================================


	//プレイヤー
	int posX = 200;
	int posY = 600;
	int speed = 10;
	int radius = 40;
	int bulletX = 100;
	int bulletY = 100;
	int bulletflag = 0;
	//0は存在しない１は存在する

	//敵
	const int enemyMax = 5;
	int enemyX[enemyMax] = { 0 };
	int enemyY[enemyMax] = { 0 };
	int enemyFlag[enemyMax] = { 0 };
	int enemyColor[enemyMax] = { 0 }; // 0=黒, 1=赤


	// 色定義（ARGB形式）
 /* const unsigned int RED = 0xFFFF0000;*/
  /*const unsigned int YELLOW = 0xFFFFFF00;*/
 /* const unsigned int GREEN = 0xFF00FF00;*/
	const unsigned int GRAY = 0xFF808080;

	// HP関連の定数
	const int HP_MAX = 15;       // HPの合計（赤5 + 黄5 + 緑5）
	const int HP_SEGMENT = 5;    // 色ごとのHP数
	const int HP_X = 100;        // HPバーのX座標
	const int HP_Y = 50;         // HPバーのY座標
	const int HP_W = 15;         // HP1個の幅
	const int HP_H = 20;         // HP1個の高さ

	// HP（3色 × 5 = 15）
	int hp = 15;



	//スコア
	int  score = 0;

	// 画面の状態
	enum Scene {
		TITLE,
		GAME_PLAY,
		SCORE
	};

	int SceneNo = TITLE;

	// 現在時刻の情報で初期化
	srand((unsigned int)time(NULL));
	//enemyX = rand() % 1100;

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		//=========================================================-
		// ゲームの処理
		//=========================================================

		switch (SceneNo) {
		case TITLE:

			/// 更新処理
			// スペースを押したら背景が変わる
			if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
				SceneNo = GAME_PLAY;
			}


			break;

		case GAME_PLAY:

            if (timer >= 0 && hp > 0)
            {
                // 敵の発生
                if (rand() % 20 == 0)
                {
                    for (int i = 0; i < enemyMax; i++)
                    {
                        if (enemyFlag[i] == 0)
                        {
                            enemyFlag[i] = 1;
                            enemyY[i] = 0;
                            enemyX[i] = rand() % 1100;
                            enemyColor[i] = rand() % 2;
                            break;
                        }
                    }
                }

                // 弾と敵の当たり判定
                //for (int i = 0; i < enemyMax; i++)
                //{
                //    if (enemyFlag[i] == 1 && bulletflag == 1)
                //    {
                //        if (abs(bulletX - enemyX[i]) < 100 && abs(bulletY - enemyY[i]) < 100)
                //        {
                //            bulletflag = 0;
                //            enemyFlag[i] = 0;

                //            // HPの段階によってスコアが変わる
                //            if (enemyColor[i] == 0) { // 黒い敵
                //                if (hp > 10) {
                //                    score += 3; // 緑ゲージ
                //                } else if (hp > 5) {
                //                    score += 2; // 黄ゲージ
                //                } else {
                //                    score += 1; // 赤ゲージ
                //                }
                //            } else if (enemyColor[i] == 1) { // 赤い敵
                //                hp -= 1; // 赤い敵に当たったらHP減る
                //            }
                //        }
                //    }
                //}

                // 弾と敵の当たり判定
                for (int i = 0; i < enemyMax; i++)
                {
                    if (enemyFlag[i] == 1 && bulletflag == 1)
                    {
                        if (abs(bulletX - enemyX[i]) < 100 && abs(bulletY - enemyY[i]) < 100)
                        {
                            bulletflag = 0;
                            enemyFlag[i] = 0;

                            if (enemyColor[i] == 0) { // 黒い敵
                                // --- HPの残り色数を計算 ---
                                int colorsLeft = 0;
                                if (hp > 10) {
                                    colorsLeft = 3; // 緑・黄・赤
                                } else if (hp > 5) {
                                    colorsLeft = 2; // 黄・赤
                                } else if (hp > 0) {
                                    colorsLeft = 1; // 赤だけ
                                }

                                // --- 色数に応じてスコア加算 ---
                                if (colorsLeft == 3) {
                                    score += 1;
                                } else if (colorsLeft == 2) {
                                    score += 3;
                                } else if (colorsLeft == 1) {
                                    score += 5;
                                }

                            } else if (enemyColor[i] == 1) { // 赤い敵
                                hp -= 1; // 赤い敵に当たったらHP減る
                            }
                        }
                    }
                }


                // 敵とプレイヤーの当たり判定
                for (int i = 0; i < enemyMax; i++)
                {
                    if (enemyFlag[i] == 1)
                    {
                        if (abs(posX - enemyX[i]) < (radius + 50) &&
                            abs(posY - enemyY[i]) < (radius + 50))
                        {
                            enemyFlag[i] = 0;
                            hp -= 1;
                        }
                    }
                }

                // 敵の落下
                for (int i = 0; i < enemyMax; i++)
                {
                    if (enemyFlag[i] == 1)
                    {
                        enemyY[i] += 5;
                        if (enemyY[i] > 720)
                        {
                            enemyFlag[i] = 0;
                        }
                    }
                }

                // プレイヤーの移動
                if (keys[DIK_D]) { posX += speed; }
                if (keys[DIK_A]) { posX -= speed; }

                // タイマー
                timer -= 1;

                // 弾処理
                if (bulletflag == 1) { bulletY -= 15; }
                if (bulletflag == 0)
                {
                    if (preKeys[DIK_SPACE] == 0 && keys[DIK_SPACE] != 0)
                    {
                        bulletflag = 1;
                        bulletX = posX;
                        bulletY = posY;
                    }
                }
                if (bulletY < 0) { bulletflag = 0; }
            }

            // ゲームオーバー判定
            if (hp <= 0 || timer < 0)
            {
                Novice::ScreenPrintf(600, 300, "Game Over");
            }

            ///
            /// 描画処理
            ///
            Novice::DrawEllipse(posX, posY, radius, radius, 0.0f, WHITE, kFillModeSolid);



            //// HPバー描画（3色）
            //for (int i = 0; i < 15; i++) {
            //    unsigned int color;

            //    if (i < 5) color = RED;        // 赤ゲージ

            //    else if (i < 10) color = 0xFF00FF00; // 黄ゲージ
            //    else color = GREEN;            // 緑ゲージ

            //    if (i < hp) {
            //        Novice::DrawBox(100 + i * 20, 50, 15, 20, 0.0f, color, kFillModeSolid);
            //    } else {
            //        Novice::DrawBox(100 + i * 20, 50, 15, 20, 0.0f, 0xFF808080, kFillModeSolid);
            //    }
            //}

            // HPバー描画（3色）
            //for (int i = 0; i < 15; i++) {
            //    unsigned int color;

            //    if (i < 5) {
            //        color = RED;        // 0〜4 → 赤
            //    } else if (i < 10) {
            //        color = 0xFFFFFF00;
            //        // 5〜9 → 黄
            //    } else {
            //        color = GREEN;      // 10〜14 → 緑
            //    }

            //    //else {
            //    //    color = GREEN;      // 10〜14 → 緑
            //    //}

            //   
            //    if (i < hp) {
            //        Novice::DrawBox(100 + i * 15, 50, 15, 20, 0.0f, color, kFillModeSolid);
            //    } else {
            //        Novice::DrawBox(100 + i * 15, 50, 15, 20, 0.0f, 0xFF808080, kFillModeSolid);
            //    }
            //   
            //}

            // --- 描画部分の置き換え ---
            if (hp < 0) hp = 0;
            if (hp > HP_MAX) hp = HP_MAX;

            // 左から消えたブロック数
            int lost = HP_MAX - hp; // 例: hp=14 -> lost=1（元インデックスは1から始まる）

            // 残っているHPを左詰めして描画（色は "元インデックス" に基づく）
            for (int j = 0; j < hp; j++) {
                int originalIndex = j + lost; // 元の位置（0..14）
                unsigned int color;

                if (originalIndex < HP_SEGMENT) {
                    color = RED;        // 元位置 0..4
                } else if (originalIndex < HP_SEGMENT * 2) {
                    /*color = YELLOW;*/     // 元位置 5..9
                    color = BLUE;

                } else {
                    color = GREEN;      // 元位置 10..14
                }

                Novice::DrawBox(HP_X + j * HP_W, HP_Y, HP_W, HP_H, 0.0f, color, kFillModeSolid);
            }

            // 右側の空きマスをグレーで描画（見た目の隙間を作る）
            for (int j = hp; j < HP_MAX; j++) {
                Novice::DrawBox(HP_X + j * HP_W, HP_Y, HP_W, HP_H, 0.0f, GRAY, kFillModeSolid);
            }




            Novice::ScreenPrintf(100, 100, "score=%d", score);

            for (int i = 0; i < enemyMax; i++)
            {
                if (enemyFlag[i] == 1)
                {
                    unsigned int color = (enemyColor[i] == 1) ? RED : BLACK;
                    Novice::DrawBox(enemyX[i] - 50, enemyY[i] - 50, 100, 100, 0.0f, color, kFillModeSolid);
                }
            }

            if (bulletflag == 1)
            {
                Novice::DrawEllipse(bulletX, bulletY, 10, 10, 0.0f, RED, kFillModeSolid);
            }

            Novice::ScreenPrintf(0, 0, "x=%d", timer);
            Novice::DrawBox(10, 10, timer / 10, 10, 0.0f, BLACK, kFillModeSolid);







			break;


		case SCORE:

			// スコアの表示
			Novice::ScreenPrintf(600, 300, "score = %d", score);

			// シーンの切り替え
			if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
				SceneNo = TITLE;
			}

			break;


		}


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