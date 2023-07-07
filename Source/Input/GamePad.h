#pragma once

using GamePadButton = unsigned int;

#include <Windows.h>
#include <Xinput.h>

// ゲームパッド
class GamePad
{
public:
	static const GamePadButton BTN_UP				= (1 <<  0);
	static const GamePadButton BTN_RIGHT			= (1 <<  1);
	static const GamePadButton BTN_DOWN				= (1 <<  2);
	static const GamePadButton BTN_LEFT				= (1 <<  3);
	static const GamePadButton BTN_Z				= (1 <<  4);
	static const GamePadButton BTN_X				= (1 <<  5);
	static const GamePadButton BTN_C				= (1 <<  6);
	static const GamePadButton BTN_V				= (1 <<  7);
	static const GamePadButton BTN_START			= (1 <<  8);
	static const GamePadButton BTN_BACK				= (1 <<  9);
	static const GamePadButton BTN_LEFT_THUMB		= (1 << 10);
	static const GamePadButton BTN_RIGHT_THUMB		= (1 << 11);
	//static const GamePadButton BTN_LEFT_SHOULDER	= (1 << 12);
	//static const GamePadButton BTN_RIGHT_SHOULDER	= (1 << 13);
	//static const GamePadButton BTN_LEFT_TRIGGER		= (1 << 14);
	//static const GamePadButton BTN_RIGHT_TRIGGER	= (1 << 15);

	//決定等のキー
	static const GamePadButton BTN_SPACE = (1 << 12);
	static const GamePadButton BTN_ENTER = (1 << 13);
	static const GamePadButton BTN_BACKSPACE = (1 << 14);
	//アクション対応キー
	static const GamePadButton BTN_J = (1 << 15);
	static const GamePadButton BTN_K = (1 << 16);
	static const GamePadButton BTN_L = (1 << 17);
	//左右移動
	static const GamePadButton BTN_A = (1 << 18);
	static const GamePadButton BTN_D = (1 << 19);
	//上下移動
	static const GamePadButton BTN_W = (1 << 29);
	static const GamePadButton BTN_S = (1 << 30);

	//Xboxキー配置
	static const GamePadButton BTN_PAD_A	= (1 << 20);
	static const GamePadButton BTN_PAD_B	= (1 << 21);
	static const GamePadButton BTN_PAD_X	= (1 << 22);
	static const GamePadButton BTN_PAD_Y	= (1 << 23);
	static const GamePadButton BTN_PAD_LB	= (1 << 24);
	static const GamePadButton BTN_PAD_RB	= (1 << 25);
	static const GamePadButton BTN_PAD_LT	= (1 << 26);
	static const GamePadButton BTN_PAD_RT	= (1 << 27);
	static const GamePadButton BTN_PAD_MENU = (1 << 28);

public:
	GamePad() {}
	~GamePad() {}

	// 更新
	void Update();

	// スロット設定
	void SetSlot(int slot) { this->slot = slot; }

	// ボタン入力状態の取得
	GamePadButton GetButton() const { return buttonState[0]; }

	// ボタン押下状態の取得
	GamePadButton GetButtonDown() const { return buttonDown; }

	// ボタン押上状態の取得
	GamePadButton GetButtonUp() const { return buttonUp; }

	// 左スティックX軸入力状態の取得
	float GetAxisLX() const { return axisLx; }

	// 左スティックY軸入力状態の取得
	float GetAxisLY() const { return axisLy; }

	// 右スティックX軸入力状態の取得
	float GetAxisRX() const { return axisRx; }

	// 右スティックY軸入力状態の取得
	float GetAxisRY() const { return axisRy; }

	// 左トリガー入力状態の取得
	float GetTriggerL() const { return triggerL; }

	// 右トリガー入力状態の取得
	float GetTriggerR() const { return triggerR; }

	//コントローラー接続状態の取得
	bool GetControllerState();

private:
	GamePadButton		buttonState[2] = { 0 };
	GamePadButton		buttonDown = 0;
	GamePadButton		buttonUp = 0;
	float				axisLx = 0.0f;
	float				axisLy = 0.0f;
	float				axisRx = 0.0f;
	float				axisRy = 0.0f;
	float				triggerL = 0.0f;
	float				triggerR = 0.0f;
	int					slot = 0;

	//コントローラー接続状態
	DWORD dwResult;
	bool isController = false;

};