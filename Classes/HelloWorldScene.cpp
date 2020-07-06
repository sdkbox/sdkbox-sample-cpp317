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
#include "PluginHMS/PluginHMS.h"
#include "json/rapidjson.h"
#include "json/document.h"

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
        label = Label::createWithSystemFont("test", "arial", 12);
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

class PluginHMSListener : public sdkbox::HMSListener {
public:
    PluginHMSListener(HelloWorld* scene): hw(scene) {
    }

    void onLogin(int code, const std::string& errorOrJson) {
        cocos2d::log("HMS onLogin: %d, %s", code, errorOrJson.c_str());
        if (0 != code) {
            showMsg("login failed:" + errorOrJson);
            return;
        }
        rapidjson::Document doc;
        doc.Parse(errorOrJson.c_str());
        
        std::ostringstream ss;
        ss << "Login success:" << doc["displayName"].GetString();
        showMsg(ss.str());
    }

    void onPlayerInfo(int code, const std::string& errorOrJson) {
        std::ostringstream ss;
        ss << "HMS listener onPlayerInfo code:" << code;
        showMsg(ss.str());
        cocos2d::log("%s", errorOrJson.c_str());
    };

    void onPlayerExtraInfo(int code, const std::string& errorOrJson) {
        std::ostringstream ss;
        ss << "HMS listener onPlayerExtraInfo code:" << code;
        showMsg(ss.str());
        cocos2d::log("%s", errorOrJson.c_str());
    };

    void onPlayerGameBegin(int code, const std::string& errorOrJson) {
        std::ostringstream ss;
        ss << "HMS listener onPlayerGameBegin code:" << code;
        showMsg(ss.str());
        cocos2d::log("%s", errorOrJson.c_str());
    };

    void onPlayerGameEnd(int code, const std::string& errorOrJson) {
        std::ostringstream ss;
        ss << "HMS listener onPlayerGameEnd code:" << code;
        showMsg(ss.str());
        cocos2d::log("%s", errorOrJson.c_str());
    };

    void onIAPReady(int code, const std::string& errorOrJson) {
        std::ostringstream ss;
        ss << "HMS listener onIAPReady code:" << code;
        showMsg(ss.str());
        cocos2d::log("%s", errorOrJson.c_str());
    }

    void onIAPProducts(int code, const std::string& errorOrJson) {
        std::ostringstream ss;
        ss << "HMS listener onIAPProducts code:" << code;
        showMsg(ss.str());
        cocos2d::log("%s", errorOrJson.c_str());
        
        if (0 != code) {
            return;
        }
        rapidjson::Document doc;
        doc.Parse(errorOrJson.c_str());
        if (!doc.HasMember("products")) {
            return;
        }
        const rapidjson::Value& products = doc["products"];
        if (!products.IsArray()) {
            return;
        }
        
        ss.str("");
        ss << "Products ID:";
        for (rapidjson::SizeType i = 0; i < products.Size(); i++) {
            const rapidjson::Value &p = products[i];
            ss << p["productId"].GetString() << " | ";

            HMSProduct hmsProd;
            hmsProd.id = p["productId"].GetString();
            hmsProd.type = p["type"].GetString();
            
            hw->addHMSProduct(hmsProd);
        }
        showMsg(ss.str());
    }

    void onIAPPurchase(int code, const std::string& errorOrJson) {
        std::ostringstream ss;
        ss << "HMS listener onIAPPurchase code:" << code;
        showMsg(ss.str());
        cocos2d::log("%s", errorOrJson.c_str());
        
        if (0 != code) {
            return;
        }
        rapidjson::Document doc;
        doc.Parse(errorOrJson.c_str());
        if (!doc.HasMember("inAppPurchaseData")
            || !doc.HasMember("inAppDataSignature")) {
            return;
        }
        const std::string iapData = doc["inAppPurchaseData"].GetString();
        const std::string iapSignature = doc["inAppDataSignature"].GetString();

        rapidjson::Document iapDoc;
        iapDoc.Parse(iapData.c_str());
        
        const std::string pToken = iapDoc["purchaseToken"].GetString();
        const std::string pId = iapDoc["productId"].GetString();
        
        ss.str("");
        ss << "Purchase ID:PurchaseToken" << " => " << pId << ":" << pToken;
        showMsg(ss.str());

        if (hw->isConsumable(pId)) {
            hw->addPurchaseToken(pToken);
        } else {
            // purchase success delivery product to player
            ss.str("");
            ss << "Purchase Success, delivery" << pId << " to player";
            showMsg(ss.str());
        }
    }

    void onIAPConsume(int code, const std::string& errorOrJson) {
        std::ostringstream ss;
        ss << "HMS listener onIAPConsume code:" << code;
        showMsg(ss.str());
        cocos2d::log("%s", errorOrJson.c_str());
        if (0 == code) {
            // consume success, delivery this consumable to player
            ss.str("");
            ss << "Consume Success, delivery to player";
            showMsg(ss.str());
        }
    }

    void onIAPOwnedPurchases(int code, const std::string& errorOrJson) {
        std::ostringstream ss;
        ss << "HMS listener onIAPOwnedPurchases code:" << code;
        showMsg(ss.str());
        cocos2d::log("%s", errorOrJson.c_str());

        if (0 != code) {
            return;
        }
        rapidjson::Document doc;
        doc.Parse(errorOrJson.c_str());
        if (!doc.HasMember("ownedPurchases")) {
            return;
        }
        const rapidjson::Value& purchases = doc["ownedPurchases"];
        if (!purchases.IsArray()) {
            return;
        }

        ss.str("");
        ss << "Owned Products ID:";
        for (rapidjson::SizeType i = 0; i < purchases.Size(); i++) {
            const rapidjson::Value &p = purchases[i];

            rapidjson::Document purchaseData;
            purchaseData.Parse(p["inAppPurchaseData"].GetString());
            std::string pId = purchaseData["productId"].GetString();
            std::string pToken = purchaseData["purchaseToken"].GetString();
            ss << pId << " | ";

            if (hw->isConsumable(pId)) {
                // consumable product have not consume
                hw->addPurchaseToken(pToken);
            }
        }
        showMsg(ss.str());
    }

    void onIAPOwnedPurchaseRecords(int code, const std::string& errorOrJson) {
        std::ostringstream ss;
        ss << "HMS listener onIAPOwnedPurchaseRecords code:" << code;
        showMsg(ss.str());
        cocos2d::log("%s", errorOrJson.c_str());
    }


    void onAchievementList(int code, const std::string& errorOrJson) {
        std::ostringstream ss;
        ss << "HMS listener onAchievementList code:" << code;
        showMsg(ss.str());
        cocos2d::log("%s", errorOrJson.c_str());
    }

    void onAchievementShow(int code, const std::string& errorOrJson) {
        std::ostringstream ss;
        ss << "HMS listener onAchievementShow code:" << code;
        showMsg(ss.str());
        cocos2d::log("%s", errorOrJson.c_str());
    }

    void onAchievementVisualize(int code, const std::string& errorOrJson) {
        std::ostringstream ss;
        ss << "HMS listener onAchievementVisualize code:" << code;
        showMsg(ss.str());
        cocos2d::log("%s", errorOrJson.c_str());
    }

    void onAchievementGrow(int code, const std::string& errorOrJson) {
        std::ostringstream ss;
        ss << "HMS listener onAchievementGrow code:" << code;
        showMsg(ss.str());
        cocos2d::log("%s", errorOrJson.c_str());
    }

    void onAchievementMakeSteps(int code, const std::string& errorOrJson) {
        std::ostringstream ss;
        ss << "HMS listener onAchievementMakeSteps code:" << code;
        showMsg(ss.str());
        cocos2d::log("%s", errorOrJson.c_str());
    }

    void onAchievementReach(int code, const std::string& errorOrJson) {
        std::ostringstream ss;
        ss << "HMS listener onAchievementReach code:" << code;
        showMsg(ss.str());
        cocos2d::log("%s", errorOrJson.c_str());
    }

    void onEventList(int code, const std::string& errorOrJson) {
        std::ostringstream ss;
        ss << "HMS listener onEventList code:" << code;
        showMsg(ss.str());
        cocos2d::log("%s", errorOrJson.c_str());
    }

    void onRankingSwitchStatus(int code, const std::string& errorOrJson) {
        std::ostringstream ss;
        ss << "HMS listener onRankingSwitchStatus code:" << code;
        showMsg(ss.str());
        cocos2d::log("%s", errorOrJson.c_str());
    }

    void onRankingSetSwitchStatus(int code, const std::string& errorOrJson) {
        std::ostringstream ss;
        ss << "HMS listener onRankingSetSwitchStatus code:" << code;
        showMsg(ss.str());
        cocos2d::log("%s", errorOrJson.c_str());
    }

    void onRankingSubmitScore(int code, const std::string& errorOrJson) {
        std::ostringstream ss;
        ss << "HMS listener onRankingSubmitScore code:" << code;
        showMsg(ss.str());
        cocos2d::log("%s", errorOrJson.c_str());
    }

    void onRankingShow(int code, const std::string& errorOrJson) {
        std::ostringstream ss;
        ss << "HMS listener onRankingShow code:" << code;
        showMsg(ss.str());
        cocos2d::log("%s", errorOrJson.c_str());
    }

    void onRankingList(int code, const std::string& errorOrJson) {
        std::ostringstream ss;
        ss << "HMS listener onRankingList code:" << code;
        showMsg(ss.str());
        cocos2d::log("%s", errorOrJson.c_str());
    }

    void onRankingCurPlayerScore(int code, const std::string& errorOrJson) {
        std::ostringstream ss;
        ss << "HMS listener onRankingCurPlayerScore code:" << code;
        showMsg(ss.str());
        cocos2d::log("%s", errorOrJson.c_str());
    }

    void onRankingPlayerCenteredScores(int code, const std::string& errorOrJson) {
        std::ostringstream ss;
        ss << "HMS listener onRankingPlayerCenteredScores code:" << code;
        showMsg(ss.str());
        cocos2d::log("%s", errorOrJson.c_str());
    }

    void onRankingMoreScores(int code, const std::string& errorOrJson) {
        std::ostringstream ss;
        ss << "HMS listener onRankingMoreScores code:" << code;
        showMsg(ss.str());
        cocos2d::log("%s", errorOrJson.c_str());
    }

    void onRankingTopScores(int code, const std::string& errorOrJson) {
        std::ostringstream ss;
        ss << "HMS listener onRankingTopScores code:" << code;
        showMsg(ss.str());
        cocos2d::log("%s", errorOrJson.c_str());
    }

    void onArchiveLimitThumbnailSize(int code, const std::string& errorOrJson) {
        std::ostringstream ss;
        ss << "HMS listener onArchiveLimitThumbnailSize code:" << code;
        showMsg(ss.str());
        cocos2d::log("%s", errorOrJson.c_str());
    }

    void onArchiveLimitDetailsSize(int code, const std::string& errorOrJson) {
        std::ostringstream ss;
        ss << "HMS listener onArchiveLimitDetailsSize code:" << code;
        showMsg(ss.str());
        cocos2d::log("%s", errorOrJson.c_str());
    }

    void onArchiveAdd(int code, const std::string& errorOrJson) {
        std::ostringstream ss;
        ss << "HMS listener onArchiveAdd code:" << code;
        showMsg(ss.str());
        cocos2d::log("%s", errorOrJson.c_str());
    }

    void onArchiveShow(int code, const std::string& errorOrJson) {
        std::ostringstream ss;
        ss << "HMS listener onArchiveShow code:" << code;
        showMsg(ss.str());
        cocos2d::log("%s", errorOrJson.c_str());
    }

    void onArchiveSummaryList(int code, const std::string& errorOrJson) {
        std::ostringstream ss;
        ss << "HMS listener onArchiveSummaryList code:" << code;
        showMsg(ss.str());
        cocos2d::log("%s", errorOrJson.c_str());

        if (0 != code) {
            return;
        }
        if (0 == errorOrJson.length()) {
            return;
        }
        rapidjson::Document doc;
        doc.Parse(errorOrJson.c_str());
        
        if (!doc.HasMember("archiveSummarys")) {
            return;
        }

        // random select archive id
        const auto& ass = doc["archiveSummarys"];
        std::string aid = "";
        int asSize = ass.Size();
        if (asSize > 0) {
            const auto& as = ass[cocos2d::random(0, asSize - 1)];
            aid = as["id"].GetString();
        }
        if (aid.length() > 0) {
            hw->setArchiveId(aid);
        }
    }

    void onArchiveSelect(int code, const std::string& errorOrJson) {
        std::ostringstream ss;
        ss << "HMS listener onArchiveSelect code:" << code;
        showMsg(ss.str());
        cocos2d::log("%s", errorOrJson.c_str());
    }

    void onArchiveThumbnail(int code, const std::string& errorOrJson, unsigned char* coverData, unsigned int coverDataLen) {
        std::ostringstream ss;
        ss << "HMS listener onArchiveThumbnail code:" << code << " len:" << coverDataLen;
        showMsg(ss.str());
        cocos2d::log("%s", errorOrJson.c_str());
    }

    void onArchiveUpdate(int code, const std::string& errorOrJson) {
        std::ostringstream ss;
        ss << "HMS listener onArchiveUpdate code:" << code;
        showMsg(ss.str());
        cocos2d::log("%s", errorOrJson.c_str());
    }

    void onArchiveLoad(int code, const std::string& errorOrJson, unsigned char* contentData, unsigned int contentDataLen) {
        std::ostringstream ss;
        ss << "HMS listener onArchiveLoad code:" << code << " len:" << contentDataLen;
        showMsg(ss.str());
        cocos2d::log("%s", errorOrJson.c_str());
    }

    void onArchiveRemove(int code, const std::string& errorOrJson) {
        std::ostringstream ss;
        ss << "HMS listener onArchiveRemove code:" << code;
        showMsg(ss.str());
        cocos2d::log("%s", errorOrJson.c_str());
    }

    void onGamePlayerStats(int code, const std::string& errorOrJson) {
        std::ostringstream ss;
        ss << "HMS listener onGamePlayerStats code:" << code;
        showMsg(ss.str());
        cocos2d::log("%s", errorOrJson.c_str());
    }

    void onGameSummary(int code, const std::string& errorOrJson) {
        std::ostringstream ss;
        ss << "HMS listener onGameSummary code:" << code;
        showMsg(ss.str());
        cocos2d::log("%s", errorOrJson.c_str());
    }

    void onAdClose(int code, const std::string& errorOrJson) {
        std::ostringstream ss;
        ss << "HMS listener onAdClose code:" << code;
        showMsg(ss.str());
        cocos2d::log("%s", errorOrJson.c_str());
    }

    void onAdFail(int code, const std::string& errorOrJson) {
        std::ostringstream ss;
        ss << "HMS listener onAdFail code:" << code;
        showMsg(ss.str());
        cocos2d::log("%s", errorOrJson.c_str());
    }

    void onAdLeave(int code, const std::string& errorOrJson) {
        std::ostringstream ss;
        ss << "HMS listener onAdLeave code:" << code;
        showMsg(ss.str());
        cocos2d::log("%s", errorOrJson.c_str());
    }

    void onAdOpen(int code, const std::string& errorOrJson) {
        std::ostringstream ss;
        ss << "HMS listener onAdOpen code:" << code;
        showMsg(ss.str());
        cocos2d::log("%s", errorOrJson.c_str());
    }

    void onAdLoad(int code, const std::string& errorOrJson) {
        std::ostringstream ss;
        ss << "HMS listener onAdLoad code:" << code;
        showMsg(ss.str());
        cocos2d::log("%s", errorOrJson.c_str());
    }

    void onAdClick(int code, const std::string& errorOrJson) {
        std::ostringstream ss;
        ss << "HMS listener onAdClick code:" << code;
        showMsg(ss.str());
        cocos2d::log("%s", errorOrJson.c_str());
    }

    void onAdImpression(int code, const std::string& errorOrJson) {
        std::ostringstream ss;
        ss << "HMS listener onAdImpression code:" << code;
        showMsg(ss.str());
        cocos2d::log("%s", errorOrJson.c_str());
    }

    void onAdReward(int code, const std::string& errorOrJson) {
        std::ostringstream ss;
        ss << "HMS listener onAdReward code:" << code;
        showMsg(ss.str());
        cocos2d::log("%s", errorOrJson.c_str());
    }


private:
    HelloWorld* hw;

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
    cocos2d::Size vSize = cocos2d::Director::getInstance()->getVisibleSize();
    mTitle = Label::createWithSystemFont("Main", "arial", 24);
    mTitle->setPosition(cocos2d::Vec2(vSize.width/2, vSize.height - 100));
    addChild(mTitle);


    this->products.clear();
    this->consumablePurchaseToken.clear();
    this->mAdList.push_back("banner");
    this->mAdList.push_back("interstitial");
    this->mAdList.push_back("rewarded-video");
    this->mAdName = this->mAdList.front();
    this->mAdList.pop_front();

    this->readImageForArchiveCover();

    mMenu = Menu::create();
    addChild(mMenu);
    showMenu("");

    sdkbox::PluginHMS::setListener(new PluginHMSListener(this));
    sdkbox::PluginHMS::init();
    sdkbox::PluginHMS::login(0); // slient sigin
    sdkbox::PluginHMS::buoyShow();


    sdkbox::PluginHMS::adSetRewardData("cdata");
    sdkbox::PluginHMS::adSetRewardUserId("uid666");

    /*
     * adContentClassification:
     *   "W"->Content suitable for toddlers and older audiences;
     *  "PI"->Content suitable for kids and older audiences
     *   "J"->Content suitable for teenagers and older audiences.
     *   "A"->Content suitable only for adults.
     *    ""->Unknown rating.
     */
    sdkbox::PluginHMS::adSetAdContentClassification("A");

    /*
     * tagForUnderAgeOfPromise:
     *  0->Do not process ad requests as directed to users under the age of consent;
     *  1->Process ad requests as directed to users under the age of consent;
     * -1->Whether to process ad requests as directed to users under the age of consent is not specified;
     */
    sdkbox::PluginHMS::adSetTagForUnderAgeOfPromise(0);

    /*
    * tagForChildProtection:
    *  0->Do not process ad requests according to the COPPA;
    *  1->Process ad requests according to the COPPA;
    * -1->Whether to process ad requests according to the COPPA is not specified;
    */
    sdkbox::PluginHMS::adSetTagForChildProtection(0);

    /*
    * nonPersonalizedAd
    *  0->Request both personalized and non-personalized ads (default);
    *  1->Request only non-personalized ads;
    */
    sdkbox::PluginHMS::adSetNonPersonalizedAd(0);

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

void HelloWorld::showMenu(const std::string& menuName) {
    mMenu->removeAllChildrenWithCleanup(true);
    std::string title = menuName;
    if (title.empty()) {
        title = "Main";
    }
    mTitle->setString(title);
    if (0 == title.compare("Main")) {
        genMainMenu();
    } else if (0 == title.compare("Account")) {
        genAccountMenu();
    } else if (0 == title.compare("IAP")) {
        genIAPMenu();
    } else if (0 == title.compare("Game Player")) {
        genGamePlayerMenu();
    } else if (0 == title.compare("Achievement")) {
        genGameAchievementMenu();
    } else if (0 == title.compare("Event")) {
        genGameEventMenu();
    } else if (0 == title.compare("Ranking")) {
        genGameRankingMenu();
    } else if (0 == title.compare("Archive")) {
        genGameArchiveMenu();
    } else if (0 == title.compare("Status")) {
        genGameStatsMenu();
    } else if (0 == title.compare("Ad")) {
        genAdMenu();
    } else {
        CCLOG("Error, Unknow menu type");
    }
    mMenu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Back", "arial", 24), [this](Ref*){
        this->showMenu("");
    }));
    mMenu->alignItemsVerticallyWithPadding(10);
}

void HelloWorld::genMainMenu() {
    mMenu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Account Test", "arial", 24), [this](Ref*){
        this->showMenu("Account");
    }));
    mMenu->addChild(MenuItemLabel::create(Label::createWithSystemFont("IAP Test", "arial", 24), [this](Ref*){
        this->showMenu("IAP");
    }));
    mMenu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Game Player Test", "arial", 24), [this](Ref*){
        this->showMenu("Game Player");
    }));
    mMenu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Game Achievement Test", "arial", 24), [this](Ref*){
        this->showMenu("Achievement");
    }));
    mMenu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Game Event Test", "arial", 24), [this](Ref*){
        this->showMenu("Event");
    }));
    mMenu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Game Ranking Test", "arial", 24), [this](Ref*){
        this->showMenu("Ranking");
    }));
    mMenu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Game Archive Test", "arial", 24), [this](Ref*){
        this->showMenu("Archive");
    }));
    mMenu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Game Status Test", "arial", 24), [this](Ref*){
        this->showMenu("Status");
    }));
    mMenu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Ad Test", "arial", 24), [this](Ref*){
        this->showMenu("Ad");
    }));
}

void HelloWorld::genAccountMenu() {
    mMenu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Login", "arial", 24), [](Ref*){
        showMsg("to login...");
        // sdkbox::PluginHMS::login(0); // slient login
        sdkbox::PluginHMS::login(1); // login (id token)
        // sdkbox::PluginHMS::login(2); // login (author code)
    }));
    mMenu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Logout", "arial", 24), [](Ref*){
        showMsg("to logout...");
        sdkbox::PluginHMS::logout();
    }));
}

void HelloWorld::genIAPMenu() {
    mMenu->addChild(MenuItemLabel::create(Label::createWithSystemFont("RequestProducts", "arial", 24), [](Ref*){
        showMsg("to RequestProducts...");
        sdkbox::PluginHMS::iapRequestProducts();
    }));
    mMenu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Purchase", "arial", 24), [](Ref*){
        showMsg("to Purchase...");
        std::string productName = "coin";
//        productName = "remove_ads";
//        productName = "vip";
        sdkbox::PluginHMS::iapPurchase("coin");
    }));
    mMenu->addChild(MenuItemLabel::create(Label::createWithSystemFont("PurchaseWithPrice", "arial", 24), [](Ref*){
        showMsg("to PurchaseWithPrice...");

        std::string productInfo = "{\"currency\":\"CNY\",\"priceType\":0,\"sdkChannel\":\"1\",\"productName\":\"test\",\"amount\":\"1.99\",\"productId\":\"consumeproduct1\",\"serviceCatalog\":\"X38\",\"country\":\"CN\",\"reservedInfor\":\"{\\\"a\\\": 1, \\\"b\\\":\\\"s\\\"}\",\"developerPayload\":\"payload1\"}";
        sdkbox::PluginHMS::iapPurchaseWithPrice(productInfo);
    }));
    mMenu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Consume", "arial", 24), [this](Ref*){
        if (0 == this->consumablePurchaseToken.size()) {
            return ;
        }
        showMsg("to Consume...");
        const std::string purchaseToken = this->consumablePurchaseToken.front();
        this->consumablePurchaseToken.pop_front();
        sdkbox::PluginHMS::iapConsume(purchaseToken);
    }));
    mMenu->addChild(MenuItemLabel::create(Label::createWithSystemFont("RequestOwnedPurchase", "arial", 24), [](Ref*){
        showMsg("to RequestOwnedPurchase...");
        sdkbox::PluginHMS::iapRequestOwnedPurchases();
    }));
    mMenu->addChild(MenuItemLabel::create(Label::createWithSystemFont("RequestOwnedPurchaseRecord", "arial", 24), [](Ref*){
        showMsg("to RequestOwnedPurchaseRecord...");
        sdkbox::PluginHMS::iapRequestOwnedPurchaseRecords();
    }));
}

void HelloWorld::genGamePlayerMenu() {
    mMenu->addChild(MenuItemLabel::create(Label::createWithSystemFont("ReqPlayInfo", "arial", 24), [](Ref*){
        showMsg("to request player info...");
        sdkbox::PluginHMS::playerRequestInfo();
    }));
    mMenu->addChild(MenuItemLabel::create(Label::createWithSystemFont("ReqPlayExtraInfo", "arial", 24), [](Ref*){
        showMsg("to request player extra info...");
        sdkbox::PluginHMS::playerRequestExtraInfo();
    }));
    mMenu->addChild(MenuItemLabel::create(Label::createWithSystemFont("SubmitGameBegin", "arial", 24), [](Ref*){
        showMsg("to submit player game begin...");
        sdkbox::PluginHMS::playerSubmitGameBegin();
    }));
    mMenu->addChild(MenuItemLabel::create(Label::createWithSystemFont("SubmitGameEnd", "arial", 24), [](Ref*){
        showMsg("to submit player game end...");
        sdkbox::PluginHMS::playerSubmitGameEnd();
    }));
}

void HelloWorld::genGameAchievementMenu() {
    mMenu->addChild(MenuItemLabel::create(Label::createWithSystemFont("ReqAchievementList", "arial", 24), [](Ref*){
        showMsg("to achievementRequestList...");
        sdkbox::PluginHMS::achievementRequestList();
    }));
    mMenu->addChild(MenuItemLabel::create(Label::createWithSystemFont("ShowAchievementList", "arial", 24), [](Ref*){
        showMsg("to achievementShow...");
        sdkbox::PluginHMS::achievementShow();
    }));
    mMenu->addChild(MenuItemLabel::create(Label::createWithSystemFont("VisualizeAchievement", "arial", 24), [](Ref*){
        showMsg("to achievementVisualize...");
        sdkbox::PluginHMS::achievementVisualize("5shoot");
    }));
    mMenu->addChild(MenuItemLabel::create(Label::createWithSystemFont("GrowAchievement", "arial", 24), [](Ref*){
        showMsg("to achievementGrow...");
        sdkbox::PluginHMS::achievementGrow("3shoot", 1);
    }));
    mMenu->addChild(MenuItemLabel::create(Label::createWithSystemFont("MakeStepsAchievement", "arial", 24), [](Ref*){
        showMsg("to achievementMakeSteps...");
        sdkbox::PluginHMS::achievementMakeSteps("3shoot", 2);
    }));
    mMenu->addChild(MenuItemLabel::create(Label::createWithSystemFont("ReachAchievement", "arial", 24), [](Ref*){
        showMsg("to achievementReach...");
        sdkbox::PluginHMS::achievementReach("freshman");
    }));
}

void HelloWorld::genGameEventMenu() {
    mMenu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Grow Event", "arial", 24), [](Ref*){
        showMsg("to eventGrow...");
        sdkbox::PluginHMS::eventGrow("gencoin", 1);
    }));
    mMenu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Event List", "arial", 24), [](Ref*){
        showMsg("to eventRequestList...");
        sdkbox::PluginHMS::eventRequestList(true, "gencoin,consumecoin");
    }));
}

void HelloWorld::genGameRankingMenu() {
    mMenu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Request Switch Status", "arial", 24), [](Ref*){
        showMsg("to rankingRequestSwitchStatus...");
        sdkbox::PluginHMS::rankingRequestSwitchStatus();
    }));
    mMenu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Set Switch Status", "arial", 24), [](Ref*){
        showMsg("to rankingSetSwitchStatus...");
        sdkbox::PluginHMS::rankingSetSwitchStatus(1);
    }));
    mMenu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Submit Score", "arial", 24), [](Ref*){
        showMsg("to rankingSubmitScore...");
        sdkbox::PluginHMS::rankingSubmitScore("shooter", 23, "H");
    }));
    mMenu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Show Ranking", "arial", 24), [](Ref*){
        showMsg("to rankingShow...");
        int timeDimension = 0; // day
        timeDimension = 1; // week
        timeDimension = 2; // all time
        sdkbox::PluginHMS::rankingShow(timeDimension, "shooter");
        // sdkbox::PluginHMS::rankingShow(timeDimension); // show all ranking
    }));
    mMenu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Request Ranking List", "arial", 24), [](Ref*){
        showMsg("to rankingRequestList...");
        bool realtime = true;
        realtime = false; // use cachce
        sdkbox::PluginHMS::rankingRequestList(realtime, "shooter");
    }));
    mMenu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Request Cur Player Score", "arial", 24), [](Ref*){
        showMsg("to rankingRequestCurPlayerScore...");
        std::string rankingName = "shooter";
        int timeDimension = 0; // day
        timeDimension = 1; // week
        timeDimension = 2; // all time
        sdkbox::PluginHMS::rankingRequestCurPlayerScore(rankingName, timeDimension);
    }));
    mMenu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Request Player Centered Scores", "arial", 24), [](Ref*){
        showMsg("to rankingRequestPlayerCenteredScores...");
        std::string rankingName = "shooter";
        int timeDimension = 0; // day
        timeDimension = 1; // week
        timeDimension = 2; // all time
        sdkbox::PluginHMS::rankingRequestPlayerCenteredScores(rankingName, timeDimension, 9, false);
    }));
    mMenu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Request More Scores", "arial", 24), [](Ref*){
        showMsg("to rankingRequestMoreScores...");
        std::string rankingName = "shooter";
        int timeDimension = 0; // day
        timeDimension = 1; // week
        timeDimension = 2; // all time
        int offset = 1;
        int pageSize = 10;
        int pageDirection = 0; //0: next page, 1: previous page
        sdkbox::PluginHMS::rankingRequestMoreScores(rankingName, timeDimension, offset, pageSize, pageDirection);
    }));
    mMenu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Request Top Scores", "arial", 24), [](Ref*){
        showMsg("to rankingRequestTopScores...");
        std::string rankingName = "shooter";
        int timeDimension = 0; // day
        timeDimension = 1; // week
        timeDimension = 2; // all time
        int offset = 1;
        int pageSize = 10;
        int pageDirection = 0; //0: next page, 1: previous page
        sdkbox::PluginHMS::rankingRequestTopScores(rankingName, timeDimension, offset, pageSize, pageDirection);
    }));
}

void HelloWorld::genGameArchiveMenu() {
    mMenu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Request LimitThumbnailSize", "arial", 24), [](Ref*){
        showMsg("to archiveRequestLimitThumbnailSize...");
        sdkbox::PluginHMS::archiveRequestLimitThumbnailSize();
    }));
    mMenu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Request LimitDetailsSize", "arial", 24), [](Ref*){
        showMsg("to archiveRequestLimitDetailsSize...");
        sdkbox::PluginHMS::archiveRequestLimitDetailsSize();
    }));
    mMenu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Add Archive", "arial", 24), [this](Ref*){
        showMsg("to archiveAdd...");
        long playedTime = cocos2d::random(1, 10000);
        long progress = cocos2d::random(1, 100);
        std::string description = "this is archive description";
        bool supportCache = true;
        unsigned char* coverBytes = this->coverData.getBytes();
        int coverBytesLen = (int)this->coverData.getSize();
        std::string coverBytesType = "png";

        std::string archiveStr = "archiveData";
        unsigned char* archiveData = (unsigned char*)archiveStr.c_str();
        int archiveDataLen = (int)archiveStr.length();

        sdkbox::PluginHMS::archiveAdd(playedTime, progress, description, supportCache,
                                      coverBytes, coverBytesLen, coverBytesType,
                                      archiveData, archiveDataLen);
    }));
    mMenu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Show Archives", "arial", 24), [](Ref*){
        showMsg("to archiveShow...");
        std::string archivesTitle = "Archive Title";
        bool showAddButton = true;
        bool showDeleteButton = true;
        int pageSize = 10;
        sdkbox::PluginHMS::archiveShow(archivesTitle, showAddButton, showDeleteButton, pageSize);
    }));
    mMenu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Request Archive Summary List", "arial", 24), [](Ref*){
        showMsg("to archiveRequestSummaryList...");
        bool realtime = true;
        sdkbox::PluginHMS::archiveRequestSummaryList(realtime);
    }));
    mMenu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Request Archive Thumbnail", "arial", 24), [this](Ref*){
        if (0 == this->mArchiveId.size()) {
            showMsg("archive id is empty");
            return;
        }
        showMsg("to archiveRequestThumbnail...");
        sdkbox::PluginHMS::archiveRequestThumbnail(this->mArchiveId);
    }));
    mMenu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Update Archive", "arial", 24), [this](Ref*){
        if (0 == this->mArchiveId.size()) {
            showMsg("archive id is empty");
            return;
        }
        showMsg("to archiveUpdate...");
        long playedTime = cocos2d::random(1, 10000);
        long progress = cocos2d::random(1, 100);
        std::string description = "this is archive description";
        unsigned char* coverBytes = nullptr;
        int coverBytesLen = 0;
        std::string coverBytesType = "";

        std::ostringstream ss;
        ss << "archiveData_" << cocos2d::random(0, 100);
        std::string archiveStr = ss.str();
        unsigned char* archiveData = (unsigned char*)archiveStr.c_str();
        int archiveDataLen = (int)archiveStr.length();

        sdkbox::PluginHMS::archiveUpdate(this->mArchiveId,
                                         playedTime, progress, description,
                                         coverBytes, coverBytesLen, coverBytesType,
                                         archiveData, archiveDataLen);
    }));
    mMenu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Load Archive", "arial", 24), [this](Ref*){
        if (0 == this->mArchiveId.size()) {
            showMsg("archive id is empty");
            return;
        }
        showMsg("to archiveLoad...");
        int conflictPolicy = -1; //hms willn't process conflict
        conflictPolicy = 1; //hms will resolved conflict by played time
        conflictPolicy = 2; //hms will resolved conflict by progress
        conflictPolicy = 3; //hms will resolved conflict by last update time
        sdkbox::PluginHMS::archiveLoad(this->mArchiveId, conflictPolicy);
    }));
    mMenu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Remove Archive", "arial", 24), [this](Ref*){
        if (0 == this->mArchiveId.size()) {
            showMsg("archive id is empty");
            return;
        }
        showMsg("to archiveRemove...");
        sdkbox::PluginHMS::archiveRemove(this->mArchiveId);
    }));

}

void HelloWorld::genGameStatsMenu() {
    mMenu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Game Player Stats", "arial", 24), [](Ref*){
        showMsg("to gamePlayerStatsRequest...");
        bool realtime = false;
        sdkbox::PluginHMS::gamePlayerStatsRequest(realtime);
    }));
    mMenu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Game Summary", "arial", 24), [](Ref*){
        showMsg("to gameSummaryRequest...");
        bool realtime = false;
        sdkbox::PluginHMS::gameSummaryRequest(realtime);
    }));
}

void HelloWorld::genAdMenu() {
    mMenu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Change Ad Name", "arial", 24), [this](Ref*){
        showMsg("to change ad");
        this->mAdList.push_back(std::move(this->mAdName));
        this->mAdName = std::move(this->mAdList.front());
        this->mAdList.pop_front();
        showMsg("Current Ad Name:" + this->mAdName);
    }));
    mMenu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Cache", "arial", 24), [this](Ref*){
        showMsg("to adCache...");
        sdkbox::PluginHMS::adCache(this->mAdName);
    }));
    mMenu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Show", "arial", 24), [this](Ref*){
        if (!sdkbox::PluginHMS::adIsAvailable(this->mAdName)) {
            showMsg("ad is not ready");
            return;
        }
        showMsg("to adShow...");
        sdkbox::PluginHMS::adShow(this->mAdName);
    }));
    mMenu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Hide", "arial", 24), [this](Ref*){
        showMsg("to adHide...");
        sdkbox::PluginHMS::adHide(this->mAdName);
    }));
}

void HelloWorld::addHMSProduct(const HMSProduct& p) {
    this->products.push_back(p);
}

void HelloWorld::addPurchaseToken(const std::string& purchaseToken) {
    this->consumablePurchaseToken.push_back(purchaseToken);
}

bool HelloWorld::isConsumable(const std::string& pid) {
    for (const auto& it : products) {
        if (0 == pid.compare(it.id)) {
            if (0 == it.type.compare("consumable")) {
                return true;
            } else {
                return false;
            }
        }
    }
    
    // unmanged product
    if (0 == pid.compare("consumeproduct1")) {
        return true;
    }
    return false;
}

void HelloWorld::readImageForArchiveCover() {
    this->coverData = cocos2d::FileUtils::getInstance()->getDataFromFile("icon.png");
}

void HelloWorld::setArchiveId(const std::string& aid) {
    this->mArchiveId = aid;
}

