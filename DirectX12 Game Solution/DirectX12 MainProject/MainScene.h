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

    // �ϐ��̐錾

    //�w�i
    DX9::SPRITE bgTestSprite;
    float bgPositionX;
    
    //�X�N���[�����x
    const float bgMoveSpeed = 300.0f;

    //�w�i���[�v�ʒu
    const float bgResetPosition = 2560.0f;


    //�v���C���[
    //����
    DX9::SPRITE smallFishTestSprite;

    //�i�}�Y
    DX9::SPRITE mediumFishTestSprite;

    //��
    DX9::SPRITE largeFishTestSprite;

    //�v���C���[�T�C�Y
    enum playerScale {
        //����
        smallFishScaleX = 110,
        smallFishScaleY = 70,
        //�i�}�Y
        mediumFishScaleX = 170,
        mediumFishScaleY = 105,
        //��
        largeFishScaleX = 230,
        largeFishScaleY = 170
    };

    //�v���C���[���
    int playerStatus;
    enum playerState {
        //����
        smallFishState,
        //�i�}�Y
        mediumFishState,
        //��
        largeFishState
    };

    //�X�s�[�h���
    float speedUpTime;
    int playerSpeedStatus;
    enum playerSpeedState {
        smallFishSpeedState,
        mediumFishSpeedState,
        largeFishSpeedState,
        speedUpState
    };

    //�v���C���[���W
    float playerPositionX, playerPositionY;

    //�v���C���[�����ړ����x
    enum { playerAutoMoveSpeed = 300 };

    //�v���C���[�ړ����x
    //�L�[�{�[�h
    const float   keyboardPlayerUpMoveSpeed    = -30.0f;
    const float   keyboardPlayerDownMoveSpeed  = 30.0f;
    const float   keyboardPlayerRightMoveSpeed = 30.0f;
    const float   keyboardPlayerLeftMoveSpeed  = -30.0f;
    //�p�b�h
    const float gamepadButtonPlayerMoveSpeedUp    = -30.0f;
    const float gamepadButtonPlayerMoveSpeedDown  = 30.0f;
    const float gamepadButtonPlayerMoveSpeedRight = 30.0f;
    const float gamepadButtonPlayerMoveSpeedLeft  = -30.0f;

    const float gamepadPlayerMoveSpeedX = 500.0f;
    const float gamepadPlayerMoveSpeedY = 500.0f;

    //����
    float playerInertiaX, playerInertiaY;

    //�v���C���[�ړ��͈�
    enum playerMoveRange {
        playerMoveRangeTop    = 251,
        playerMoveRangeBottom = 720,
        playerMoveRangeRight  = 1280,
        playerMoveRangeLeft   = 0
    };


    //��(�A�C�e��)
    DX9::SPRITE wormTestSprite;
    float wormPositionX, wormPositionY;

    //���T�C�Y
    enum {
        wormScaleX = 80,
        wormScaleY = 70
    };

    //�������ʒu
    const float wormInitialPositionX = 1500.0f;

    //�����Z�b�g�ʒu
    const float wormResetPositionX = -100.0f;

    //�������_�������ʒu
    std::mt19937 randomEngine;
    std::uniform_int_distribution<int> randomWormPositionY;
    int wormInitialPositionY;


    //��Q��
    //��
    DX9::SPRITE birdTestSprite;
    float birdPositionX, birdPositionY;

    //��(��)
    DX9::SPRITE bigRockTestSprite;
    float bigRockPositionX,bigRockPositionY;
    
    //��(��)
    DX9::SPRITE smallRockTestSprite;
    float smallRockPositionX, smallRockPositionY;

    //��
    DX9::SPRITE woodTestSprite;
    float woodPositionX, woodPositionY;

    //��Q�������ʒu
    const float obstacleInitialPositionX = 1500.0f;

    //��Q�����Z�b�g�ʒu
    const float obstacleResetPositionX = -300.0f;

    //��Q�������_�������ʒu
    std::uniform_int_distribution<int> randomObstaclePositionY;
    int obstacleInitialPositionY;

    //��Q�������_���p�^�[��
    std::uniform_int_distribution<int> randomObstacle;
    int obstaclePattern;

    //��Q���T�C�Y
    enum obstacleScale {
        //��
        birdScaleX = 230,
        birdScaleY = 100,
        //��(��)
        bigRockScaleX = 230,
        bigRockScaleY = 210,
        //��(��)
        smallRockScaleX = 40,
        smallRockScaleY = 50,
        //��
        woodScaleX = 230,
        woodScaleY = 130
    };

    //��Q�����
    int obstacleStatus;
    enum obstacleState {
        birdState,
        bigRockState,
        smallRockState,
        woodState
    };


    //UI
    //�Q�[�W
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
    

    //�f�o�b�O�p
    DX9::SPRITEFONT playerStatusFont;
    DX9::SPRITEFONT gaugeStageFont;


private:
    //Update���֐��̒�`

    //�w�i
    void bgMoveUpdate(const float deltaTime);

    //�X�N���[�����x
    void shiftBgMoveSpeed(const float deltaTime);

    //�X�N���[�����x����
    void setBgScrollSpeed();


    //��ԑJ�ڊ���
    void gaugePlayerStateAssignUpdate();

    //��ԑJ��
    void gaugeStageUpdate(const float deltaTime);


    //�v���C���[
    //�ړ��\�͈�
    void playerMoveRangeUpdate();
    
    //�����ړ�
    void playerAutoMoveUpdate(const float deltaTime);
    
    //�L�[�{�[�h����
    void playerControlKeyboardUpdate(const float deltaTime);
   
    //�p�b�h����
    void playerControlGamepadUpdate(const float deltaTime);


    //��(�A�C�e��)
    //���ړ�
    void wormMoveUpdate(const float deltaTime);

    //���ďo��
    void wormReAppearanceUpdate(const float deltaTime);

    //���ʒu���Z�b�g
    void wormPositionResetUpdate();
    
    //�������蔻��
    bool wormCollisionDetectionUpdate();


    //��Q��
    //��Q���ړ�
    void obstacleMoveUpdate(const float deltaTime);

    /*
    //��
    void birdObstacleMoveUpdate(const float deltaTime);

    //��(��)
    void bigRockObstacleMoveUpdate(const float deltaTime);

    //��(��)
    void smallRockObstacleMoveUpdate(const float deltaTime);

    //��
    void woodObstacleMoveUpdate(const float deltaTime);
    */

    //��Q�������蔻�� 
    bool obstacleCollisionDetectionUpdate();

    //��Q���ďo��
    void obstacleReAppearanceUpdate(const float deltaTime);

    //��Q���Ē��I
    void obstacleReLotteryUpdate(const float deltaTime);

    //��Q���ʒu���Z�b�g
    void obstaclePositionResetUpdate();


    //UI
    //�Q�[�W
    void gaugeMoveUpdate();


    //�����蔻��֐�
    //�x�[�X�����蔻��
    bool collisionDetectionBase(Rect& rect1, Rect& rect2);

    //�v���C���[�͈͐ݒ�ςݓ����蔻��
    bool PlayerCollisionDetection(Rect& rect2);

};