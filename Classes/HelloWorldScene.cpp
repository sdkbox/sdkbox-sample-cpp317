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

#include <vector>

#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

#include "json/rapidjson.h"
#include "json/document.h"
#include "json/stringbuffer.h"
#include "json/writer.h"

#include "PluginIAP/PluginIAP.h"


#define APP_SHARED_SECRET "bc92e25bab7741d0868256fcf8bd71e3"

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

class IAPPluginListener : public sdkbox::IAPListener {
public:

    HelloWorld* hwScene;
    
    IAPPluginListener() {
        hwScene = nullptr;
    }

    virtual void onInitialized(bool success) {
        std::stringstream buf;
        buf << "IAP onInitialized:" << success;
        showMsg(buf.str());
    }

    virtual void onSuccess(const sdkbox::Product& p) {
        std::stringstream buf;
        buf << "IAP onSuccess:" << p.name;
        showMsg(buf.str());
        if (nullptr != hwScene) {
            if (p.name == "subscription1") {
                hwScene->receipt = p.receiptCipheredPayload;
            }
        }
    }
    virtual void onFailure(const sdkbox::Product& p, const std::string& msg) {}
    virtual void onCanceled(const sdkbox::Product& p) {}
    virtual void onRestored(const sdkbox::Product& p) {
        std::stringstream buf;
        buf << "IAP onRestored:" << p.name << ":" << p.price;
        showMsg(buf.str());

        rapidjson::Document d;
        if (p.receipt.size() > 0) {
            d.Parse<0>(p.receipt.c_str());
            if (!d.HasParseError()) {
                if (d.IsObject() && d.HasMember("purchaseToken")) {
                    buf.str("");
                    buf << "IAP onRestored: purchaseToken:" << d["purchaseToken"].GetString();
                    showMsg(buf.str());
                }
            }
        }
    }

    virtual void onProductRequestSuccess(const std::vector<sdkbox::Product>& products) {
        std::stringstream buf;
        buf << "IAP onProductRequestSuccess:";
        showMsg(buf.str());
        for (sdkbox::Product p : products) {
            buf.str("");
            buf << p.name << ":" << p.price;
            showMsg(buf.str());
        }
    }

    virtual void onProductRequestFailure(const std::string& msg) {}

    virtual void onRestoreComplete(bool ok, const std::string &msg) {
        std::stringstream buf;
        buf << "IAP onRestoreComplete:" << ok << ":" << msg;
        showMsg(buf.str());
    }

    virtual bool onShouldAddStorePayment(const std::string& productName) { return true; };
    virtual void onFetchStorePromotionOrder(const std::vector<std::string>& productNames, const std::string& error) {};
    virtual void onFetchStorePromotionVisibility(const std::string productName,
                                                 bool visibility,
                                                 const std::string& error) {};
    virtual void onUpdateStorePromotionOrder(const std::string& error) {};
    virtual void onUpdateStorePromotionVisibility(const std::string& error) {};

    virtual void onConsumed(const sdkbox::Product& p, const std::string& error) {
        std::stringstream buf;
        buf << "IAP onConsumed:" << p.name;
        if (error.empty()) {
            buf << " Suc";
        } else {
            buf << " fal:" << error;
        }
        showMsg(buf.str());

        if (!error.empty()) {
            // consume failed, try again
            sdkbox::IAP::finishTransaction(p.id);
        }
    };
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
    receipt = "";
    
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

    menu->addChild(MenuItemLabel::create(Label::createWithSystemFont("List Products", "arial", 12), [](Ref*){
        auto list = sdkbox::IAP::getProducts();
        std::stringstream buf;
        showMsg("List Products");
        for (auto p : list) {
            buf.str("");
            buf << p.name << ":" << p.price;
            showMsg(buf.str());

            cocos2d::log("IAP: Name: %s", p.name.c_str());
            cocos2d::log("IAP: ID: %s", p.id.c_str());
            cocos2d::log("IAP: Title: %s", p.title.c_str());
            cocos2d::log("IAP: Desc: %s", p.description.c_str());
            cocos2d::log("IAP: Price: %s", p.price.c_str());
            cocos2d::log("IAP: Price Currency: %s", p.currencyCode.c_str());
            cocos2d::log("IAP: Price Value: %f", p.priceValue);
            cocos2d::log("IAP: transactionID: %s", p.transactionID.c_str());
        }
    }));
    // menu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Request Products", "arial", 12), [](Ref*){
    //     showMsg("Request Products");
    //     sdkbox::IAP::refresh();
    // }));
    menu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Restore", "arial", 12), [](Ref*){
        showMsg("Restore");
        sdkbox::IAP::restore();
    }));
    menu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Purchase", "arial", 12), [](Ref*){
        showMsg("Purchase coin_package");
        sdkbox::IAP::purchase("coin_package");
        // sdkbox::IAP::purchase("subscription1");
    }));
    menu->addChild(MenuItemLabel::create(Label::createWithSystemFont("VerifyReceipt", "arial", 12), [this](Ref*){
        showMsg("Verify auto renewing receipt");
        showMsg("This is for testing, please DO NOT use it in a production environment.");
        this->verifyReceipt();
    }));
    
    menu->alignItemsVerticallyWithPadding(10);
    addChild(menu);
    
    IAPPluginListener *lis = new IAPPluginListener();
    lis->hwScene = this;
    sdkbox::IAP::setListener(lis);
    sdkbox::IAP::init();
    sdkbox::IAP::setDebug(true);
    sdkbox::IAP::enableUserSideVerification(true);

}


std::string HelloWorld::getVerifyData() {
    if (0 == this->receipt.size()) {
        return "";
    }

    rapidjson::Document document;
    document.SetObject();
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    document.AddMember("receipt-data", rapidjson::StringRef(this->receipt.c_str()), allocator);
    document.AddMember("password", APP_SHARED_SECRET, allocator);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    document.Accept(writer);

    return buffer.GetString();
}

/*
 * This is for TEST, please DO NOT verify receipt in your app directly!
 *
 */
void HelloWorld::verifyReceipt() {
    std::string url = "https://sandbox.itunes.apple.com/verifyReceipt";
    //std::string url = "https://buy.itunes.apple.com/verifyReceipt";

    std::string postData = this->getVerifyData();
    if (0 == postData.size()) {
        showMsg("ERROR! post data is 0, can't verify receipt");
        return;
    }
    cocos2d::network::HttpRequest* request = new cocos2d::network::HttpRequest();
    request->setUrl(url.c_str());
    request->setRequestType(cocos2d::network::HttpRequest::Type::POST);
    request->setResponseCallback([this](cocos2d::network::HttpClient* client, cocos2d::network::HttpResponse* response) {
        this->onVerifyReceipt(client, response);
    });
    request->setTag("verifyReceipt");

    std::vector<std::string> headers;
    headers.push_back("Content-Type: application/json; charset=utf-8");
    request->setHeaders(headers);
    request->setRequestData(postData.c_str(), postData.size());
    
    cocos2d::network::HttpClient::getInstance()->send(request);
    request->release();
}

void HelloWorld::onVerifyReceipt(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response) {
    
    long code = response->getResponseCode();
    if (200 != code) {
        showMsg("ERROR! veiry receipt response code is not 200");
        log("%ld", code);
        return;
    }
    std::vector<char> *respData = response->getResponseData();
    std::stringstream ss;

    for (unsigned int i = 0; i < respData->size(); i++){
        ss << (*respData)[i];
    }
    rapidjson::Document d;
    d.Parse<0>(ss.str().c_str());
    if (d.HasParseError()) {
        showMsg("ERROR! parse verify receipt response fialed");
        return;
    }
    if (!d.IsObject()) {
        showMsg("ERROR! parse verify receipt response is not json object");
        return;
    }
    if (d.HasMember("status")) {
        int status = d["status"].GetInt();
        if (0 == status) {
            showMsg("SUCCESS! VerifyReceipt success");
        } else {
            showMsg("ERROR! verify receipt failed");
        }
    }
}
