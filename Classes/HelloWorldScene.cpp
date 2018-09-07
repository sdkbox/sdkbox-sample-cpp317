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

#include "PluginFyber/PluginFyber.h"

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


class FBListener : public sdkbox::FyberListener {
public:
    virtual void onVirtualCurrencyConnectorFailed(int error,                    // deprecated filed, always: 0
                                                  const std::string& errorCode, // deprecated filed, alwasy: ""
                                                  const std::string& errorMsg) override {
        std::stringstream buf;
        
        buf << "onVirtualCurrencyConnectorFailed:" << error << ":" << errorCode << ":" << errorMsg;
        showMsg(buf.str());
    }

    virtual void onVirtualCurrencyConnectorSuccess(double deltaOfCoins,
                                                   const std::string& currencyId,
                                                   const std::string& currencyName,
                                                   const std::string& transactionId) override {
        std::stringstream buf;

        buf << "onVirtualCurrencyConnectorSuccess:" << deltaOfCoins << ":" << currencyId << ":" << currencyName << ":" << transactionId;
        showMsg(buf.str());
    }

    virtual void onCanShowInterstitial(bool canShowInterstitial) override {
        std::stringstream buf;
        
        buf << "onCanShowInterstitial:" << canShowInterstitial;
        showMsg(buf.str());
        
        if (canShowInterstitial) {
            sdkbox::PluginFyber::showInterstitial();
        }
    }

    virtual void onInterstitialDidShow() override {
        std::stringstream buf;
        
        buf << "onInterstitialDidShow";
        showMsg(buf.str());
    }

    virtual void onInterstitialDismiss(const std::string& reason) override {
        std::stringstream buf;
        
        buf << "onInterstitialDismiss:" << reason;
        showMsg(buf.str());
    }

    virtual void onInterstitialFailed() override {
        std::stringstream buf;
        
        buf << "onInterstitialFailed";
        showMsg(buf.str());
    }
    
    virtual void onBrandEngageClientReceiveOffers(bool areOffersAvailable) override {
        std::stringstream buf;
        
        buf << "onBrandEngageClientReceiveOffers:" << areOffersAvailable;
        showMsg(buf.str());
        if (areOffersAvailable) {
            sdkbox::PluginFyber::showOffers();
        }
    }

    virtual void onBrandEngageClientChangeStatus(int status, const std::string& msg) override {
        std::stringstream buf;
        
        buf << "onBrandEngageClientChangeStatus:" << status << ":" << msg;
        showMsg(buf.str());
    }

    virtual void onOfferWallFinish(int status) override {
        std::stringstream buf;
        
        buf << "onOfferWallFinish:" << status;
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

    menu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Request Interstitial Then Show", "arial", 24), [](Ref*){
        sdkbox::PluginFyber::requestInterstitial();
//        sdkbox::PluginFyber::showInterstitial();
    }));
    menu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Request Video Then Show", "arial", 24), [](Ref*){
        sdkbox::PluginFyber::requestOffers();
//        sdkbox::PluginFyber::showOffers();
    }));
    menu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Show OfferWall", "arial", 24), [](Ref*){
        sdkbox::PluginFyber::showOfferWall();
    }));

    menu->alignItemsVerticallyWithPadding(10);
    addChild(menu);

    sdkbox::PluginFyber::setListener(new FBListener());
    sdkbox::PluginFyber::init("123");

    sdkbox::PluginFyber::setAge(4);
    sdkbox::PluginFyber::setBirthdate("2000-01-02");
    sdkbox::PluginFyber::setGender(sdkbox::FYB_UserGenderMale);
    sdkbox::PluginFyber::setSexualOrientation(sdkbox::FYB_UserSexualOrientationGay);
    sdkbox::PluginFyber::setEthnicity(sdkbox::FYB_UserEthnicityAsian);
    sdkbox::PluginFyber::setLocation(30.67, 104.06);
    sdkbox::PluginFyber::cleanLocation();
    sdkbox::PluginFyber::setMaritalStatus(sdkbox::FYB_UserMartialStatusSingle);
    sdkbox::PluginFyber::setNumberOfChildren(3);
    sdkbox::PluginFyber::setAnnualHouseholdIncome(5);
    sdkbox::PluginFyber::setEducation(sdkbox::FYB_UserEducationMasters);
    sdkbox::PluginFyber::setZipcode("10086");
    sdkbox::PluginFyber::setInterests({"sport", "game"});
    sdkbox::PluginFyber::setIap(true);
    sdkbox::PluginFyber::setIapAmount(10);
    sdkbox::PluginFyber::setNumberOfSessions(2);
    sdkbox::PluginFyber::setPsTime(100);
    sdkbox::PluginFyber::setLastSession(1423958400);
    sdkbox::PluginFyber::setConnectionType(sdkbox::FYB_UserConnectionType3G);
    sdkbox::PluginFyber::setDevice(sdkbox::FYB_UserDeviceIPhone);
    sdkbox::PluginFyber::setVersion("lion");
    sdkbox::PluginFyber::cleanCustomParameters();
    sdkbox::PluginFyber::addCustomParameters("int", "123");
    sdkbox::PluginFyber::addCustomParameters("float", "123.4");
    sdkbox::PluginFyber::addCustomParameters("boolean", "true");
    sdkbox::PluginFyber::addCustomParameters("daterange", "1423958400"); // 15/02/2015
    sdkbox::PluginFyber::addCustomParameters("string", "foobar");

}
