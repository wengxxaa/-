//
//  CommentLayer.hpp
//  GGBond
//
//  Created by walle on 3/8/16.
//
//

#ifndef CommentLayer_h
#define CommentLayer_h



#include <string>

using namespace std;

#include "cocos2d.h"
USING_NS_CC;

#define MAX_STRING_LEN (1024*100)

class CommentUtil{
public:
    static int getCurrentLanguage()
    {
        int result = 0;
#if (COCOS2D_VERSION>0x00030000)
        LanguageType currentLanguage = Application::getInstance()->getCurrentLanguage();
        if(currentLanguage != LanguageType::CHINESE){
            result = 0;
        }else{
            result = 1;
        }
#else
        int currentLanguage = (int)CCApplication::sharedApplication()->getCurrentLanguage();
        if(currentLanguage != kLanguageChinese){
            result = 0;
        }else{
            result = 1;
        }
#endif
        
        
        return result; 
    }
    
    static unsigned long get_ElapsedRealtime();
    
    static string intToString(int data)
    {
        std::string ret;
        char* pBuf = (char*)malloc(MAX_STRING_LEN);
        if(pBuf != NULL){
            sprintf(pBuf,"%d",data);
            ret = pBuf;
            free(pBuf);
        }
        return ret;
    }
    
    static string getDate()
    {
        string d = "";
        struct tm *tm;
        time_t tmep ;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
        time(&tmep);
        tm =localtime(&tmep);
#else
        struct timeval now;
        gettimeofday(&now,NULL);
        tm =localtime(&now.tv_sec);
#endif
        int year = tm->tm_year +1900;
        d.append((intToString(year) ).c_str());
        int month = tm->tm_mon +1;
        if(month<10)
        {
            d.append(("0"+intToString(month) ).c_str());
        }else
        {
            d.append((intToString(month)).c_str());
        }
        int day = tm->tm_mday;
        if(day < 10)
        {
            d.append(("0" + intToString(day)) .c_str());
        }else
        {
            d.append(intToString(day) .c_str());
        }
        return d;
    }
};


#if (COCOS2D_VERSION>0x00030000)

class CommentLayer : public LayerColor{
public:
    static CommentLayer* create();
#if (COCOS2D_VERSION>0x00030000)
    static void showCommentLayer(Layer* layer, string url);
#else
    static void showCommentLayer(CCLayer* layer, string url);
#endif
    
private:
    virtual bool init();
    virtual void onEnter()
    {
        Layer::onEnter();
        registerTouchEvent();
    }
    virtual void onExit()
    {
        Layer::onExit();
    }
    void registerTouchEvent()
    {
        auto mListener = EventListenerTouchOneByOne::create();
        mListener->setSwallowTouches(true);
        mListener->onTouchBegan = CC_CALLBACK_2(CommentLayer::onTouchBegan, this);
        mListener->onTouchMoved = CC_CALLBACK_2(CommentLayer::onTouchMoved, this);
        mListener->onTouchEnded = CC_CALLBACK_2(CommentLayer::onTouchEnded, this);
        //node->getEventDispatcher()->addEventListenerWithFixedPriority(mListener,priority);
        this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mListener,this);
    }
    virtual bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
    {
        return true;
    }
    virtual void onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
    {
    }
    virtual void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
    {
    }
    
    void goCommentItemCallback(cocos2d::Ref* pSender);
    void buyCommentItemCallback(cocos2d::Ref* pSender);
    void noCommentItemCallback(cocos2d::Ref* pSender);
    
private:
    string mURL;
};

#else

class CommentLayer : public CCLayerColor{
public:
    static CommentLayer* create();
#if (COCOS2D_VERSION>0x00030000)
    static void showCommentLayer(Layer* layer, string url);
#else
    static void showCommentLayer(CCLayer* layer, string url);
#endif
    
private:
    virtual bool init();
    virtual void onEnter()
    {
        CCLayer::onEnter();
        setTouchEnabled(true);
    }
    virtual void onExit()
    {
        CCLayer::onExit();
    }
    virtual void registerWithTouchDispatcher()
    {
        CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority - 1, true);
    }
    virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
    {
        return true;
    }
    
    void goCommentItemCallback(CCObject* pSender);
    void buyCommentItemCallback(CCObject* pSender);
    void noCommentItemCallback(CCObject* pSender);
private:
    string mURL;
};


#endif


#endif /* CommentLayer_hpp */
