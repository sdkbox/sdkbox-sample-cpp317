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

#include "PluginAppodeal/PluginAppodeal.h"

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


class ADListener : public sdkbox::AppodealListener {
public:
    virtual void onBannerDidLoadAd();
    virtual void onBannerDidFailToLoadAd();
    virtual void onBannerDidClick();
    virtual void onBannerPresent();
    
    virtual void onInterstitialDidLoadAd();
    virtual void onInterstitialDidFailToLoadAd();
    virtual void onInterstitialWillPresent();
    virtual void onInterstitialDidDismiss();
    virtual void onInterstitialDidClick();
    
    virtual void onVideoDidLoadAd();
    virtual void onVideoDidFailToLoadAd();
    virtual void onVideoDidPresent();
    virtual void onVideoWillDismiss();
    virtual void onVideoDidFinish();
    
    virtual void onSkippableVideoDidLoadAd();
    virtual void onSkippableVideoDidFailToLoadAd();
    virtual void onSkippableVideoDidPresent();
    virtual void onSkippableVideoWillDismiss();
    virtual void onSkippableVideoDidFinish();
    virtual void onSkippableVideoDidClick();
    
    virtual void onNonSkippableVideoDidLoadAd();
    virtual void onNonSkippableVideoDidFailToLoadAd();
    virtual void onNonSkippableVideoDidPresent();
    virtual void onNonSkippableVideoWillDismiss();
    virtual void onNonSkippableVideoDidFinish();
    virtual void onNonSkippableVideoDidClick();
    
    virtual void onRewardVideoDidLoadAd();
    virtual void onRewardVideoDidFailToLoadAd();
    virtual void onRewardVideoDidPresent();
    virtual void onRewardVideoWillDismiss();
    virtual void onRewardVideoDidFinish(int amount, const std::string& name);
};

void ADListener::onBannerDidLoadAd() {
    showMsg("Listener onBannerDidLoadAd");
}
void ADListener::onBannerDidFailToLoadAd() {
    showMsg("Listener onBannerDidFailToLoadAd");
}
void ADListener::onBannerDidClick() {
    showMsg("Listener onBannerDidClick");
}
void ADListener::onBannerPresent() {
    showMsg("Listener onBannerPresent");
}

void ADListener::onInterstitialDidLoadAd() {
    showMsg("Listener onInterstitialDidLoadAd");
}
void ADListener::onInterstitialDidFailToLoadAd() {
    showMsg("Listener onInterstitialDidFailToLoadAd");
}
void ADListener::onInterstitialWillPresent() {
    showMsg("Listener onInterstitialWillPresent");
}
void ADListener::onInterstitialDidDismiss() {
    showMsg("Listener onInterstitialDidDismiss");
}
void ADListener::onInterstitialDidClick() {
    showMsg("Listener onInterstitialDidClick");
}

void ADListener::onVideoDidLoadAd() {
    showMsg("Listener onVideoDidLoadAd");
}
void ADListener::onVideoDidFailToLoadAd() {
    showMsg("Listener onVideoDidFailToLoadAd");
}
void ADListener::onVideoDidPresent() {
    showMsg("Listener onVideoDidPresent");
}
void ADListener::onVideoWillDismiss() {
    showMsg("Listener onVideoWillDismiss");
}
void ADListener::onVideoDidFinish() {
    showMsg("Listener onVideoDidFinish");
}

void ADListener::onSkippableVideoDidLoadAd() {
    showMsg("Listener onSkippableVideoDidLoadAd");
}

void ADListener::onSkippableVideoDidFailToLoadAd() {
    showMsg("Listener onSkippableVideoDidFailToLoadAd");
}

void ADListener::onSkippableVideoDidPresent() {
    showMsg("Listener onSkippableVideoDidPresent");
}

void ADListener::onSkippableVideoWillDismiss() {
    showMsg("Listener onSkippableVideoWillDismiss");
}

void ADListener::onSkippableVideoDidFinish() {
    showMsg("Listener onSkippableVideoDidFinish");
}

void ADListener::onSkippableVideoDidClick() {
    showMsg("Listener onSkippableVideoDidClick");
}


void ADListener::onNonSkippableVideoDidLoadAd() {
    showMsg("Listener onNonSkippableVideoDidLoadAd");
}

void ADListener::onNonSkippableVideoDidFailToLoadAd() {
    showMsg("Listener onNonSkippableVideoDidFailToLoadAd");
}

void ADListener::onNonSkippableVideoDidPresent() {
    showMsg("Listener onNonSkippableVideoDidPresent");
}

void ADListener::onNonSkippableVideoWillDismiss() {
    showMsg("Listener onNonSkippableVideoWillDismiss");
}

void ADListener::onNonSkippableVideoDidFinish() {
    showMsg("Listener onNonSkippableVideoDidFinish");
}

void ADListener::onNonSkippableVideoDidClick() {
    showMsg("Listener onNonSkippableVideoDidClick");
}


void ADListener::onRewardVideoDidLoadAd() {
    showMsg("Listener onRewardVideoDidLoadAd");
}
void ADListener::onRewardVideoDidFailToLoadAd() {
    showMsg("Listener onRewardVideoDidFailToLoadAd");
}
void ADListener::onRewardVideoDidPresent() {
    showMsg("Listener onRewardVideoDidPresent");
}
void ADListener::onRewardVideoWillDismiss() {
    showMsg("Listener onRewardVideoWillDismiss");
}
void ADListener::onRewardVideoDidFinish(int amount, const std::string& name) {
    std::stringstream buf;
    
    buf << "Listener onRewardVideoDidFinish:" << amount << ":" << name;
    showMsg(buf.str());
}


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

    menu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Show Bottom", "arial", 24), [](Ref*){
        if (sdkbox::PluginAppodeal::isReadyForShowWithStyle(sdkbox::PluginAppodeal::ShowStyle::AppodealShowStyleBannerBottom)) {
            sdkbox::PluginAppodeal::showAd(sdkbox::PluginAppodeal::ShowStyle::AppodealShowStyleBannerBottom);
        } else {
            showMsg("bottom is not ready");
        }
    }));
    
    menu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Show Video", "arial", 24), [](Ref*){
        if (sdkbox::PluginAppodeal::isReadyForShowWithStyle(sdkbox::PluginAppodeal::ShowStyle::AppodealShowStyleRewardedVideo)) {
            sdkbox::PluginAppodeal::showAd(sdkbox::PluginAppodeal::ShowStyle::AppodealShowStyleRewardedVideo);
        } else {
            showMsg("video is not ready");
        }
    }));
    
    menu->alignItemsVerticallyWithPadding(10);
    addChild(menu);
    
    sdkbox::PluginAppodeal::setDebugEnabled(true);
    sdkbox::PluginAppodeal::disableLocationPermissionCheck();
    sdkbox::PluginAppodeal::setAutocache(true, sdkbox::PluginAppodeal::AdType::AppodealAdTypeAll);
    sdkbox::PluginAppodeal::setSmartBannersEnabled(true);
    sdkbox::PluginAppodeal::setBannerAnimationEnabled(true);
    sdkbox::PluginAppodeal::setBannerBackgroundVisible(true);
    
    sdkbox::PluginAppodeal::setListener(new ADListener());
    sdkbox::PluginAppodeal::init();
    
    sdkbox::PluginAppodeal::setUserVkId("user id");
    sdkbox::PluginAppodeal::setUserFacebookId("facebook id");
    sdkbox::PluginAppodeal::setUserEmail("test@sdkbox.com");
    sdkbox::PluginAppodeal::setUserBirthday("11/11/1999"); //DD/MM/YYYY
    sdkbox::PluginAppodeal::setUserAge(11);
    sdkbox::PluginAppodeal::setUserGender(sdkbox::PluginAppodeal::Gender::AppodealUserGenderMale);
    sdkbox::PluginAppodeal::setUserOccupation(sdkbox::PluginAppodeal::Occupation::AppodealUserOccupationSchool);
    sdkbox::PluginAppodeal::setUserRelationship(sdkbox::PluginAppodeal::Relationship::AppodealUserRelationshipSingle);
    sdkbox::PluginAppodeal::setUserSmokingAttitude(sdkbox::PluginAppodeal::SmokingAttitude::AppodealUserSmokingAttitudeNegative);
    sdkbox::PluginAppodeal::setUserAlcoholAttitude(sdkbox::PluginAppodeal::AlcoholAttitude::AppodealUserAlcoholAttitudeNegative);
    sdkbox::PluginAppodeal::setUserInterests("game");
    
    sdkbox::PluginAppodeal::cacheAd(sdkbox::PluginAppodeal::AdType::AppodealAdTypeAll);
    
    std::stringstream buf;
    buf << "Appodeal Version:" << sdkbox::PluginAppodeal::getSDKVersion();
    showMsg(buf.str());
}
