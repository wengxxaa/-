//
//  CommentLayer.cpp
//  GGBond
//
//  Created by walle on 3/8/16.
//
//

#include "CommentLayer.h"
#include <sys/socket.h>
#include <sys/sysctl.h>
#include "cj/cjconfig.h"


unsigned long CommentUtil::get_ElapsedRealtime()
{
    NSTimeInterval time = [NSProcessInfo processInfo].systemUptime;
    return (long)time * 1000;
}

CommentLayer* CommentLayer::create()
{
    CommentLayer *pRet = new CommentLayer();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}


#if (COCOS2D_VERSION>0x00030000)

void CommentLayer::showCommentLayer(Layer* layer, string url){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
    CommentLayer* commentLayer = CommentLayer::create();
    if(commentLayer){
        commentLayer->mURL = url;
        if(layer != NULL){
            layer->addChild(commentLayer, 33333333);
        }else{
            Director::getInstance()->getRunningScene()->addChild(commentLayer, 33333333);
        }
    }
    
#endif
}


bool CommentLayer::init(){
    if (!LayerColor::initWithColor(Color4B(0, 0, 0, 125)))
    {
        return false;
    }
    //如果已经评论不再弹出
    bool haveComment = UserDefault::getInstance()->getBoolForKey("HaveComment", false);
    if(haveComment == true){
        //测试模式没有这个约束
#if COCOS2D_DEBUG > 0
#else
        return false;
#endif
    }
    
    //玩15分钟才会弹
    string secondString = UserDefault::getInstance()->getStringForKey("PlayGameSecondTime","");
    unsigned long second = atol(secondString.c_str());
    unsigned long nowSecond = CommentUtil::get_ElapsedRealtime();
    if((nowSecond - second) >= (15 * 60 * 1000) ){
    //if((nowSecond - second) >= (5 * 1000) ){
        //玩15分钟才会弹
    }else{
        //测试模式没有这个约束
#if COCOS2D_DEBUG > 0
#else
        return false;
#endif
    }
    
    
    //当天就弹一次
    bool isTheSameDay = strcmp(UserDefault::getInstance()->getStringForKey("LAST_PLAY_TIME_DATE_KEY","").c_str(),CommentUtil::getDate().c_str()) == 0;
    if(isTheSameDay == true){
        //测试模式没有这个约束
#if COCOS2D_DEBUG > 0
#else
        return false;
#endif
    }else{
        UserDefault::getInstance()->setStringForKey("LAST_PLAY_TIME_DATE_KEY",CommentUtil::getDate().c_str());
    }
    
    //终生查看5次不再弹出
    int viewCount = UserDefault::getInstance()->getIntegerForKey("ViewCommentLayerCount", 0);
    if(viewCount == 5){
        //测试模式没有这个约束
#if COCOS2D_DEBUG > 0
#else
        return false;
#endif
    }
    viewCount++;
    UserDefault::getInstance()->setIntegerForKey("ViewCommentLayerCount", viewCount);
    
    auto winSize = Director::getInstance()->getVisibleSize();
    
    Sprite* Dialog_bg = Sprite::create(__String::createWithFormat("comment/c_bg_%d.png", CommentUtil::getCurrentLanguage())->getCString());
    Dialog_bg->setPosition(Vec2(winSize/2.0f)+Vec2(5,30));
    this->addChild(Dialog_bg);
    
    float itemStartPositionY = 280;
    const int positionYMargin = 100;
    const int itemPositionX = 195;
    
    MenuItemImage* goCommentItem = MenuItemImage::create(__String::createWithFormat("comment/c_ok_comment_%d.png", CommentUtil::getCurrentLanguage())->getCString(), __String::createWithFormat("comment/c_ok_comment_%d.png", CommentUtil::getCurrentLanguage())->getCString(),CC_CALLBACK_1(CommentLayer::goCommentItemCallback,this));
    goCommentItem->setPosition(Vec2(itemPositionX, itemStartPositionY));
    
    
    
    itemStartPositionY -= positionYMargin;
    
    MenuItemImage* buyCommentItem = MenuItemImage::create(__String::createWithFormat("comment/c_feedback_comment_%d.png", CommentUtil::getCurrentLanguage())->getCString(), __String::createWithFormat("comment/c_buy_comment_%d.png", CommentUtil::getCurrentLanguage())->getCString(),CC_CALLBACK_1(CommentLayer::buyCommentItemCallback,this));
    buyCommentItem->setPosition(Vec2(itemPositionX, itemStartPositionY));
   
    
    itemStartPositionY -= positionYMargin;
    
    MenuItemImage* noCommentItem = MenuItemImage::create(__String::createWithFormat("comment/c_no_comment_%d.png", CommentUtil::getCurrentLanguage())->getCString(), __String::createWithFormat("comment/c_no_comment_%d.png", CommentUtil::getCurrentLanguage())->getCString(), CC_CALLBACK_1(CommentLayer::noCommentItemCallback,this));
    noCommentItem->setPosition(Vec2(itemPositionX, itemStartPositionY));
    
    
    Menu* pMenu = Menu::create(goCommentItem,buyCommentItem, noCommentItem, NULL);
    Dialog_bg->addChild(pMenu);
    pMenu->setPosition(Vec2(0, 0));
    
    return true;
}

void CommentLayer::goCommentItemCallback(cocos2d::Ref* pSender){
    
    cj::starUMCount(1);
    
    GameDataInstance()->d_haoping=true;
    
    UserDefault::getInstance()->setBoolForKey("GoComment", true);
    
    if(mURL.size() > 0){
        NSString *str = [NSString stringWithUTF8String:mURL.c_str()];
        [[UIApplication sharedApplication] openURL:[NSURL URLWithString:str]];
    }
    
    this->removeFromParent();
    
}

void CommentLayer::buyCommentItemCallback(cocos2d::Ref* pSender){
    
    cj::starUMCount(2);
    
    goCommentItemCallback(nullptr);
}

void CommentLayer::noCommentItemCallback(cocos2d::Ref* pSender){
    
    cj::starUMCount(3);
    
    UserDefault::getInstance()->setStringForKey("ShowCommentDay", CommentUtil::getDate().c_str());
    this->removeFromParent();
}

#else

void CommentLayer::showCommentLayer(CCLayer* layer, string url){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    CommentLayer* commentLayer = CommentLayer::create();
    if(commentLayer){
        commentLayer->mURL = url;
        if(layer != NULL){
            layer->addChild(commentLayer, 33333333);
        }else{
            CCDirector::sharedDirector()->getRunningScene()->addChild(commentLayer, 33333333);
        }
    }
    
#endif
}


bool CommentLayer::init(){
    if (!CCLayerColor::initWithColor(ccc4(0, 0, 0, 125)))
    {
        return false;
    }
    
    bool haveComment = CCUserDefault::sharedUserDefault()->getBoolForKey("HaveComment", false);
    if(haveComment == true){
#if COCOS2D_DEBUG > 0
#else
        return false;
#endif
    }
    
    
    //玩15分钟才会弹
    string secondString = CCUserDefault::sharedUserDefault()->getStringForKey("PlayGameSecondTime","");
    unsigned long second = atol(secondString.c_str());
    unsigned long nowSecond = CommentUtil::get_ElapsedRealtime();
    if((nowSecond - second) >= (15 * 60 * 1000) ){
        //if((nowSecond - second) >= (5 * 1000) ){
        //玩15分钟才会弹
    }else{
#if COCOS2D_DEBUG > 0
#else
        return false;
#endif
    }
    //当天就弹一次
    bool isTheSameDay = strcmp(CCUserDefault::sharedUserDefault()->getStringForKey("LAST_PLAY_TIME_DATE_KEY","").c_str(),CommentUtil::getDate().c_str()) == 0;
    if(isTheSameDay == true){
#if COCOS2D_DEBUG > 0
#else
        return false;
#endif
    }else{
        CCUserDefault::sharedUserDefault()->setStringForKey("LAST_PLAY_TIME_DATE_KEY",CommentUtil::getDate().c_str());
    }
    
    //终生查看5次不再弹出
    int viewCount = CCUserDefault::sharedUserDefault()->getIntegerForKey("ViewCommentLayerCount", 0);
    if(viewCount == 5){
#if COCOS2D_DEBUG > 0
#else
        return false;
#endif
    }
    viewCount++;
    CCUserDefault::sharedUserDefault()->setIntegerForKey("ViewCommentLayerCount", viewCount);
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    CCSprite* Dialog_bg = CCSprite::create(CCString::createWithFormat("comment/c_bg_%d.png", CommentUtil::getCurrentLanguage())->getCString());
    Dialog_bg->setPosition(ccp(winSize.width/2 + 4, 586));
    this->addChild(Dialog_bg);
    
    float itemStartPositionY = 400;
    const int positionYMargin = 130;
    const int itemPositionX = winSize.width/2 - 40;
    
    CCMenuItemImage* goCommentItem = CCMenuItemImage::create(CCString::createWithFormat("comment/c_ok_comment_%d.png", CommentUtil::getCurrentLanguage())->getCString(), CCString::createWithFormat("comment/c_ok_comment_%d.png", CommentUtil::getCurrentLanguage())->getCString());
    goCommentItem->setPosition(ccp(itemPositionX, itemStartPositionY));
    goCommentItem->setTarget(this, menu_selector(CommentLayer::goCommentItemCallback));
    
    
    itemStartPositionY -= positionYMargin;
    
    CCMenuItemImage* buyCommentItem = CCMenuItemImage::create(CCString::createWithFormat("comment/c_feedback_comment_%d.png", CommentUtil::getCurrentLanguage())->getCString(), CCString::createWithFormat("comment/c_buy_comment_%d.png", CommentUtil::getCurrentLanguage())->getCString());
    buyCommentItem->setPosition(ccp(itemPositionX, itemStartPositionY));
    buyCommentItem->setTarget(this, menu_selector(CommentLayer::buyCommentItemCallback));
    
    itemStartPositionY -= positionYMargin;
    
    CCMenuItemImage* noCommentItem = CCMenuItemImage::create(CCString::createWithFormat("comment/c_no_comment_%d.png", CommentUtil::getCurrentLanguage())->getCString(), CCString::createWithFormat("comment/c_no_comment_%d.png", CommentUtil::getCurrentLanguage())->getCString());
    noCommentItem->setPosition(ccp(itemPositionX, itemStartPositionY));
    noCommentItem->setTarget(this, menu_selector(CommentLayer::noCommentItemCallback));
    
    
    CCMenu* pMenu = CCMenu::create(goCommentItem,buyCommentItem, noCommentItem, NULL);
    Dialog_bg->addChild(pMenu);
    pMenu->setTouchPriority(kCCMenuHandlerPriority - 2);
    pMenu->setPosition(ccp(0, 0));
    
    return true;
}

void CommentLayer::goCommentItemCallback(CCObject* pSender){
    CCUserDefault::sharedUserDefault()->setBoolForKey("GoComment", true);
    
    if(mURL.size() > 0){
        NSString *str = [NSString stringWithUTF8String:mURL.c_str()];
        [[UIApplication sharedApplication] openURL:[NSURL URLWithString:str]];
    }
    
 
    this->removeFromParent();
    
}

void CommentLayer::buyCommentItemCallback(CCObject* pSender){
    goCommentItemCallback(nullptr);
}

void CommentLayer::noCommentItemCallback(CCObject* pSender){
    CCUserDefault::sharedUserDefault()->setStringForKey("ShowCommentDay", CommentUtil::getDate().c_str());
    this->removeFromParent();
}
#endif

