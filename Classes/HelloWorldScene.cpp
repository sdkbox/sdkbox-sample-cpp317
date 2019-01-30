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

#include "PluginSdkboxPlay/PluginSdkboxPlay.h"

USING_NS_CC;

using namespace sdkbox;

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


class SPListener : public sdkbox::SdkboxPlayListener {
public:

    virtual void onConnectionStatusChanged( int status ) {
        std::string statusStr = "unknown";
        if (status == sdkbox::GPS_CONNECTED) statusStr = "GPS_CONNECTED";
        else if (status == sdkbox::GPS_DISCONNECTED) statusStr = "GPS_DISCONNECTED";
        else if (status == sdkbox::GPS_CONNECTION_ERROR) statusStr = "GPS_CONNECTION_ERROR";
        else if (status == sdkbox::GPS_FETCH_SERVER_AUTH_CODE_SUCCESS) statusStr = "GPS_FETCH_SERVER_AUTH_CODE_SUCCESS";

        std::stringstream buf;

        buf
        << "Status:" << status << "=" << statusStr
        << "\n. Is connected: " << (sdkbox::PluginSdkboxPlay::isConnected() ? "yes" : "no");

        showMsg(buf.str());

        if ( status == sdkbox::GPS_CONNECTED ) {
            std::string sstr = PluginSdkboxPlay::getPlayerId() +
                ":'" +
                PluginSdkboxPlay::getPlayerAccountField("name")+
                "(" +
                PluginSdkboxPlay::getPlayerAccountField("display_name")+
                ")'";

            showMsg( sstr );
        } else if ( status == sdkbox::GPS_FETCH_SERVER_AUTH_CODE_SUCCESS) {
            std::string sstr =
                PluginSdkboxPlay::getPlayerAccountField("name")+
                "(" +
                PluginSdkboxPlay::getPlayerAccountField("server_auth_code")+
                ")'";
            showMsg( sstr );
        }
    }
    virtual void onScoreSubmitted(const std::string& leaderboard_name,
                                  long score, bool maxScoreAllTime,
                                  bool maxScoreWeek,
                                  bool maxScoreToday ) {}
    virtual void onMyScore(const std::string& leaderboard_name,
                           int time_span,
                           int collection_type,
                           long score ) {}
    virtual void onMyScoreError(const std::string& leaderboard_name,
                                int time_span,
                                int collection_type,
                                int error_code,
                                const std::string& error_description) {}
    virtual void onPlayerCenteredScores( const std::string& leaderboard_name,
                                        int time_span,
                                        int collection_type,
                                        const std::string& json_with_score_entries ) {}
    virtual void onPlayerCenteredScoresError( const std::string& leaderboard_name,
                                             int time_span,
                                             int collection_type,
                                             int error_code,
                                             const std::string& error_description) {}
    virtual void onIncrementalAchievementUnlocked( const std::string& achievement_name ) {}
    virtual void onIncrementalAchievementStep( const std::string& achievement_name, double step ) {}
    virtual void onIncrementalAchievementStepError( const std::string& name, double steps, int error_code, const std::string& error_description ) {}
    virtual void onAchievementUnlocked( const std::string& achievement_name, bool newlyUnlocked ) {}
    virtual void onAchievementUnlockError( const std::string& achievement_name, int error_code, const std::string& error_description ) {}
    virtual void onAchievementsLoaded( bool reload_forced, const std::string& json_achievements_info ) {}
    virtual void onSetSteps( const std::string& name, double steps ) {}
    virtual void onSetStepsError( const std::string& name, double steps, int error_code, const std::string& error_description ) {}
    virtual void onReveal( const std::string& name) {}
    virtual void onRevealError( const std::string& name, int error_code, const std::string& error_description ) {}
    virtual void onGameData(const std::string& action, const std::string& name, const std::string& data, const std::string& error) {}
    virtual void onSaveGameData(bool success, const std::string& error) {
        std::stringstream buf;

        buf << "onSaveGameData:" << success << " e:" << error;
        showMsg(buf.str());
    }
    virtual void onLoadGameData(const sdkbox::SavedGameData* savedData, const std::string& error) {
        if (nullptr == savedData) {
            return;
        }
        std::stringstream buf;

        buf << "onLoadGameData:" << savedData->name;
        showMsg(buf.str());
    }
    virtual void onGameDataNames(const std::vector<std::string>& names, const std::string& error) {}

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

    menu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Signin/Signout", "arial", 24), [](Ref*){
        if (sdkbox::PluginSdkboxPlay::isSignedIn()) {
            showMsg("SP to signout");
            sdkbox::PluginSdkboxPlay::signout();
        } else {
            showMsg("SP to signin");
            bool showLoginUI = true;
            sdkbox::PluginSdkboxPlay::signin(showLoginUI);
        }
    }));
    menu->addChild(MenuItemLabel::create(Label::createWithSystemFont("LoadGameData", "arial", 24), [](Ref*){
        sdkbox::PluginSdkboxPlay::loadAllGameData();
    }));
    menu->addChild(MenuItemLabel::create(Label::createWithSystemFont("SaveGameData", "arial", 24), [](Ref*){
        std::string sData = "k3data";
        const void* data = (const void*)sData.c_str();
        int len = (int)sData.length();
        sdkbox::PluginSdkboxPlay::saveGameDataBinary("key3", data, len);
    }));

    menu->alignItemsVerticallyWithPadding(10);
    addChild(menu);

    sdkbox::PluginSdkboxPlay::setListener(new SPListener());
    sdkbox::PluginSdkboxPlay::init();
}
