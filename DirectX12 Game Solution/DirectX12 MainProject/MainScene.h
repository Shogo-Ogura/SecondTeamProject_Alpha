//
// MainScene.h
//

#pragma once

#include "Scene.h"
#include<random>

using Microsoft::WRL::ComPtr;
using std::unique_ptr;
using std::make_unique;
using namespace DirectX;

class MainScene final : public Scene {
public:
    MainScene();
    virtual ~MainScene() { Terminate(); }

    MainScene(MainScene&&) = default;
    MainScene& operator= (MainScene&&) = default;

    MainScene(MainScene const&) = delete;
    MainScene& operator= (MainScene const&) = delete;

    // These are the functions you will implement.
    void Initialize() override;
    void LoadAssets() override;

    void Terminate() override;

    void OnDeviceLost() override;
    void OnRestartSound() override;

    NextScene Update(const float deltaTime) override;
    void Render() override;

private:
    DX12::DESCRIPTORHEAP descriptorHeap;
    DX12::SPRITEBATCH    spriteBatch;
    DX12::HGPUDESCRIPTOR dx9GpuDescriptor;

    // 変数の宣言

    //背景
    DX9::SPRITE bgTestSprite;
    float bgPositionX;

    //スクロール速度
    const float smallFishSpeed = 300.0f;
    const float mediumFishSpeed = 500.0f;
    const float largeFishSpeed = 700.0f;
    const float topSpeed = 1300.0f;

    //背景ループ位置
    const float bgResetPosition = 2560.0f;

    //ループカウント
    int loopCount;

    //プレイ時間
    float playTime;


    //プレイヤー
    //小
    DX9::SPRITE smallFishTestSprite;

    //中
    DX9::SPRITE mediumFishTestSprite;

    //大
    DX9::SPRITE largeFishTestSprite;

    //プレイヤーサイズ
    enum playerScale {
        //金魚
        smallFishScaleX = 110,
        smallFishScaleY = 70,
        //ナマズ
        mediumFishScaleX = 170,
        mediumFishScaleY = 105,
        //鯉
        largeFishScaleX = 230,
        largeFishScaleY = 170
    };

    //プレイヤー状態
    int playerStatus;
    enum playerState {
        //金魚
        smallFishState,
        //ナマズ
        mediumFishState,
        //鯉
        largeFishState
    };

    //スピード状態
    float speedUpTime;
    int playerSpeedStatus;
    enum playerSpeedState {
        smallFishSpeedState,
        mediumFishSpeedState,
        largeFishSpeedState,
        speedUpState
    };
    bool speedUp;

    //プレイヤー座標
    float playerPositionX, playerPositionY;

    //プレイヤー自動移動速度
    enum { playerAutoMoveSpeed = 300 };

    //プレイヤー移動速度
    //キーボード
    const float   keyboardPlayerUpMoveSpeed = -30.0f;
    const float   keyboardPlayerDownMoveSpeed = 30.0f;
    const float   keyboardPlayerRightMoveSpeed = 30.0f;
    const float   keyboardPlayerLeftMoveSpeed = -30.0f;

    //パッド
    //十字キー
    const float gamePadButtonPlayerMoveSpeedUp = -30.0f;
    const float gamePadButtonPlayerMoveSpeedDown = 30.0f;
    const float gamePadButtonPlayerMoveSpeedRight = 30.0f;
    const float gamePadButtonPlayerMoveSpeedLeft = -30.0f;
    //スティック
    const float gamePadPlayerMoveSpeedX = 500.0f;
    const float gamePadPlayerMoveSpeedY = 500.0f;

    //慣性
    float playerInertiaX, playerInertiaY;

    //プレイヤー移動範囲
    enum playerMoveRange {
        playerMoveRangeTop = 251,
        playerMoveRangeBottom = 720,
        playerMoveRangeRight = 1280,
        playerMoveRangeLeft = 0
    };


    //餌(アイテム)
    DX9::SPRITE feedTestSprite;
    float feedPositionX, feedPositionY;

    //餌サイズ
    enum {
        feedScaleX = 80,
        feedScaleY = 70
    };

    //餌移動速度
    const float feedMoveSpeed = 350.0f;

    //餌初期位置
    const float feedInitialPositionX = 1500.0f;

    //餌リセット位置
    const float feedResetPositionX = -100.0f;

    //餌ランダム初期位置
    std::mt19937 randomEngine;
    std::uniform_real_distribution<float> randomFeedPositionY;
    float feedInitialPositionY;

    //餌出現範囲
    const float feedAppearanceTop = 251.0f;
    const float feedAppearanceBottom = 1000.0f;


    //障害物
    //鳥
    DX9::SPRITE birdTestSprite;
    float birdPositionX, birdPositionY;

    //岩(大)
    DX9::SPRITE bigRockTestSprite;
    float bigRockPositionX, bigRockPositionY;

    //岩(小)
    DX9::SPRITE smallRockTestSprite;
    float smallRockPositionX, smallRockPositionY;

    //木
    DX9::SPRITE woodTestSprite;
    float woodPositionX, woodPositionY;

    //障害物移動速度
    const float obstacleMoveSpeed = 350.0f;

    //障害物初期位置
    const float obstacleInitialPositionX = 1500.0f;

    //障害物リセット位置
    const float obstacleResetPositionX = -300.0f;

    //障害物ランダム初期位置
    std::uniform_real_distribution<float> randomObstaclePositionY;
    float obstacleInitialPositionY;

    //障害物ランダムパターン
    std::uniform_int_distribution<int> randomObstacle;
    int obstaclePattern;

    //障害物出現範囲
    const float obstacleAppearanceTop = 251.0f;
    const float obstacleAppearanceBottom = 510.0f;

    //障害物サイズ
    enum obstacleScale {
        //鳥
        birdScaleX = 230,
        birdScaleY = 100,
        //岩(大)
        bigRockScaleX = 230,
        bigRockScaleY = 210,
        //岩(小)
        smallRockScaleX = 40,
        smallRockScaleY = 50,
        //木
        woodScaleX = 230,
        woodScaleY = 130
    };

    //障害物状態
    int obstacleStatus;
    enum obstacleState {
        birdState,
        bigRockState,
        smallRockState,
        woodState
    };


    //UI
    //ゲージ
    DX9::SPRITE gaugeTestSprite;
    DX9::SPRITE gaugeBgTestSprite;
    float gaugeWidth;
    int gaugeStage;
    enum gaugeState {
        firstStage = 140,
        secondStage = 280,
        thirdStage = 420,
        forthStage = 560,
        fifthStage = 700
    };
    const float gaugePositionX = 100.0f;
    const float gaugePositionY = 50.0f;

    //シーン遷移
    //ゴール
    enum { lengthToGoal = 1 };

    //ゲームオーバー
    bool gameOver;


    //デバッグ用
    DX9::SPRITEFONT playerStatusFont;
    DX9::SPRITEFONT gaugeStageFont;


private:
    //Update内関数の定義

    //背景
    //スクロール速度
    void bgMoveSpeedUpdate(const float deltaTime);

    //背景ループ
    void bgLoopUpdate(const float deltaTime);

    //スクロール速度変更
    void setBgScrollSpeed();


    //クリア時間計測
    void playTimeUpdate(const float deltaTime);


    //状態遷移割当
    void gaugePlayerStateAssignUpdate();

    //状態遷移
    void gaugeStageUpdate(const float deltaTime);


    //プレイヤー
    //移動可能範囲
    void playerMoveRangeUpdate();
    
    //自動移動
    void playerAutoMoveUpdate(const float deltaTime);
    
    //キーボード操作
    void playerControlKeyboardUpdate(const float deltaTime);
   
    //パッド操作
    void playerControlGamepadUpdate(const float deltaTime);


    //餌(アイテム)
    //餌移動
    void feedMoveUpdate(const float deltaTime);

    //餌ループ
    void feedLoopUpdate();

    //餌位置リセット
    void feedPositionResetUpdate();
    
    //餌当たり判定
    bool isFeedCollisionDetectionUpdate();


    //障害物
    //障害物移動
    void obstacleMoveUpdate(const float deltaTime);

    /*
    //鳥
    void birdObstacleMoveUpdate(const float deltaTime);

    //岩(大)
    void bigRockObstacleMoveUpdate(const float deltaTime);

    //岩(小)
    void smallRockObstacleMoveUpdate(const float deltaTime);

    //木
    void woodObstacleMoveUpdate(const float deltaTime);
    */

    //障害物当たり判定 
    bool isObstacleCollisionDetectionUpdate();

    //障害物ループ
    void obstacleLoopUpdate(const float deltaTime);

    //障害物再抽選
    void obstacleReLotteryUpdate(const float deltaTime);

    //障害物位置リセット
    void obstaclePositionResetUpdate(const float deltaTime);


    //UI
    //ゲージ
    void gaugeMoveUpdate();


    //シーン遷移
    NextScene changeClearSceneUpdate();


    //当たり判定関数
    //ベース当たり判定
    bool isCollisionDetectionBase(Rect& rect1, Rect& rect2);

    //プレイヤー範囲設定済み当たり判定
    bool isPlayerCollisionDetection(Rect& rect2);

};