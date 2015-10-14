//
//  LogoScene.cpp
//  MazeJump
//
//  Created by wang haibo on 15/6/23.
//
//

#include "LogoScene.h"
#include "GameScene.h"
#include "LevelResourceManager.h"
USING_NS_CC;

Scene* LogoScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = LogoScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}
LogoScene::LogoScene()
{
    m_pLogoSprite = nullptr;
    m_pWhiteLayer = nullptr;
}
// on "init" you need to initialize your instance
bool LogoScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    auto size = Director::getInstance()->getVisibleSize();
    m_pLogoSprite = Sprite::create("logo.png");
    if(!m_pLogoSprite)
        return false;
    float scale = size.width / 960.0f;
    m_pLogoSprite->setPosition(size.width*0.5f, size.height*0.5f);
    m_pLogoSprite->setOpacity(0);
    m_pLogoSprite->setScale(scale);
    addChild(m_pLogoSprite);
    
    EaseSineOut* easeOut = EaseSineOut::create(FadeIn::create(1.0f));
    CallFunc* callFunc = CallFunc::create(CC_CALLBACK_0(LogoScene::precache, this));
    
    Sequence* sequence = Sequence::create( easeOut, callFunc, NULL);
    m_pLogoSprite->runAction(sequence);
    
    m_pWhiteLayer = LayerColor::create(Color4B::WHITE);
    if(!m_pWhiteLayer)
        return false;
    this->addChild(m_pWhiteLayer);
    EaseExponentialIn* fadeOut = EaseExponentialIn::create(FadeOut::create(1.0f));
    m_pWhiteLayer->runAction(fadeOut);
    
    return true;
}
void LogoScene::precache()
{
    if(!LevelResourceManager::getInstance()->init("LevelRes.plist"))
        CCLOGERROR("no LevelRes file!");
    
    if(m_pWhiteLayer)
    {
        EaseExponentialOut* fadeIn = EaseExponentialOut::create(FadeIn::create(1.0f));
        CallFunc* callFunc = CallFunc::create(CC_CALLBACK_0(LogoScene::endcache, this));
        Sequence* sequence = Sequence::create(fadeIn, callFunc, NULL);
        m_pWhiteLayer->runAction(sequence);
    }
}
void LogoScene::endcache()
{
    m_pLogoSprite->setVisible(false);
    auto scene = GameScene::createScene();
    Director::getInstance()->replaceScene(scene);
}