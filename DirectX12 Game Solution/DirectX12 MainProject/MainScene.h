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
    const float bgMoveSpeed = 300.0f;

    //背景ループ位置
    const float bgResetPosition = 2560.0f;


    //プレイヤー
    //金魚
    DX9::SPRITE smallFishTestSprite;

    //ナマズ
    DX9::SPRITE mediumFishTestSprite;

    //鯉
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

    //プレイヤー座標
    float playerPositionX, playerPositionY;

    //プレイヤー自動移動速度
    enum { playerAutoMoveSpeed = 300 };

    //プレイヤー移動速度
    //キーボード
    const float   keyboardPlayerUpMoveSpeed    = -30.0f;
    const float   keyboardPlayerDownMoveSpeed  = 30.0f;
    const float   keyboardPlayerRightMoveSpeed = 30.0f;
    const float   keyboardPlayerLeftMoveSpeed  = -30.0f;
    //パッド
    const float gamepadButtonPlayerMoveSpeedUp    = -30.0f;
    const float gamepadButtonPlayerMoveSpeedDown  = 30.0f;
    const float gamepadButtonPlayerMoveSpeedRight = 30.0f;
    const float gamepadButtonPlayerMoveSpeedLeft  = -30.0f;

    const float gamepadPlayerMoveSpeedX = 500.0f;
    const float gamepadPlayerMoveSpeedY = 500.0f;

    //慣性
    float playerInertiaX, playerInertiaY;

    //プレイヤー移動範囲
    enum playerMoveRange {
        playerMoveRangeTop    = 251,
        playerMoveRangeBottom = 720,
        playerMoveRangeRight  = 1280,
        playerMoveRangeLeft   = 0
    };


    //虫(アイテム)
    DX9::SPRITE wormTestSprite;
    float wormPositionX, wormPositionY;

    //虫サイズ
    enum {
        wormScaleX = 80,
        wormScaleY = 70
    };

    //虫初期位置
    const float wormInitialPositionX = 1500.0f;

    //虫リセット位置
    const float wormResetPositionX = -100.0f;

    //虫ランダム初期位置
    std::mt19937 randomEngine;
    std::uniform_int_distribution<int> randomWormPositionY;
    int wormInitialPositionY;


    //障害物
    //鳥
    DX9::SPRITE birdTestSprite;
    float birdPositionX, birdPositionY;

    //岩(大)
    DX9::SPRITE bigRockTestSprite;
    float bigRockPositionX,bigRockPositionY;
    
    //岩(小)
    DX9::SPRITE smallRockTestSprite;
    float smallRockPositionX, smallRockPositionY;

    //木
    DX9::SPRITE woodTestSprite;
    float woodPositionX, woodPositionY;

    //障害物初期位置
    const float obstacleInitialPositionX = 1500.0f;

    //障害物リセット位置
    const float obstacleResetPositionX = -300.0f;

    //障害物ランダム初期位置
    std::uniform_int_distribution<int> randomObstaclePositionY;
    int obstacleInitialPositionY;

    //障害物ランダムパターン
    std::uniform_int_distribution<int> randomObstacle;
    int obstaclePattern;

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
        firstStage  = 140,
        secondStage = 280,
        thirdStage  = 420,
        forthStage  = 560,
        fifthStage  = 700
    };
    

    //デバッグ用
    DX9::SPRITEFONT playerStatusFont;
    DX9::SPRITEFONT gaugeStageFont;


private:
    //Update内関数の定義

    //背景
    void bgMoveUpdate(const float deltaTime);

    //スクロール速度
    void shiftBgMoveSpeed(const float deltaTime);

    //スクロール速度割当
    void setBgScrollSpeed();


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


    //虫(アイテム)
    //虫移動
    void wormMoveUpdate(const float deltaTime);

    //虫再出現
    void wormReAppearanceUpdate(const float deltaTime);

    //虫位置リセット
    void wormPositionResetUpdate();
    
    //虫当たり判定
    bool wormCollisionDetectionUpdate();


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
    bool obstacleCollisionDetectionUpdate();

    //障害物再出現
    void obstacleReAppearanceUpdate(const float deltaTime);

    //障害物再抽選
    void obstacleReLotteryUpdate(const float deltaTime);

    //障害物位置リセット
    void obstaclePositionResetUpdate();


    //UI
    //ゲージ
    void gaugeMoveUpdate();


    //当たり判定関数
    //ベース当たり判定
    bool collisionDetectionBase(Rect& rect1, Rect& rect2);

    //プレイヤー範囲設定済み当たり判定
    bool PlayerCollisionDetection(Rect& rect2);

};