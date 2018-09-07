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

#include "PluginTune/PluginTune.h"

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


class TListener : public sdkbox::TuneListener {
public:
    
    virtual void onMobileAppTrackerEnqueuedActionWithReferenceId(const std::string &referenceId) override {
        showMsg("onMobileAppTrackerEnqueuedActionWithReferenceId:" + referenceId);
    }

    virtual void onMobileAppTrackerDidSucceedWithData(const std::string &data) override {
        showMsg("onMobileAppTrackerDidSucceedWithData:" + data);
    }

    virtual void onMobileAppTrackerDidFailWithError(const std::string &errorString) override {
        showMsg("onMobileAppTrackerDidFailWithError");
        cocos2d::log("Log::%s", errorString.c_str());
    }

    virtual void onMobileAppTrackerDidReceiveDeeplink(const std::string &deeplink, bool timeout) override {
        showMsg("onMobileAppTrackerDidReceiveDeeplink:" + deeplink + ":" + (timeout ? "true" : "false"));
    }

    virtual void onMobileAppTrackerDidFailDeeplinkWithError(const std::string &errorString) override {
        showMsg("onMobileAppTrackerDidFailDeeplinkWithError");
        cocos2d::log("Log::%s", errorString.c_str());
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

    menu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Measure Event", "arial", 24), [](Ref*){
        // https://developers.mobileapptracking.com/event-function-templates/
        {
            sdkbox::PluginTune::measureEventId(1122334455);
            sdkbox::PluginTune::measureEventName("measureEventName-purchase");
            
            sdkbox::TuneEvent event;
            event.eventName = "TuneEvent-2016-02-02";
            event.refId     = "RJ1357";
            event.searchString = "sweet srisp red apples";
            event.attribute1 = "srisp";
            event.attribute2 = "red";
            event.quantity = 3;
            sdkbox::PluginTune::measureEvent(event);
        }
        
        {
            sdkbox::TuneEventItem item;
            item.item = "apple";
            item.unitPrice = 0.99;
            item.quantity = 2;
            item.revenue = 1.98;
            
            std::vector<sdkbox::TuneEventItem> eventItems(1);
            eventItems.push_back(item);
            
            sdkbox::TuneEvent event = sdkbox::TuneEvent();
            event.eventName = "Purchase0211";
            event.revenue = 2.98;
            event.eventItems = eventItems;
            
            sdkbox::PluginTune::measureEvent(event);
            
        }
        
        // https://developers.mobileapptracking.com/settings-for-pre-loaded-apps/
        {
            sdkbox::TunePreloadData pd;
            pd.publisherId = "112233";
            pd.offerId = "offer_id";
            pd.agencyId = "agency_id";
            pd.publisherReferenceId = "publisher_ref_id";
            pd.publisherSub1 = "pub_sub1";
            pd.publisherSub2 = "pub_sub2";
            pd.publisherSub3 = "pub_sub3";
            pd.publisherSub4 = "pub_sub4";
            pd.publisherSub5 = "pub_sub5";
            pd.publisherSubAd = "pub_sub_ad";
            pd.publisherSubAdgroup = "pub_sub_adgroup";
            pd.publisherSubCampaign = "pub_sub_campaign";
            pd.publisherSubKeyword = "pub_sub_keyword";
            pd.publisherSubPublisher = "pub_sub_publisher";
            pd.publisherSubSite = "pub_sub_site";
            pd.advertiserSubAd = "ad_sub_ad";
            pd.advertiserSubAdgroup = "ad_sub_adgroup";
            pd.advertiserSubCampaign = "ad_sub_campaign";
            pd.advertiserSubKeyword = "ad_sub_keyword";
            pd.advertiserSubPublisher = "ad_sub_publisher";
            pd.advertiserSubSite = "ad_sub_site";
            
            sdkbox::PluginTune::setPreloadData(pd);
            sdkbox::PluginTune::measureSession();
        }
        
        sdkbox::PluginTune::setDeepLink("fb://profile/33138223345");
        sdkbox::PluginTune::checkForDeferredDeepLink();

    }));
    
    menu->alignItemsVerticallyWithPadding(10);
    addChild(menu);

    sdkbox::PluginTune::setListener(new TListener());
    sdkbox::PluginTune::init();
    sdkbox::PluginTune::measureSession();

    sdkbox::PluginTune::setDebugMode(false);
    sdkbox::PluginTune::setDebugMode(true);
    sdkbox::PluginTune::setAllowDuplicateRequests(true);
    sdkbox::PluginTune::automateIapEventMeasurement(true);
    sdkbox::PluginTune::setCurrencyCode("RMB");
    sdkbox::PluginTune::setUserEmail("natalie@somedomain.com");
    sdkbox::PluginTune::setUserName("natalie123");
    sdkbox::PluginTune::setAge(43);
    sdkbox::PluginTune::setGender(sdkbox::PluginTune::GenderFemale);
    sdkbox::PluginTune::setUserId("US13579");
    sdkbox::PluginTune::setFacebookUserId("321321321321");
    sdkbox::PluginTune::setGoogleUserId("11223344556677");
    sdkbox::PluginTune::setTwitterUserId("1357924680");
    sdkbox::PluginTune::setLatitude(9.142276, -79.724052, 15);
    sdkbox::PluginTune::setAppAdTracking(true);
    sdkbox::PluginTune::measureEventName("login");

}
