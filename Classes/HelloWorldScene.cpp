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

#include "PluginApteligent/PluginApteligent.h"

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
    cocos2d::log("Log: %s", msg.c_str());
}


class ALListener : public sdkbox::ApteligentListener {
public:

    virtual void onCrashOnLastLoad() override {
        showMsg("Apteligent: crashOnLastLoad.");
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
    auto menu = Menu::create();

    menu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Crash Me", "arial", 24), [](Ref*){
        //crash
        int i = 0;
        CCLOG("%d", 1/i);
    }));
    
    menu->alignItemsVerticallyWithPadding(10);
    addChild(menu);
    
    sdkbox::PluginApteligent::setListener(new ALListener());
    sdkbox::PluginApteligent::init();


    // filter
    sdkbox::PluginApteligent::addFilter("sensitiveURL");
    sdkbox::PluginApteligent::setLoggingLevel(sdkbox::CRLoggingLevelInfo);
    sdkbox::PluginApteligent::setAsyncBreadcrumbMode(true);
    sdkbox::PluginApteligent::updateLocation(30.67, 104.06);
    sdkbox::PluginApteligent::setUsername("MrsCritter");
    sdkbox::PluginApteligent::setValueforKey("5", "Game Level");
    sdkbox::PluginApteligent::leaveBreadcrumb("User tapped start button");

    sdkbox::PluginApteligent::beginUserflow("login");
    float v = cocos2d::random(10, 100);
    if (v > 50) {
        CCLOG("end user flow: login");
        sdkbox::PluginApteligent::setValueforUserflow(int(CCRANDOM_0_1()*10), "login");
        int value = sdkbox::PluginApteligent::valueForUserflow("login");
        sdkbox::PluginApteligent::setValueforUserflow(value+5, "login");
        sdkbox::PluginApteligent::endUserflow("login");
    } else if (v > 30) {
        CCLOG("fail user flow: login");
        sdkbox::PluginApteligent::failUserflow("login");
    } else {
        CCLOG("cancel user flow: login");
        sdkbox::PluginApteligent::cancelUserflow("login");
    }

    // logging network requests
    sdkbox::PluginApteligent::logNetworkRequest("GET", "http://www.abc123def456.com", 2.0, 1000, 100, 200);

    std::stringstream buf;
    buf << (sdkbox::PluginApteligent::didCrashOnLastLoad() ? "crash last time" : "no crash last time");
    showMsg(buf.str());
}
