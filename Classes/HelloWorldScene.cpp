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

#include "PluginAdColony/PluginAdColony.h"

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


class ACListener : public sdkbox::AdColonyListener {
public:
    /*
     * Interstitial Callback
     */
    virtual void adColonyInterstitialDidLoad(const std::string& interstitial) override {
        std::stringstream buf;
        buf << "adColonyInterstitialDidLoad" << ":" << interstitial;
        showMsg(buf.str());
    }

    virtual void adColonyInterstitialDidFailToLoad(const std::string& error) override {
        std::stringstream buf;
        buf << "adColonyInterstitialDidFailToLoad" << ":" << error;
        showMsg(buf.str());
    }

    virtual void adColonyInterstitialWillOpen(const std::string& interstitial) override {
        std::stringstream buf;
        buf << "adColonyInterstitialWillOpen" << ":" << interstitial;
        showMsg(buf.str());
   }

    virtual void adColonyInterstitialDidClose(const std::string& interstitial) override {
        std::stringstream buf;
        buf << "adColonyInterstitialDidClose" << ":" << interstitial;
        showMsg(buf.str());
   }

    virtual void adColonyInterstitialExpired(const std::string& interstitial) override {
        std::stringstream buf;
        buf << "adColonyInterstitialExpired" << ":" << interstitial;
        showMsg(buf.str());
   }

    virtual void adColonyInterstitialWillLeaveApplication(const std::string& interstitial) override {
        std::stringstream buf;
        buf << "adColonyInterstitialWillLeaveApplication" << ":" << interstitial;
        showMsg(buf.str());
   }

    virtual void adColonyInterstitialDidReceiveClick(const std::string& interstitial) override {
        std::stringstream buf;
        buf << "adColonyInterstitialDidReceiveClick" << ":" << interstitial;
        showMsg(buf.str());
   }

    virtual void adColonyInterstitialIapOpportunity(const std::string& interstitial, const std::string& iapProductID, int engagement) override {
        std::stringstream buf;
        buf << "adColonyInterstitialIapOpportunity" << ":" << interstitial;
        showMsg(buf.str());
   }

    /*
     * Banner Callback
     */
    virtual void adColonyAdViewDidLoad(const std::string& adView) override {
        std::stringstream buf;
        buf << "adColonyAdViewDidLoad" << ":" << adView;
        showMsg(buf.str());
   }

    virtual void adColonyAdViewDidFailToLoad(const std::string& error) override {
        std::stringstream buf;
        buf << "adColonyAdViewDidFailToLoad" << ":" << error;
        showMsg(buf.str());
  }

    virtual void adColonyAdViewWillLeaveApplication(const std::string& adView) override {
        std::stringstream buf;
        buf << "adColonyAdViewWillLeaveApplication" << ":" << adView;
        showMsg(buf.str());
    }
    
    virtual void adColonyAdViewWillOpen(const std::string& adView) override {
        std::stringstream buf;
        buf << "adColonyAdViewWillOpen" << ":" << adView;
        showMsg(buf.str());
    }
    
    virtual void adColonyAdViewDidClose(const std::string& adView) override {
        std::stringstream buf;
        buf << "adColonyAdViewDidClose" << ":" << adView;
        showMsg(buf.str());
    }
    
    virtual void adColonyAdViewDidReceiveClick(const std::string& adView) override {
        std::stringstream buf;
        buf << "adColonyAdViewDidReceiveClick" << ":" << adView;
        showMsg(buf.str());
    }

    /*
     * Reward Callback
     */
    virtual void adColonyReward(const std::string& name, const std::string& currencyName, int amount, bool success) override {
        std::stringstream buf;
        buf << "adColonyReward" << ":" << name << ":" << currencyName << ":" << amount << ":" << success;
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

    menu->addChild(MenuItemLabel::create(Label::createWithSystemFont("ShowAd", "arial", 24), [](Ref*){
        if (sdkbox::AdColonyAdStatus::ADCOLONY_ZONE_STATUS_ACTIVE == sdkbox::PluginAdColony::getStatus("video")) {
            sdkbox::PluginAdColony::show("video");
        } else {
            showMsg("video is not ready, to cache");
            sdkbox::PluginAdColony::requestAllAds();
        }
    }));
    menu->addChild(MenuItemLabel::create(Label::createWithSystemFont("ShowBanner", "arial", 24), [](Ref*){
        if (sdkbox::AdColonyAdStatus::ADCOLONY_ZONE_STATUS_ACTIVE == sdkbox::PluginAdColony::getStatus("banner")) {
            sdkbox::PluginAdColony::show("banner");
        } else {
            showMsg("banner is not ready, to cache");
            sdkbox::PluginAdColony::requestAllAds();
        }
    }));
    
    menu->alignItemsVerticallyWithPadding(10);
    addChild(menu);

    sdkbox::PluginAdColony::setListener(new ACListener());
    sdkbox::PluginAdColony::init();
}
