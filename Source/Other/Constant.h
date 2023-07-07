#pragma once

//------------------共通で使用する定数を記述するヘッダーファイル--------------------//

//スクリーンサイズ
//※ここ触るとmain.cppにも影響アリ。画面サイズが変わる
#define SCREEN_WIDTH			(1280)					//スクリーン横幅
#define SCREEN_HEIGHT			(720)					//スクリーン縦幅			

//最大ステージ数
#define MAX_STAGE (8)
//ステージのレベル数
#define MAX_STAGE_LEVEL (2)

//ゲームをスタートする位置...Z軸
#define START_LINE				(-390)
//ゴールする位置...Z軸
#define GOAL_LINE				(1150)

//次のシーン
#define NEXT_SCENE_GAME			(1)						//ゲームシーン
#define NEXT_SCENE_OTHER		(0)						//それ以外のシーン

//障害物関連
#define ENEMY_LINE				(30)					//縦の障害物の数
#define ENEMY_COLUMN			(3)						//レーン数
//...配置関連
#define OBJ_START_LINE			(START_LINE + 55)		//スタート地点
#define STANDARD_POTITION_X		(-10)					//X軸オブジェクトの基準位置 -10, 0, 10
#define INTERVAL_X				(10)					//X軸オブジェクトの間隔
#define INTERVAL_Z				(50)					//Z軸オブジェクトの間隔

//背景オブジェクト配置関連
#define BACK_OBJ_MAX			(3)					//背景の岩最大生成数
#define BACK_OBJ_INTERVAL_Z		(100)				//Z軸オブジェクトの間隔
#define LEFT_FRONT_LINE			(0)					//左列の一番前
#define RIGHT_FRONT_LINE		(3)					//右列の一番前


//Sprite描画基準
#define STANDARD_POS			(500)					//基準
#define ABOUT_HALF				(0.48)					//約半分
#define PIXEL_32				(32)					//32ピクセル
#define PIXEL_64				(64)					//64ピクセル
#define PIXEL_128				(128)					//128ピクセル
#define PIXEL_256				(256)					//256ピクセル
#define PIXEL_512				(512)					//512ピクセル