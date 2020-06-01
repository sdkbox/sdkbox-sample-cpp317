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
        label = Label::createWithSystemFont("test", "arial", 6);
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

    void onLogin(int code, const std::string& msg) {
        if (0 == code) {
            rapidjson::Document doc;
            doc.Parse(msg.c_str());
            
            std::ostringstream ss;
            ss << "Login success:" << doc["displayName"].GetString();
            showMsg(ss.str());
            cocos2d::log("UserInfo: %s", msg.c_str());
        } else {
            showMsg("login failed:" + msg);
        }
    }

    void onIAPReady(int code, const std::string& msg) {
        std::ostringstream ss;
        ss << "HMS listener onIAPReady code:" << code;
        showMsg(ss.str());
        cocos2d::log("%s", msg.c_str());
    }

    void onIAPProducts(int code, const std::string& msg) {
        std::ostringstream ss;
        ss << "HMS listener onIAPProducts code:" << code;
        showMsg(ss.str());
        cocos2d::log("%s", msg.c_str());
        
        if (0 != code) {
            return;
        }
        rapidjson::Document doc;
        doc.Parse(msg.c_str());
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

    void onIAPPurchase(int code, const std::string& msg) {
        std::ostringstream ss;
        ss << "HMS listener onIAPPurchase code:" << code;
        showMsg(ss.str());
        cocos2d::log("%s", msg.c_str());
        
        if (0 != code) {
            return;
        }
        rapidjson::Document doc;
        doc.Parse(msg.c_str());
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

    void onIAPPConsume(int code, const std::string& msg) {
        std::ostringstream ss;
        ss << "HMS listener onIAPPConsume code:" << code;
        showMsg(ss.str());
        cocos2d::log("%s", msg.c_str());
        if (0 == code) {
            // consume success, delivery this consumable to player
            ss.str("");
            ss << "Consume Success, delivery to player";
            showMsg(ss.str());
        }
    }

    void onIAPOwnedPurchases(int code, const std::string& msg) {
        std::ostringstream ss;
        ss << "HMS listener onIAPOwnedPurchases code:" << code;
        showMsg(ss.str());
        cocos2d::log("%s", msg.c_str());

        if (0 != code) {
            return;
        }
        rapidjson::Document doc;
        doc.Parse(msg.c_str());
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

    void onIAPOwnedPurchaseRecords(int code, const std::string& msg) {
        std::ostringstream ss;
        ss << "HMS listener onIAPOwnedPurchaseRecords code:" << code;
        showMsg(ss.str());
        cocos2d::log("%s", msg.c_str());
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
    
    createTestMenu();

    sdkbox::PluginHMS::setListener(new PluginHMSListener(this));
    sdkbox::PluginHMS::init();

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

    menu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Login", "arial", 6), [](Ref*){
        showMsg("to login...");
        // sdkbox::PluginHMS::login(0); // slient login
        sdkbox::PluginHMS::login(1); // login (id token)
        // sdkbox::PluginHMS::login(2); // login (author code)
    }));
    menu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Logout", "arial", 6), [](Ref*){
        showMsg("to logout...");
        sdkbox::PluginHMS::logout();
    }));
    menu->addChild(MenuItemLabel::create(Label::createWithSystemFont("RequestProducts", "arial", 6), [](Ref*){
        showMsg("to RequestProducts...");
        sdkbox::PluginHMS::iapRequestProducts();
    }));
    menu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Purchase", "arial", 6), [](Ref*){
        showMsg("to Purchase...");
        std::string productName = "coin";
//        productName = "remove_ads";
//        productName = "vip";
        sdkbox::PluginHMS::iapPurchase("coin");
    }));
    menu->addChild(MenuItemLabel::create(Label::createWithSystemFont("PurchaseWithPrice", "arial", 6), [](Ref*){
        showMsg("to PurchaseWithPrice...");
        std::string productInfo = "{\"currency\":\"CNY\",\"priceType\":0,\"sdkChannel\":\"1\",\"productName\":\"test\",\"amount\":\"1.99\",\"productId\":\"consumeproduct1\",\"serviceCatalog\":\"X38\",\"country\":\"CN\",\"reservedInfor\":\"{\\\"a\\\": 1, \\\"b\\\":\\\"s\\\"}\",\"developerPayload\":\"payload1\"}";
        sdkbox::PluginHMS::iapPurchaseWithPrice(productInfo);
    }));
    menu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Consume", "arial", 6), [this](Ref*){
        showMsg("to Consume...");
        if (0 == this->consumablePurchaseToken.size()) {
            return ;
        }
        const std::string purchaseToken = this->consumablePurchaseToken.front();
        this->consumablePurchaseToken.pop_front();
        sdkbox::PluginHMS::iapConsume(purchaseToken);
    }));
    menu->addChild(MenuItemLabel::create(Label::createWithSystemFont("RequestOwnedPurchase", "arial", 6), [](Ref*){
        showMsg("to RequestOwnedPurchase...");
        sdkbox::PluginHMS::iapRequestOwnedPurchases();
    }));
    menu->addChild(MenuItemLabel::create(Label::createWithSystemFont("RequestOwnedPurchaseRecord", "arial", 6), [](Ref*){
        showMsg("to RequestOwnedPurchaseRecord...");
        sdkbox::PluginHMS::iapRequestOwnedPurchaseRecords();
    }));
    
    menu->alignItemsVerticallyWithPadding(2);
    addChild(menu);
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
    return false;
}

