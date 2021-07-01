//
// MainScene.cpp
//

#include "Base/pch.h"
#include "Base/dxtk.h"
#include "SceneFactory.h"

// Initialize member variables.
MainScene::MainScene() : dx9GpuDescriptor{}
{
    //コンストラクター
    //変数の初期化（0にする）

}

// Initialize a variable and audio resources.
void MainScene::Initialize()
{
    //ゲームシーンの初期化
    //変数を初期化（具体的な数値を設定する）
    //サウンドを読み込んだりする
    
    //背景
    bgPositionX = 0.0f;


    //ゲージ段階
    gaugeStage = firstStage;


    //プレイヤー
    //座標
    playerPositionX = 100.0f;
    playerPositionY = 300.0f;

    //慣性
    playerInertiaX = 0.0f;
    playerInertiaY = 0.0f;

    //無敵時間
    speedUpTime = 0.0f;

    //状態
    playerStatus = smallFishState;
    playerSpeedStatus = smallFishSpeedState;


    //虫(アイテム)
    wormPositionX = 1500.0f;
    wormPositionY = 300.0f;

    //虫位置リセット
    std::random_device rnd_dev;
    randomEngine = std::mt19937(rnd_dev());
    randomWormPositionY = std::uniform_int_distribution<int>(playerMoveRangeTop, playerMoveRangeBottom);


    //障害物
    //障害物座標
    //鳥
    birdPositionX = 1500.0f;
    birdPositionY = 300.0f;

    //岩(大)
    bigRockPositionX = 1500.0f;
    bigRockPositionY = 300.0f;

    //岩(小)
    smallRockPositionX = 1500.0f;
    smallRockPositionY = 300.0f;

    //木
    woodPositionX = 1500.0f;
    woodPositionY = 300.0f;

    //障害物状態
    obstacleStatus = bigRockState;

    //ランダムリセット座標
    randomObstaclePositionY = std::uniform_int_distribution<int>(playerMoveRangeTop, playerMoveRangeBottom);
    obstacleInitialPositionY = randomObstaclePositionY(randomEngine);

    //ランダム
    randomObstacle = std::uniform_int_distribution<int>(1, 4);


    //UI
    //ゲージ
    gaugeWidth = 140;
    
}

// Allocate all memory the Direct3D and Direct2D resources.
void MainScene::LoadAssets()
{
    //グラフィックの設定をする
    //Windows10はドライバがアップデートされると読み込み直しが必要になる

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


    // グラフィックリソースの初期化処理

    //背景
    //bgTestSprite = DX9::Sprite::CreateFromFile(DXTK->Device9, L"bgTestSprite.png");
    bgTestSprite = DX9::Sprite::CreateFromFile(DXTK->Device9, L"bgSprite02.png");
    
    
    //プレイヤー
    //金魚
    smallFishTestSprite = DX9::Sprite::CreateFromFile(DXTK->Device9, L"goldfishTestSprite.png");

    //ナマズ
    //mediumFishTestSprite = DX9::Sprite::CreateFromFile(DXTK->Device9, L"catfishTestSprite.png");
    mediumFishTestSprite = DX9::Sprite::CreateFromFile(DXTK->Device9, L"mediumFishSprite.png");

    //鯉
    largeFishTestSprite = DX9::Sprite::CreateFromFile(DXTK->Device9, L"carpTestSprite.png");


    //虫(アイテム)
    //wormTestSprite = DX9::Sprite::CreateFromFile(DXTK->Device9, L"wormTestSprite.png");
    wormTestSprite = DX9::Sprite::CreateFromFile(DXTK->Device9, L"wormSprite.png");


    //障害物
    //鳥
    //birdTestSprite = DX9::Sprite::CreateFromFile(DXTK->Device9, L"kingfisherTestSprite.png");
    birdTestSprite = DX9::Sprite::CreateFromFile(DXTK->Device9, L"birdSprite.png");

    //岩(大)
    bigRockTestSprite = DX9::Sprite::CreateFromFile(DXTK->Device9, L"bigRockTestSprite.png");

    //岩(小)
    smallRockTestSprite = DX9::Sprite::CreateFromFile(DXTK->Device9, L"smallRockTestSprite.png");

    //木
    woodTestSprite = DX9::Sprite::CreateFromFile(DXTK->Device9, L"woodTestSprite.png");


    //UI
    //ゲージ
    gaugeTestSprite = DX9::Sprite::CreateFromFile(DXTK->Device9, L"gaugeTestSprite.png");
    gaugeBgTestSprite = DX9::Sprite::CreateFromFile(DXTK->Device9, L"gaugeBgTestSprite.png");


    //デバッグ用
    playerStatusFont = DX9::SpriteFont::CreateFromFontName(DXTK->Device9, L"ＭＳ Ｐ明朝", 20);
    gaugeStageFont = DX9::SpriteFont::CreateFromFontName(DXTK->Device9, L"ＭＳ Ｐ明朝", 20);

}

// Releasing resources required for termination.
void MainScene::Terminate()
{
    DXTK->ResetAudioEngine();
    DXTK->WaitForGpu();

    // TODO: Add your Termination logic here.
    //ゲーム(シーン)終了時に何かしたい場合に使う
}

// Direct3D resource cleanup.
void MainScene::OnDeviceLost()
{
    //ドライバーアップデートを検出した場合に使う
    //読み込みなおす前に何かしたい場合に何か書く
}

// Restart any looped sounds here
void MainScene::OnRestartSound()
{
    //サウンドデバイスが切り替わるとサウンドが全部停止する
    //BGMなどを再スタートさせる
}

// Updates the scene.
NextScene MainScene::Update(const float deltaTime)
{
    // If you use 'deltaTime', remove it.
    UNREFERENCED_PARAMETER(deltaTime);

    // TODO: Add your game logic here.
    //ゲームを動かすプログラムを記述する

    //背景
    bgMoveUpdate(deltaTime);

    //スクロール速度
    shiftBgMoveSpeed(deltaTime);

    //スクロース速度割当
    setBgScrollSpeed();


    //状態遷移割当
    gaugePlayerStateAssignUpdate();
    
    //状態遷移
    gaugeStageUpdate(deltaTime);
    

    //プレイヤー
    //移動範囲
    playerMoveRangeUpdate();

    //自動移動
    playerAutoMoveUpdate(deltaTime);

    //キーボード操作
    playerControlKeyboardUpdate(deltaTime);

    //パッド操作
    playerControlGamepadUpdate(deltaTime);


    //虫(アイテム)
    //移動
    wormMoveUpdate(deltaTime);

    //虫再出現
    //wormReAppearanceUpdate(deltaTime);

    //障害物
    //移動
    obstacleMoveUpdate(deltaTime);

    //障害物再出現
    if (birdPositionX < obstacleResetPositionX || bigRockPositionX < obstacleResetPositionX || smallRockPositionX < obstacleResetPositionX || woodPositionX < obstacleResetPositionX)
    {
        obstaclePositionResetUpdate();
        obstacleReLotteryUpdate(deltaTime);
    }


    //UI
    //ゲージ
    gaugeMoveUpdate();
   

    return NextScene::Continue;
}

// Draws the scene.
void MainScene::Render()
{
    // TODO: Add your rendering code here.
    //描画のプログラムを書く
    
    DXTK->Direct3D9->Clear(DX9::Colors::RGBA(0, 0, 0, 255));  // 手順1

    DXTK->Direct3D9->BeginScene();    // 手順2
    DX9::SpriteBatch->Begin();        // 手順4

    // (ここに2D描画の処理が入る)     // 手順5

    //背景
    DX9::SpriteBatch->DrawSimple(bgTestSprite.Get(), SimpleMath::Vector3(bgPositionX, 0, 10));


    //プレイヤー
    //金魚
    if (gaugeStage == firstStage || gaugeStage == secondStage) {
        DX9::SpriteBatch->DrawSimple(smallFishTestSprite.Get(), SimpleMath::Vector3(playerPositionX, playerPositionY, -5));
    }
    //ナマズ
    if (gaugeStage == thirdStage || gaugeStage == forthStage) {
        DX9::SpriteBatch->DrawSimple(mediumFishTestSprite.Get(), SimpleMath::Vector3(playerPositionX, playerPositionY, -5));
    }
    //鯉
    if (gaugeStage == fifthStage) {
        DX9::SpriteBatch->DrawSimple(largeFishTestSprite.Get(), SimpleMath::Vector3(playerPositionX, playerPositionY, -5));
    }


    //虫(アイテム)
    DX9::SpriteBatch->DrawSimple(wormTestSprite.Get(), SimpleMath::Vector3(wormPositionX, wormPositionY, 0));


    //障害物
    //鳥 
    DX9::SpriteBatch->DrawSimple(birdTestSprite.Get(), SimpleMath::Vector3(birdPositionX, birdPositionY, -4));
   
    //岩(大)
    DX9::SpriteBatch->DrawSimple(bigRockTestSprite.Get(), SimpleMath::Vector3(bigRockPositionX, bigRockPositionY, -4));
    
    //岩(小)
    DX9::SpriteBatch->DrawSimple(smallRockTestSprite.Get(), SimpleMath::Vector3(smallRockPositionX, smallRockPositionY, -400));

    //木
    DX9::SpriteBatch->DrawSimple(woodTestSprite.Get(), SimpleMath::Vector3(woodPositionX, woodPositionY, -4));


    //UI
    //ゲージ
    DX9::SpriteBatch->DrawSimple(gaugeTestSprite.Get(), SimpleMath::Vector3(100, 50, 8),
        RectWH(0.0f, 0.0f, gaugeWidth, 100.0f));
    DX9::SpriteBatch->DrawSimple(gaugeBgTestSprite.Get(), SimpleMath::Vector3(100, 50, 9));


    //デバッグ用
    //DX9::SpriteBatch->DrawString(playerStatusFont.Get(), SimpleMath::Vector2(0, 670), DX9::Colors::RGBA(0, 0, 0, 255), L"playerStatus:%d", playerStatus+1);
    //DX9::SpriteBatch->DrawString(gaugeStageFont.Get(), SimpleMath::Vector2(230, 670), DX9::Colors::RGBA(0, 0, 0, 255), L"gaugeStage:%d", gaugeStage+1);


    DX9::SpriteBatch->End();          // 手順6
    DXTK->Direct3D9->EndScene();      // 手順7

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

//Update内関数の定義

//背景
void MainScene::bgMoveUpdate(const float deltaTime)
{
    //位置リセット
    if (bgPositionX <= -bgResetPosition)
        bgPositionX = 0;
}

//スクロール速度
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

//スクロール速度割当
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


//状態遷移割当
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

//状態遷移
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


//プレイヤー
//移動可能範囲
void MainScene::playerMoveRangeUpdate()
{
    //上・左
    if (playerPositionY <= playerMoveRangeTop)
    {
        playerPositionY = playerMoveRangeTop;
    }
    if (playerPositionX <= playerMoveRangeLeft)
    {
        playerPositionX = playerMoveRangeLeft;
    }
    //右・下
    //金魚
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
    //ナマズ
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
    //鯉
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

//自動移動
void MainScene::playerAutoMoveUpdate(const float deltaTime)
{
    playerPositionX -= playerAutoMoveSpeed * deltaTime;
}

//キーボード操作
void MainScene::playerControlKeyboardUpdate(const float deltaTime)
{
    //左右
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

    //上下
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

//パッド操作
void MainScene::playerControlGamepadUpdate(const float deltaTime)
{
    //十字キー
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

    //スティック
    playerPositionX += gamepadPlayerMoveSpeedX * DXTK->GamePadState[0].thumbSticks.leftX * deltaTime;
    playerPositionY -= gamepadPlayerMoveSpeedY * DXTK->GamePadState[0].thumbSticks.leftY * deltaTime;
}


//虫(アイテム)
//移動
void MainScene::wormMoveUpdate(const float deltaTime)
{
    wormPositionX -= 350 * deltaTime;

    //ループ
    if (wormPositionX <= -200)
    {
        wormPositionX = 1500;
    }
}

//虫再出現
void MainScene::wormReAppearanceUpdate(const float deltaTime)
{
    if (wormPositionX <= wormResetPositionX || wormCollisionDetectionUpdate()) 
    {
        wormPositionResetUpdate();
    }
}

//位置リセット
void MainScene::wormPositionResetUpdate()
{
    wormInitialPositionY = randomWormPositionY(randomEngine);

    wormPositionX = wormInitialPositionX;
    wormPositionY = wormInitialPositionY;
}

//虫当たり判定
bool MainScene::wormCollisionDetectionUpdate()
{
    if (PlayerCollisionDetection(RectWH(wormPositionX, wormPositionY, wormScaleX, wormScaleY)))
        return true;
    else
        return false;
}


//障害物
//障害物移動 
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
//鳥
void MainScene::birdObstacleMoveUpdate(const float deltaTime)
{
    birdPositionX -= 300.0f * deltaTime;
}

//岩(大)
void MainScene::bigRockObstacleMoveUpdate(const float deltaTime)
{
    bigRockPositionX -= 300.0f * deltaTime;
}

//岩(小)
void MainScene::smallRockObstacleMoveUpdate(const float deltaTime)
{
    smallRockPositionX -= 300.0f * deltaTime;
}

//木
void MainScene::woodObstacleMoveUpdate(const float deltaTime)
{
    woodPositionX -= 300.0f * deltaTime;
}
*/

//障害物当たり判定
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

//障害物再出現
void MainScene::obstacleReAppearanceUpdate(const float deltaTime)
{
    if (birdPositionX < obstacleResetPositionX || bigRockPositionX < obstacleResetPositionX || smallRockPositionX < obstacleResetPositionX || woodPositionX < obstacleResetPositionX ||
        obstacleCollisionDetectionUpdate()) 
    {
        obstaclePositionResetUpdate();
        obstacleReLotteryUpdate(deltaTime);
    }
}

//障害物再抽選
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

//障害物位置リセット
void MainScene::obstaclePositionResetUpdate()
{
    obstacleInitialPositionY = randomObstaclePositionY(randomEngine);

    //鳥
    birdPositionX = obstacleInitialPositionX;
    birdPositionY = obstacleInitialPositionY;
    //岩(大)
    bigRockPositionX = obstacleInitialPositionX;
    bigRockPositionY = obstacleInitialPositionY;
    //岩(小)
    smallRockPositionX = obstacleInitialPositionX;
    smallRockPositionY = obstacleInitialPositionY;
    //木
    woodPositionX = obstacleInitialPositionX;
    woodPositionY = obstacleInitialPositionY;
}


//UI
//ゲージ
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


//当たり判定関数
//ベース当たり判定
bool MainScene::collisionDetectionBase(Rect& rect1, Rect& rect2) {

    if (rect1.left > rect2.right || rect1.right < rect2.left ||
        rect1.top > rect2.bottom || rect1.bottom < rect2.top) {
        return false;
    }
    return true;
}

//プレイヤーサイズ設定済み当たり判定
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