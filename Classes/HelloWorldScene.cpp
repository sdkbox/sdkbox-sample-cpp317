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

#include "PluginOneSignal/PluginOneSignal.h"

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


class OSListener : public sdkbox::OneSignalListener {

private:
    HelloWorld* scene;
    
public:
    
    void setScene(HelloWorld* s) {
        scene = s;
    }
    
    void onNotificationOpened(const std::string &message) {
        showMsg("onNotificationOpened:" + message);
    }
    
    void onNotification(bool isActive, const std::string& message, const std::string& additionalData) {
        std::stringstream buf;
        
        buf << "onNotification:" << isActive << ":" << message << ":" << additionalData;
        showMsg(buf.str());
    }

    void onSendTag(bool success, const std::string& k, const std::string& message) {
        std::stringstream buf;
        
        buf << "onSendTag:" << success << ":" << k << ":" << message;
        showMsg(buf.str());
    }

    void onGetTags(const std::string& jsonString) {
        showMsg("onGetTags:" + jsonString);
    }

    void onIdsAvailable(const std::string& userId, const std::string& pushToken) {
        std::stringstream buf;
        
        buf << "onIdsAvailable:" << userId << ":" << pushToken;
        showMsg(buf.str());
        scene->userid = userId;
    }

    void onPostNotification(bool success, const std::string& message) {
        std::stringstream buf;
        
        buf << "onPostNotification:" << success << ":" << message;
        showMsg(buf.str());
    }

    void onNotificationReceived(const std::string& message) {
        showMsg("onNotificationReceived:" + message);
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

    menu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Post Notification", "arial", 24), [this](Ref*){
        std::string jsonStr = "{\"contents\":{\"en\":\"Test Message\"},\"include_player_ids\":[\"4db7cb0f-daca-49d6-b3dd-a96774de9f72\",\"33ad9d96-df00-42a2-b5ab-73417f777d42\",\"5ea075c5-016d-4f55-891c-3b6c4b393e49\",\"USER_ID\"],\"data\":{\"key\":\"val\"}}";

        std::string patter = "USER_ID";
        size_t pos = jsonStr.find(patter);
        jsonStr.replace(pos, patter.size(), this->userid);

        // https://documentation.onesignal.com/v2.0/docs/notifications-create-notification
        sdkbox::PluginOneSignal::postNotification(jsonStr);
    }));
    
    menu->alignItemsVerticallyWithPadding(10);
    addChild(menu);
    
    OSListener *l = new OSListener();
    l->setScene(this);
    sdkbox::PluginOneSignal::setListener(l);
    sdkbox::PluginOneSignal::init();
    
    sdkbox::PluginOneSignal::setSubscription(true);
    sdkbox::PluginOneSignal::setEmail("test@example.com");
    sdkbox::PluginOneSignal::sendTag("key", "value");
    sdkbox::PluginOneSignal::sendTag("key1", "value1");
    sdkbox::PluginOneSignal::getTags();
    sdkbox::PluginOneSignal::idsAvailable();
    sdkbox::PluginOneSignal::enableInAppAlertNotification(true);

    sdkbox::PluginOneSignal::promptLocation();
}
