//
// MainScene.cpp
//

#include "Base/pch.h"
#include "Base/dxtk.h"
#include "SceneFactory.h"

// Initialize member variables.
MainScene::MainScene() : dx9GpuDescriptor{}
{
    //�R���X�g���N�^�[
    //�ϐ��̏������i0�ɂ���j

}

// Initialize a variable and audio resources.
void MainScene::Initialize()
{
    //�Q�[���V�[���̏�����
    //�ϐ����������i��̓I�Ȑ��l��ݒ肷��j
    //�T�E���h��ǂݍ��񂾂肷��
    
    //�w�i
    bgPositionX = 0.0f;


    //�Q�[�W�i�K
    gaugeStage = firstStage;


    //�v���C���[
    //���W
    playerPositionX = 100.0f;
    playerPositionY = 300.0f;

    //����
    playerInertiaX = 0.0f;
    playerInertiaY = 0.0f;

    //���G����
    speedUpTime = 0.0f;

    //���
    playerStatus = smallFishState;
    playerSpeedStatus = smallFishSpeedState;


    //��(�A�C�e��)
    wormPositionX = 1500.0f;
    wormPositionY = 300.0f;

    //���ʒu���Z�b�g
    std::random_device rnd_dev;
    randomEngine = std::mt19937(rnd_dev());
    randomWormPositionY = std::uniform_int_distribution<int>(playerMoveRangeTop, playerMoveRangeBottom);


    //��Q��
    //��Q�����W
    //��
    birdPositionX = 1500.0f;
    birdPositionY = 300.0f;

    //��(��)
    bigRockPositionX = 1500.0f;
    bigRockPositionY = 300.0f;

    //��(��)
    smallRockPositionX = 1500.0f;
    smallRockPositionY = 300.0f;

    //��
    woodPositionX = 1500.0f;
    woodPositionY = 300.0f;

    //��Q�����
    obstacleStatus = bigRockState;

    //�����_�����Z�b�g���W
    randomObstaclePositionY = std::uniform_int_distribution<int>(playerMoveRangeTop, playerMoveRangeBottom);
    obstacleInitialPositionY = randomObstaclePositionY(randomEngine);

    //�����_��
    randomObstacle = std::uniform_int_distribution<int>(1, 4);


    //UI
    //�Q�[�W
    gaugeWidth = 140;
    
}

// Allocate all memory the Direct3D and Direct2D resources.
void MainScene::LoadAssets()
{
    //�O���t�B�b�N�̐ݒ������
    //Windows10�̓h���C�o���A�b�v�f�[�g�����Ɠǂݍ��ݒ������K�v�ɂȂ�

    descriptorHeap = DX12::CreateDescriptorHeap(DXTK->Device, 1);

    ResourceUploadBatch resourceUploadBatch(DXTK->Device);
    resourceUploadBatch.Begin();

    RenderTargetState rtState(DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_FORMAT_D32_FLOAT);
    SpriteBatchPipelineStateDescription pd(rtState, &CommonStates::NonPremultiplied);
    D3D12_VIEWPORT viewport = {
        0.0f, 0.0f, 1280.0f, 720.0f,
        D3D12_MIN_DEPTH, D3D12_MAX_DEPTH
    };

    

    spriteBatch = DX12::CreateSpriteBatch(DXTK->Device, resourceUploadBatch, pd, &viewport);

    dx9GpuDescriptor = DXTK->Direct3D9->CreateShaderResourceView(descriptorHeap.get(), 0);

    auto uploadResourcesFinished = resourceUploadBatch.End(DXTK->CommandQueue);
    uploadResourcesFinished.wait();


    // �O���t�B�b�N���\�[�X�̏���������

    //�w�i
    //bgTestSprite = DX9::Sprite::CreateFromFile(DXTK->Device9, L"bgTestSprite.png");
    bgTestSprite = DX9::Sprite::CreateFromFile(DXTK->Device9, L"bgSprite02.png");
    
    
    //�v���C���[
    //����
    smallFishTestSprite = DX9::Sprite::CreateFromFile(DXTK->Device9, L"goldfishTestSprite.png");

    //�i�}�Y
    //mediumFishTestSprite = DX9::Sprite::CreateFromFile(DXTK->Device9, L"catfishTestSprite.png");
    mediumFishTestSprite = DX9::Sprite::CreateFromFile(DXTK->Device9, L"mediumFishSprite.png");

    //��
    largeFishTestSprite = DX9::Sprite::CreateFromFile(DXTK->Device9, L"carpTestSprite.png");


    //��(�A�C�e��)
    //wormTestSprite = DX9::Sprite::CreateFromFile(DXTK->Device9, L"wormTestSprite.png");
    wormTestSprite = DX9::Sprite::CreateFromFile(DXTK->Device9, L"wormSprite.png");


    //��Q��
    //��
    //birdTestSprite = DX9::Sprite::CreateFromFile(DXTK->Device9, L"kingfisherTestSprite.png");
    birdTestSprite = DX9::Sprite::CreateFromFile(DXTK->Device9, L"birdSprite.png");

    //��(��)
    bigRockTestSprite = DX9::Sprite::CreateFromFile(DXTK->Device9, L"bigRockTestSprite.png");

    //��(��)
    smallRockTestSprite = DX9::Sprite::CreateFromFile(DXTK->Device9, L"smallRockTestSprite.png");

    //��
    woodTestSprite = DX9::Sprite::CreateFromFile(DXTK->Device9, L"woodTestSprite.png");


    //UI
    //�Q�[�W
    gaugeTestSprite = DX9::Sprite::CreateFromFile(DXTK->Device9, L"gaugeTestSprite.png");
    gaugeBgTestSprite = DX9::Sprite::CreateFromFile(DXTK->Device9, L"gaugeBgTestSprite.png");


    //�f�o�b�O�p
    playerStatusFont = DX9::SpriteFont::CreateFromFontName(DXTK->Device9, L"�l�r �o����", 20);
    gaugeStageFont = DX9::SpriteFont::CreateFromFontName(DXTK->Device9, L"�l�r �o����", 20);

}

// Releasing resources required for termination.
void MainScene::Terminate()
{
    DXTK->ResetAudioEngine();
    DXTK->WaitForGpu();

    // TODO: Add your Termination logic here.
    //�Q�[��(�V�[��)�I�����ɉ����������ꍇ�Ɏg��
}

// Direct3D resource cleanup.
void MainScene::OnDeviceLost()
{
    //�h���C�o�[�A�b�v�f�[�g�����o�����ꍇ�Ɏg��
    //�ǂݍ��݂Ȃ����O�ɉ����������ꍇ�ɉ�������
}

// Restart any looped sounds here
void MainScene::OnRestartSound()
{
    //�T�E���h�f�o�C�X���؂�ւ��ƃT�E���h���S����~����
    //BGM�Ȃǂ��ăX�^�[�g������
}

// Updates the scene.
NextScene MainScene::Update(const float deltaTime)
{
    // If you use 'deltaTime', remove it.
    UNREFERENCED_PARAMETER(deltaTime);

    // TODO: Add your game logic here.
    //�Q�[���𓮂����v���O�������L�q����

    //�w�i
    bgMoveUpdate(deltaTime);

    //�X�N���[�����x
    shiftBgMoveSpeed(deltaTime);

    //�X�N���[�X���x����
    setBgScrollSpeed();


    //��ԑJ�ڊ���
    gaugePlayerStateAssignUpdate();
    
    //��ԑJ��
    gaugeStageUpdate(deltaTime);
    

    //�v���C���[
    //�ړ��͈�
    playerMoveRangeUpdate();

    //�����ړ�
    playerAutoMoveUpdate(deltaTime);

    //�L�[�{�[�h����
    playerControlKeyboardUpdate(deltaTime);

    //�p�b�h����
    playerControlGamepadUpdate(deltaTime);


    //��(�A�C�e��)
    //�ړ�
    wormMoveUpdate(deltaTime);

    //���ďo��
    //wormReAppearanceUpdate(deltaTime);

    //��Q��
    //�ړ�
    obstacleMoveUpdate(deltaTime);

    //��Q���ďo��
    if (birdPositionX < obstacleResetPositionX || bigRockPositionX < obstacleResetPositionX || smallRockPositionX < obstacleResetPositionX || woodPositionX < obstacleResetPositionX)
    {
        obstaclePositionResetUpdate();
        obstacleReLotteryUpdate(deltaTime);
    }


    //UI
    //�Q�[�W
    gaugeMoveUpdate();
   

    return NextScene::Continue;
}

// Draws the scene.
void MainScene::Render()
{
    // TODO: Add your rendering code here.
    //�`��̃v���O����������
    
    DXTK->Direct3D9->Clear(DX9::Colors::RGBA(0, 0, 0, 255));  // �菇1

    DXTK->Direct3D9->BeginScene();    // �菇2
    DX9::SpriteBatch->Begin();        // �菇4

    // (������2D�`��̏���������)     // �菇5

    //�w�i
    DX9::SpriteBatch->DrawSimple(bgTestSprite.Get(), SimpleMath::Vector3(bgPositionX, 0, 10));


    //�v���C���[
    //����
    if (gaugeStage == firstStage || gaugeStage == secondStage) {
        DX9::SpriteBatch->DrawSimple(smallFishTestSprite.Get(), SimpleMath::Vector3(playerPositionX, playerPositionY, -5));
    }
    //�i�}�Y
    if (gaugeStage == thirdStage || gaugeStage == forthStage) {
        DX9::SpriteBatch->DrawSimple(mediumFishTestSprite.Get(), SimpleMath::Vector3(playerPositionX, playerPositionY, -5));
    }
    //��
    if (gaugeStage == fifthStage) {
        DX9::SpriteBatch->DrawSimple(largeFishTestSprite.Get(), SimpleMath::Vector3(playerPositionX, playerPositionY, -5));
    }


    //��(�A�C�e��)
    DX9::SpriteBatch->DrawSimple(wormTestSprite.Get(), SimpleMath::Vector3(wormPositionX, wormPositionY, 0));


    //��Q��
    //�� 
    DX9::SpriteBatch->DrawSimple(birdTestSprite.Get(), SimpleMath::Vector3(birdPositionX, birdPositionY, -4));
   
    //��(��)
    DX9::SpriteBatch->DrawSimple(bigRockTestSprite.Get(), SimpleMath::Vector3(bigRockPositionX, bigRockPositionY, -4));
    
    //��(��)
    DX9::SpriteBatch->DrawSimple(smallRockTestSprite.Get(), SimpleMath::Vector3(smallRockPositionX, smallRockPositionY, -400));

    //��
    DX9::SpriteBatch->DrawSimple(woodTestSprite.Get(), SimpleMath::Vector3(woodPositionX, woodPositionY, -4));


    //UI
    //�Q�[�W
    DX9::SpriteBatch->DrawSimple(gaugeTestSprite.Get(), SimpleMath::Vector3(100, 50, 8),
        RectWH(0.0f, 0.0f, gaugeWidth, 100.0f));
    DX9::SpriteBatch->DrawSimple(gaugeBgTestSprite.Get(), SimpleMath::Vector3(100, 50, 9));


    //�f�o�b�O�p
    //DX9::SpriteBatch->DrawString(playerStatusFont.Get(), SimpleMath::Vector2(0, 670), DX9::Colors::RGBA(0, 0, 0, 255), L"playerStatus:%d", playerStatus+1);
    //DX9::SpriteBatch->DrawString(gaugeStageFont.Get(), SimpleMath::Vector2(230, 670), DX9::Colors::RGBA(0, 0, 0, 255), L"gaugeStage:%d", gaugeStage+1);


    DX9::SpriteBatch->End();          // �菇6
    DXTK->Direct3D9->EndScene();      // �菇7

    DXTK->Direct3D9->UpdateResource();

    DXTK->ResetCommand();
    DXTK->ClearRenderTarget(Colors::CornflowerBlue);

    const auto heapes = descriptorHeap->Heap();
    DXTK->CommandList->SetDescriptorHeaps(1, &heapes);

    spriteBatch->Begin(DXTK->CommandList);
    spriteBatch->Draw(
        dx9GpuDescriptor,
        XMUINT2(1280, 720),
        SimpleMath::Vector2(0.0f, 0.0f)
    );
    spriteBatch->End();

    DXTK->Direct3D9->WaitUpdate();
    DXTK->ExecuteCommandList();
}

//Update���֐��̒�`

//�w�i
void MainScene::bgMoveUpdate(const float deltaTime)
{
    //�ʒu���Z�b�g
    if (bgPositionX <= -bgResetPosition)
        bgPositionX = 0;
}

//�X�N���[�����x
void MainScene::shiftBgMoveSpeed(const float deltaTime)
{
    switch (playerSpeedStatus) {
    case smallFishSpeedState:
        bgPositionX -= bgMoveSpeed * deltaTime;
        break;
    case mediumFishSpeedState:
        bgPositionX -= (bgMoveSpeed + 200.0f) * deltaTime;
        break;
    case largeFishSpeedState:
        bgPositionX -= (bgMoveSpeed + 400.0f) * deltaTime;
        break;
    case speedUpState:
        //speedUpTime += deltaTime;
        bgPositionX -= (bgMoveSpeed + 600.0f) * deltaTime;
        /*if (speedUpTime >= 2.0f)
        {
            speedUpTime = 0.0f;
            playerSpeedStatus = largeFishSpeedState;
        }*/
        break;
    }
}

//�X�N���[�����x����
void MainScene::setBgScrollSpeed()
{
    switch (playerStatus){
    case smallFishState:
        playerSpeedStatus = smallFishSpeedState;
        break;
    case mediumFishState:
        playerSpeedStatus = mediumFishSpeedState;
        break;
    case largeFishState:
        playerSpeedStatus = largeFishSpeedState;
        break;
    }
    if (playerStatus == largeFishState && wormCollisionDetectionUpdate())
    {
        playerSpeedStatus = speedUpState;
    }
}


//��ԑJ�ڊ���
void MainScene::gaugePlayerStateAssignUpdate()
{
    if (gaugeStage == firstStage || gaugeStage == secondStage) {
        playerStatus = smallFishState;
    }
    else if (gaugeStage == thirdStage || gaugeStage == forthStage) {
        playerStatus = mediumFishState;
    }
    else if (gaugeStage == fifthStage) {
        playerStatus = largeFishState;
    }
}

//��ԑJ��
void MainScene::gaugeStageUpdate(const float deltaTime)
{
    switch (gaugeStage) {
    case firstStage:
        if (wormCollisionDetectionUpdate())
        {
            wormReAppearanceUpdate(deltaTime);
            gaugeStage = secondStage;
        }
        break;
    case secondStage:
        if (wormCollisionDetectionUpdate())
        {
            wormReAppearanceUpdate(deltaTime);
            gaugeStage = thirdStage;
        }
        else if (obstacleCollisionDetectionUpdate())
        {
            obstacleReAppearanceUpdate(deltaTime);
            gaugeStage = firstStage;
        }
        break;
    case thirdStage:
        if (wormCollisionDetectionUpdate())
        {
            wormReAppearanceUpdate(deltaTime);
            gaugeStage = forthStage;
        }
        else if (obstacleCollisionDetectionUpdate())
        {
            obstacleReAppearanceUpdate(deltaTime);
            gaugeStage = secondStage;
        }
        break;
    case forthStage:
        if (wormCollisionDetectionUpdate())
        {
            wormReAppearanceUpdate(deltaTime);
            gaugeStage = fifthStage;
        }
        else if (obstacleCollisionDetectionUpdate())
        {
            obstacleReAppearanceUpdate(deltaTime);
            gaugeStage = thirdStage;
        }
        break;
    case fifthStage:
        if (wormCollisionDetectionUpdate())
        {
            wormReAppearanceUpdate(deltaTime);
        }
        else if (obstacleCollisionDetectionUpdate())
        {
            obstacleReAppearanceUpdate(deltaTime);
            gaugeStage = forthStage;
        }
        break;
    }
}


//�v���C���[
//�ړ��\�͈�
void MainScene::playerMoveRangeUpdate()
{
    //��E��
    if (playerPositionY <= playerMoveRangeTop)
    {
        playerPositionY = playerMoveRangeTop;
    }
    if (playerPositionX <= playerMoveRangeLeft)
    {
        playerPositionX = playerMoveRangeLeft;
    }
    //�E�E��
    //����
    if (playerStatus == smallFishState) {
        if (playerPositionX >= playerMoveRangeRight - smallFishScaleX)
        {
            playerPositionX = playerMoveRangeRight - smallFishScaleX;
        }
        if (playerPositionY >= playerMoveRangeBottom - smallFishScaleY)
        {
            playerPositionY = playerMoveRangeBottom - smallFishScaleY;
        }
    }
    //�i�}�Y
    if (playerStatus == mediumFishState) {
        if (playerPositionX >= playerMoveRangeRight - mediumFishScaleX)
        {
            playerPositionX = playerMoveRangeRight - mediumFishScaleX;
        }
        if (playerPositionY >= playerMoveRangeBottom - mediumFishScaleY)
        {
            playerPositionY = playerMoveRangeBottom - mediumFishScaleY;
        }
    }
    //��
    if (playerStatus == largeFishState) {
        if (playerPositionX >= playerMoveRangeRight - largeFishScaleX)
        {
            playerPositionX = playerMoveRangeRight - largeFishScaleX;
        }
        if (playerPositionY >= playerMoveRangeBottom - largeFishScaleY)
        {
            playerPositionY = playerMoveRangeBottom - largeFishScaleY;
        }
    }
}

//�����ړ�
void MainScene::playerAutoMoveUpdate(const float deltaTime)
{
    playerPositionX -= playerAutoMoveSpeed * deltaTime;
}

//�L�[�{�[�h����
void MainScene::playerControlKeyboardUpdate(const float deltaTime)
{
    //���E
    if (DXTK->KeyState->D)
    {
        playerInertiaX = playerInertiaX + deltaTime * (keyboardPlayerRightMoveSpeed - playerInertiaX);
    }
    if (DXTK->KeyState->A)
    {
        playerInertiaX = playerInertiaX + deltaTime * (keyboardPlayerLeftMoveSpeed - playerInertiaX);
    }
    if (!DXTK->KeyState->D && !DXTK->KeyState->A)
    {
        playerInertiaX = playerInertiaX + 2 * deltaTime * (0.0f - playerInertiaX);
    }

    playerPositionX += playerInertiaX;

    //�㉺
    if (DXTK->KeyState->S)
    {
        playerInertiaY = playerInertiaY + deltaTime * (keyboardPlayerDownMoveSpeed - playerInertiaY);
    }
    if (DXTK->KeyState->W)
    {
        playerInertiaY = playerInertiaY + deltaTime * (keyboardPlayerUpMoveSpeed - playerInertiaY);
    }
    if (!DXTK->KeyState->D && !DXTK->KeyState->A)
    {
        playerInertiaY = playerInertiaY + 2 * deltaTime * (0.0f - playerInertiaY);
    }

    playerPositionY += playerInertiaY;

}

//�p�b�h����
void MainScene::playerControlGamepadUpdate(const float deltaTime)
{
    //�\���L�[
    if (DXTK->GamePadState[0].IsDPadRightPressed())
    {
        playerInertiaX = playerInertiaX + deltaTime * (gamepadButtonPlayerMoveSpeedRight - playerInertiaX);
    }
    if (DXTK->GamePadState[0].IsDPadLeftPressed())
    {
        playerInertiaX = playerInertiaX + deltaTime * (gamepadButtonPlayerMoveSpeedLeft - playerInertiaX);
    }
    if (!DXTK->GamePadState[0].IsDPadRightPressed() && !DXTK->GamePadState[0].IsDPadLeftPressed())
    {
        playerInertiaX = playerInertiaX + 2 * deltaTime * (0.0f - playerInertiaX);
    }

    playerPositionX += playerInertiaX;


    if (DXTK->GamePadState[0].IsDPadDownPressed())
    {
        playerInertiaY = playerInertiaY + deltaTime * (gamepadButtonPlayerMoveSpeedDown - playerInertiaY);
    }
    if (DXTK->GamePadState[0].IsDPadUpPressed())
    {
        playerInertiaY = playerInertiaY + deltaTime * (gamepadButtonPlayerMoveSpeedUp - playerInertiaY);
    }
    if (!DXTK->GamePadState[0].IsDPadDownPressed() && !DXTK->GamePadState[0].IsDPadUpPressed())
    {
        playerInertiaY = playerInertiaY + 2 * deltaTime * (0.0f - playerInertiaY);
    }

    playerPositionY += playerInertiaY;

    //�X�e�B�b�N
    playerPositionX += gamepadPlayerMoveSpeedX * DXTK->GamePadState[0].thumbSticks.leftX * deltaTime;
    playerPositionY -= gamepadPlayerMoveSpeedY * DXTK->GamePadState[0].thumbSticks.leftY * deltaTime;
}


//��(�A�C�e��)
//�ړ�
void MainScene::wormMoveUpdate(const float deltaTime)
{
    wormPositionX -= 350 * deltaTime;

    //���[�v
    if (wormPositionX <= -200)
    {
        wormPositionX = 1500;
    }
}

//���ďo��
void MainScene::wormReAppearanceUpdate(const float deltaTime)
{
    if (wormPositionX <= wormResetPositionX || wormCollisionDetectionUpdate()) 
    {
        wormPositionResetUpdate();
    }
}

//�ʒu���Z�b�g
void MainScene::wormPositionResetUpdate()
{
    wormInitialPositionY = randomWormPositionY(randomEngine);

    wormPositionX = wormInitialPositionX;
    wormPositionY = wormInitialPositionY;
}

//�������蔻��
bool MainScene::wormCollisionDetectionUpdate()
{
    if (PlayerCollisionDetection(RectWH(wormPositionX, wormPositionY, wormScaleX, wormScaleY)))
        return true;
    else
        return false;
}


//��Q��
//��Q���ړ� 
void MainScene::obstacleMoveUpdate(const float deltaTime)
{
    switch (obstacleStatus) {
    case birdState:
        birdPositionX -= 350 * deltaTime;
        break;
    case bigRockState:
        bigRockPositionX -= 350 * deltaTime;
        break;
    case smallRockState:
        smallRockPositionX -= 350 * deltaTime;
        break;
    case woodState:
        woodPositionX -= 350 * deltaTime;
        break;
    }
}

/*
//��
void MainScene::birdObstacleMoveUpdate(const float deltaTime)
{
    birdPositionX -= 300.0f * deltaTime;
}

//��(��)
void MainScene::bigRockObstacleMoveUpdate(const float deltaTime)
{
    bigRockPositionX -= 300.0f * deltaTime;
}

//��(��)
void MainScene::smallRockObstacleMoveUpdate(const float deltaTime)
{
    smallRockPositionX -= 300.0f * deltaTime;
}

//��
void MainScene::woodObstacleMoveUpdate(const float deltaTime)
{
    woodPositionX -= 300.0f * deltaTime;
}
*/

//��Q�������蔻��
bool MainScene::obstacleCollisionDetectionUpdate()
{
    switch (obstacleStatus) {
    case birdState:
        if (PlayerCollisionDetection(RectWH(birdPositionX, birdPositionY, birdScaleX, birdScaleY)))
            return true;
        break;
    case bigRockState:
        if (PlayerCollisionDetection(RectWH(bigRockPositionX, bigRockPositionY, bigRockScaleX, bigRockScaleY)))
            return true;
        break;
    case smallRockState:
        if (PlayerCollisionDetection(RectWH(smallRockPositionX, smallRockPositionY, smallRockScaleX, smallRockScaleY)))
            return true;
        break;
    case woodState:
        if (PlayerCollisionDetection(RectWH(woodPositionX, woodPositionY, woodScaleX, woodScaleY)))
            return true;
        break;
    }

    return false;
}

//��Q���ďo��
void MainScene::obstacleReAppearanceUpdate(const float deltaTime)
{
    if (birdPositionX < obstacleResetPositionX || bigRockPositionX < obstacleResetPositionX || smallRockPositionX < obstacleResetPositionX || woodPositionX < obstacleResetPositionX ||
        obstacleCollisionDetectionUpdate()) 
    {
        obstaclePositionResetUpdate();
        obstacleReLotteryUpdate(deltaTime);
    }
}

//��Q���Ē��I
void MainScene::obstacleReLotteryUpdate(const float deltaTime)
{
    obstaclePattern = randomObstacle(randomEngine);
    switch (obstaclePattern) {
    case 1:
        obstacleStatus = birdState;
        break;
    case 2:
        obstacleStatus = bigRockState;
        break;
    case 3:
        obstacleStatus = smallRockState;
        break;
    case 4:
        obstacleStatus = woodState;
        break;
    }
}

//��Q���ʒu���Z�b�g
void MainScene::obstaclePositionResetUpdate()
{
    obstacleInitialPositionY = randomObstaclePositionY(randomEngine);

    //��
    birdPositionX = obstacleInitialPositionX;
    birdPositionY = obstacleInitialPositionY;
    //��(��)
    bigRockPositionX = obstacleInitialPositionX;
    bigRockPositionY = obstacleInitialPositionY;
    //��(��)
    smallRockPositionX = obstacleInitialPositionX;
    smallRockPositionY = obstacleInitialPositionY;
    //��
    woodPositionX = obstacleInitialPositionX;
    woodPositionY = obstacleInitialPositionY;
}


//UI
//�Q�[�W
void MainScene::gaugeMoveUpdate()
{
    switch (gaugeStage) {
    case firstStage:
        gaugeWidth = firstStage;
        break;
    case secondStage:
        gaugeWidth = secondStage;
        break;
    case thirdStage:
        gaugeWidth = thirdStage;
        break;
    case forthStage:
        gaugeWidth = forthStage;
        break;
    case fifthStage:
        gaugeWidth = fifthStage;
        break;
    }
}


//�����蔻��֐�
//�x�[�X�����蔻��
bool MainScene::collisionDetectionBase(Rect& rect1, Rect& rect2) {

    if (rect1.left > rect2.right || rect1.right < rect2.left ||
        rect1.top > rect2.bottom || rect1.bottom < rect2.top) {
        return false;
    }
    return true;
}

//�v���C���[�T�C�Y�ݒ�ςݓ����蔻��
bool MainScene::PlayerCollisionDetection(Rect& rect2)
{
    Rect goldfishRange = RectWH(playerPositionX, playerPositionY, smallFishScaleX, smallFishScaleY);
    Rect catfishRange = RectWH(playerPositionX, playerPositionY, mediumFishScaleX, mediumFishScaleY);
    Rect carpRange = RectWH(playerPositionX, playerPositionY, largeFishScaleX, largeFishScaleY);
    
    switch (playerStatus) {
    case smallFishState:
        return collisionDetectionBase(goldfishRange, rect2);
        break;
    case mediumFishState:
        return collisionDetectionBase(catfishRange, rect2);
        break;
    case largeFishState:
        return collisionDetectionBase(carpRange, rect2);
        break;
    }

    return false;
}