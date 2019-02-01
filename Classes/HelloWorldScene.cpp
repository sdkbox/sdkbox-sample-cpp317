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

#include "PluginShare/PluginShare.h"

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


class SListener : public sdkbox::ShareListener {
public:
    virtual void onShareState(const sdkbox::SocialShareResponse& response) {
        std::stringstream buf;
        buf << "onShareState:";
        switch (response.state) {
            case sdkbox::SocialShareState::SocialShareStateNone: { buf << "none"; break; }
            case sdkbox::SocialShareState::SocialShareStateUnkonw: { buf << "unknow"; break; }
            case sdkbox::SocialShareState::SocialShareStateBegin: { buf << "begin"; break; }
            case sdkbox::SocialShareState::SocialShareStateSuccess: { buf << "success"; break; }
            case sdkbox::SocialShareState::SocialShareStateFail: { buf << "failed"; break; }
            case sdkbox::SocialShareState::SocialShareStateCancelled: { buf << "cancel"; break; }
            case sdkbox::SocialShareStateSelectShow: { buf << "select show"; break; }
            case sdkbox::SocialShareStateSelectCancelled: { buf << "select cancel"; break; }
            case sdkbox::SocialShareStateSelected: { buf << "selected"; break; }
            default: { buf << response.state; break; }
        }

        buf << ":";
        switch(response.platform) {
            case sdkbox::SocialPlatform::Platform_Unknow: { buf << "unknow"; break; }
            case sdkbox::SocialPlatform::Platform_Twitter: { buf << "twitter"; break; }
            case sdkbox::SocialPlatform::Platform_Facebook: { buf << "facebook"; break; }
            case sdkbox::SocialPlatform::Platform_SMS: { buf << "sms"; break; }
            case sdkbox::SocialPlatform::Platform_Mail: { buf << "mail"; break; }
            case sdkbox::SocialPlatform::Platform_Native: { buf << "native"; break; }
            case sdkbox::SocialPlatform::Platform_Select: { buf << "select"; break; }
            case sdkbox::SocialPlatform::Platform_All: { buf << "all"; break; }
            default: { buf << response.platform; break; }
        }
        
        buf << ":";
        buf << response.error;
        
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

    menu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Share", "arial", 24), [this](Ref*){
        char alphabet[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
            'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
            'u', 'v', 'w', 'x', 'y', 'z'};
        std::stringstream shareText;
        shareText << "#sdkbox - the cure for sdk fatigue ";
        int max = random(5, 10);
        for (int i = 0; i < max; i++) {
            shareText << alphabet[random(0, 25)];
        }
        
        sdkbox::SocialShareInfo info;
        info.text = shareText.str();
        info.title = "sdkbox";
        info.image = this->_captureFilename;
        info.link = "http://www.sdkbox.com";
        info.platform = sdkbox::SocialPlatform::Platform_Select;
        info.showDialog = true; // share directly or share with dialog
        sdkbox::PluginShare::share(info);
    }));

    menu->addChild(MenuItemLabel::create(Label::createWithSystemFont("NativeShare", "arial", 24), [](Ref*){
        sdkbox::SocialShareInfo info;
        info.title = "title";
        info.text = "text";
        info.image = "button.png";
        info.link = "www.sdkbox.com";
        sdkbox::PluginShare::nativeShare(info);
    }));

    menu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Capture Screen", "arial", 24), [this](Ref*){
        std::string path = "screenshot.png";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        path = "/mnt/sdcard/screenshot.png";
#endif
        utils::captureScreen([this](bool suc, const std::string& path) {
            if (suc) {
                this->_captureFilename = path;
            }
        }, path);
    }));

    menu->alignItemsVerticallyWithPadding(10);
    addChild(menu);
    
    sdkbox::PluginShare::setListener(new SListener());
    sdkbox::PluginShare::init();
}
