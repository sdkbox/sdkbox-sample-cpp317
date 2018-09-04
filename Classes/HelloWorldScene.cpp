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

#include "PluginChartboost/PluginChartboost.h"

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


class CBListener : public sdkbox::ChartboostListener {
public:

    virtual void onChartboostCached(const std::string& name) override {
        showMsg("onChartboostCached:" + name);
    }

    virtual bool onChartboostShouldDisplay(const std::string& name) override {
        showMsg("onChartboostShouldDisplay:" + name);

        return true;
    }

    virtual void onChartboostDisplay(const std::string& name) override {
        showMsg("onChartboostDisplay:" + name);
    }

    virtual void onChartboostDismiss(const std::string& name) override {
        showMsg("onChartboostDismiss:" + name);
    }

    virtual void onChartboostClose(const std::string& name) override {
        showMsg("onChartboostClose:" + name);
    }

    virtual void onChartboostClick(const std::string& name) override {
        showMsg("onChartboostClick:" + name);
    }

    virtual void onChartboostReward(const std::string& name, int reward) override {
        std::stringstream buf;
        
        buf << "onChartboostReward" << ":" << name << ":" << reward;
        showMsg(buf.str());
    }

    virtual void onChartboostFailedToLoad(const std::string& name, sdkbox::CB_LoadError e) override {
        std::stringstream buf;
        
        buf << "onChartboostFailedToLoad" << ":" << name << ":" << e;
        showMsg(buf.str());
    }

    virtual void onChartboostFailToRecordClick(const std::string& name, sdkbox::CB_ClickError e) override {
        std::stringstream buf;
        
        buf << "onChartboostFailToRecordClick" << ":" << name << ":" << e;
        showMsg(buf.str());
    }

    virtual void onChartboostConfirmation() override {
        showMsg("onChartboostConfirmation");
    }

    virtual void onChartboostCompleteStore() override {
        showMsg("onChartboostCompleteStore");
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

    menu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Show", "arial", 24), [](Ref*){
        sdkbox::PluginChartboost::show("Level Complete");
    }));
    
    menu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Cache", "arial", 24), [](Ref*){
        sdkbox::PluginChartboost::cache("Level Complete");
    }));
    
    menu->alignItemsVerticallyWithPadding(10);
    addChild(menu);
    
    sdkbox::PluginChartboost::setListener(new CBListener());
    sdkbox::PluginChartboost::init();
}
