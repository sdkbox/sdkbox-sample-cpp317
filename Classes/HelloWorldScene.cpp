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

#include "PluginSdkboxAds/PluginSdkboxAds.h"

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


class SAListener : public sdkbox::PluginSdkboxAdsListener {
public:
    virtual void onAdAction( const std::string& ad_unit_id, const std::string& zone, sdkbox::AdActionType action_type) override {
        std::stringstream buf;
        buf << "onAdAction:" << ad_unit_id << ":" << zone << ":";
        switch (action_type) {
            case sdkbox::AdActionType::LOADED: {
                buf << "loaded";
                break;
            }
            case sdkbox::AdActionType::LOAD_FAILED: {
                buf << "load failed";
                
                std::map<std::string, std::string> opts;
                opts.insert(std::pair<std::string, std::string>(zone, "true"));
                sdkbox::PluginSdkboxAds::cacheControl(ad_unit_id, opts);
                break;
            }
            case sdkbox::AdActionType::CLICKED: {
                buf << "clicked";
                break;
            }
            case sdkbox::AdActionType::REWARD_STARTED: {
                buf << "reward start";
                break;
            }
            case sdkbox::AdActionType::REWARD_ENDED: {
                buf << "reward end";
                
                break;
            }
            case sdkbox::AdActionType::REWARD_CANCELED: {
                buf << "reward cancel";
                break;
            }
            case sdkbox::AdActionType::AD_STARTED: {
                buf << "ad start";
                break;
            }
            case sdkbox::AdActionType::AD_CANCELED: {
                buf << "ad cancel";
                break;
            }
            case sdkbox::AdActionType::AD_ENDED: {
                buf << "ad end";
                break;
            }
            case sdkbox::AdActionType::ADACTIONTYPE_UNKNOWN: {
                buf << "unknow";
                break;
            }
            default: {
                buf << "unknow action";
                break;
            }
        }
        showMsg(buf.str());
    }

    virtual void onRewardAction( const std::string& ad_unit_id, const std::string& zone_id, float reward_amount, bool reward_succeed) override {
        std::stringstream buf;
        buf << "onRewardAction:" << ad_unit_id << ":" << zone_id << ":" << reward_amount << ":" << reward_succeed;
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

    menu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Show Placement", "arial", 24), [](Ref*) {
        std::string placementName = "placement-1";
        if (sdkbox::PluginSdkboxAds::isAvailable(placementName)) {
            sdkbox::PluginSdkboxAds::placement(placementName);
        } else {
            showMsg(placementName + " is not avaiable");
        }
    }));
    
    menu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Show AdMob Interstitial", "arial", 24), [](Ref*) {
        sdkbox::PluginSdkboxAds::playAd("AdMob", "interstitial");
        //sdkbox::PluginSdkboxAds::playAd("UnityAds", "rewardedVideo");
    }));
    
    menu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Cache AdMob Reward", "arial", 24), [](Ref*) {
        std::map<std::string, std::string> opts;
        opts.insert(std::pair<std::string, std::string>("reward", "true"));
        sdkbox::PluginSdkboxAds::cacheControl("AdMob", opts);
    }));
    
    menu->alignItemsVerticallyWithPadding(10);
    addChild(menu);
    
    sdkbox::PluginSdkboxAds::setListener(new SAListener());
    sdkbox::PluginSdkboxAds::init();
}
