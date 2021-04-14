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

#include "PluginFacebook/PluginFacebook.h"

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
 * Facebook Listener
 ******************/

class FBListener : public sdkbox::FacebookListener {

public:
    virtual void onLogin(bool isLogin, const std::string& msg) {
        std::stringstream buf;
        buf << "onLogin:" << isLogin << ":" << msg;
        showMsg(buf.str());
    }

    virtual void onSharedSuccess(const std::string& message) {
    }

    virtual void onSharedFailed(const std::string& message) {
    }

    virtual void onSharedCancel() {
        
    }

    virtual void onAPI(const std::string& key, const std::string& jsonData) {
        
    }

    virtual void onPermission(bool isLogin, const std::string& msg) {
        std::stringstream buf;
        buf << "onPermission:" << isLogin << ":" << msg;
        showMsg(buf.str());
    }

    virtual void onFetchFriends(bool ok, const std::string& msg) {
        std::stringstream buf;
        buf << "onFetchFriends:" << ok << ":" << msg;
        showMsg(buf.str());
    }

    virtual void onRequestInvitableFriends( const sdkbox::FBInvitableFriendsInfo& friends ) {
        
    }

    virtual void onInviteFriendsWithInviteIdsResult( bool result, const std::string& msg ) {
        
    }

    virtual void onInviteFriendsResult( bool result, const std::string& msg ) {
        
    }

    virtual void onGetUserInfo( const sdkbox::FBGraphUser& userInfo ) {
        
    }

    virtual void onRequestGiftResult(bool result, const std::string& msg) {
        
    }

    virtual void onSendGiftResult(bool result, const std::string& msg) {
        
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

    menu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Login/Logout", "arial", 24), [](Ref*){
        if (sdkbox::PluginFacebook::isLoggedIn()) {
            showMsg("FB to Logout");
            sdkbox::PluginFacebook::logout();
        } else {
            showMsg("FB to Login");
            std::vector<std::string> permissions;
            permissions.push_back(sdkbox::FB_PERM_READ_EMAIL);
            permissions.push_back(sdkbox::FB_PERM_READ_USER_FRIENDS);
            sdkbox::PluginFacebook::login(permissions);
        }
    }));
    menu->addChild(MenuItemLabel::create(Label::createWithSystemFont("CheckStatus", "arial", 24), [](Ref*){
        std::stringstream buf;
        for (auto& permission : sdkbox::PluginFacebook::getPermissionList()) {
            buf.str("");
            buf << "permission:";
            buf << permission.data();
            showMsg(buf.str());
        }
        CCLOG("##FB> Access token: %s", sdkbox::PluginFacebook::getAccessToken().c_str());
        buf.str("");
        buf << "User id:" << sdkbox::PluginFacebook::getUserID();
        showMsg(buf.str());
        buf.str("");
        buf << "FBSDK ver:" << sdkbox::PluginFacebook::getSDKVersion();
        showMsg(buf.str());
    }));

    menu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Firends", "arial", 24), [](Ref*){
        std::stringstream buf;
        for (auto& friendInfo : sdkbox::PluginFacebook::getFriends()) {
            buf.str("");
            buf << "Friend:" << friendInfo.getName();
            showMsg(buf.str());
        }

        // just get your friends that are using the app.
        // The number of friends defaults to 25.
        sdkbox::PluginFacebook::fetchFriends();
    }));

    menu->alignItemsVerticallyWithPadding(10);
    addChild(menu);

    sdkbox::PluginFacebook::setListener(new FBListener());
    sdkbox::PluginFacebook::init();
}
