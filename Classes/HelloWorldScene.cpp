/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

#include "PluginUnityAds/PluginUnityAds.h"

USING_NS_CC;




/******************
 * Show logs
 ******************/
std::vector<std::string> msgbuf;
static void showMsg(const std::string& msg) {
    //
    Label *label = dynamic_cast<Label*>(Director::getInstance()->getNotificationNode());
    if (label == nullptr) {
        auto size = Director::getInstance()->getWinSize();
        label = Label::createWithSystemFont("test", "arial", 16);
        label->setAnchorPoint(Vec2(0,0));
        label->setTextColor(Color4B(0, 255, 0, 255));
        label->setPosition(10, size.height*0.1);
        Director::getInstance()->setNotificationNode(label);
    }
    
    msgbuf.push_back(msg);
    if (msgbuf.size() > 10) {
        msgbuf.erase(msgbuf.cbegin());
    }
    
    
    std::string text = "";
    for (int i = 0; i < msgbuf.size(); i++) {
        std::stringstream buf;
        buf << i << " " << msgbuf[i] << "\n";
        text = text + buf.str();
    }
    
    label->setString(text);
}

/******************
 * UnityAds Listener
 ******************/

class UAListener : public sdkbox::UnityAdsListener {
public:
    
    void unityAdsDidClick(const std::string& placementId) {
        std::stringstream buf;
        buf << "unityAdsDidClick" << "|" << placementId;
        showMsg(buf.str());
    }
    
    void unityAdsPlacementStateChanged(const std::string& placementId, sdkbox::PluginUnityAds::SBUnityAdsPlacementState oldState, sdkbox::PluginUnityAds::SBUnityAdsPlacementState newState) {
        std::stringstream buf;
        buf << "unityAdsPlacementStateChanged" << "|" << placementId << "|" << oldState << "|" << newState;
        showMsg(buf.str());
    }
    
    void unityAdsReady(const std::string& placementId) {
        std::stringstream buf;
        buf << "unityAdsReady" << "|" << placementId;
        showMsg(buf.str());
    }
    
    void unityAdsDidError(sdkbox::PluginUnityAds::SBUnityAdsError error, const std::string& message) {
        std::stringstream buf;
        buf << "unityAdsDidError" << "|" << error << "|" << message;
        showMsg(buf.str());
    }
    
    void unityAdsDidStart(const std::string& placementId) {
        std::stringstream buf;
        buf << "unityAdsDidStart" << "|" << placementId;
        showMsg(buf.str());
    }
    
    void unityAdsDidFinish(const std::string& placementId, sdkbox::PluginUnityAds::SBUnityAdsFinishState state) {
        std::stringstream buf;
        buf << "unityAdsDidFinish" << "|" << placementId << "|" << state;
        showMsg(buf.str());
    }
    
};









Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

//    auto visibleSize = Director::getInstance()->getVisibleSize();
//    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    createTestMenu();

    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}



void HelloWorld::createTestMenu() {
    sdkbox::PluginUnityAds::setListener(new UAListener());
    sdkbox::PluginUnityAds::init();
    
    if (!sdkbox::PluginUnityAds::isSupported()) {
        showMsg("UnityAds is not support this platform");
        return;
    }
    
    auto menu = Menu::create();
    
    menu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Show", "arial", 24), [](Ref*){
        std::string placement = "";
        if (sdkbox::PluginUnityAds::isReady(placement)) {
            sdkbox::PluginUnityAds::show(placement);
        } else {
            showMsg("UnityAds is not ready");
        }
    }));
    
    menu->alignItemsVerticallyWithPadding(10);
    addChild(menu);
}


