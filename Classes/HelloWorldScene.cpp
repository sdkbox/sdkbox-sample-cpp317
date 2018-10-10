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

#include "Plugingpg/PluginGPG.h"
#include "gpg/player_manager.h"
#include "gpg/builder.h"


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

    menu->addChild(MenuItemLabel::create(Label::createWithSystemFont("SignIn", "arial", 24), [this](Ref*){
        this->gameServices = gpg::GameServices::Builder()
            .SetOnAuthActionFinished([this](gpg::AuthOperation op, gpg::AuthStatus status) {
                std::stringstream buf;
                if( status == gpg::AuthStatus::VALID ){
                    this->gameServices->Players().FetchSelf([=](gpg::PlayerManager::FetchSelfResponse const &response) {
                        if (gpg::IsSuccess(response.status)) {
                            std::stringstream buf;
                            buf << "Player Name:" << response.data.Name() << " id:" << response.data.Id();
                            showMsg(buf.str());
                        }
                    });
                    this->isSignedIn = true;
                    buf << "SignedIn success:" << (int)status;
                } else {
                    this->isSignedIn = false;
                    
                    buf << "SignedIn failed:" << (int)status;
                }
                showMsg(buf.str());
        })
        .SetOnMultiplayerInvitationEvent([](gpg::MultiplayerEvent evt, std::string msg, gpg::MultiplayerInvitation inv){
        })
        .EnableSnapshots()
        .SetDefaultOnLog(gpg::LogLevel::VERBOSE)
        .Create(*CreatePlatformConfiguration().get());
    }));
    menu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Create/Join", "arial", 24), [this](Ref*){
        gpg::RealTimeRoomConfig config =
            gpg::RealTimeRoomConfig::Builder()
            .SetMinimumAutomatchingPlayers(2)
            .SetMaximumAutomatchingPlayers(2)
            .Create();

        this->gameServices->RealTimeMultiplayer().CreateRealTimeRoom(config,
                           this,
                           [this](gpg::RealTimeMultiplayerManager::RealTimeRoomResponse const & response) {
                               std::stringstream buf;
                               if (gpg::MultiplayerStatus::VALID == response.status) {
                                   buf << "Room success: " << response.room.Id();
                               } else {
                                   buf << "Room failed: " << response.status;
                               }
                               showMsg(buf.str());
                           });
    }));
    
    menu->alignItemsVerticallyWithPadding(10);
    addChild(menu);
    
    sdkbox::PluginGPG::init();
}


void HelloWorld::OnRoomStatusChanged(gpg::RealTimeRoom const &room) {
    showMsg("OnRoomStatusChanged");
}

void HelloWorld::OnConnectedSetChanged(gpg::RealTimeRoom const &room) {
    showMsg("OnConnectedSetChanged");
}

void HelloWorld::OnP2PConnected(gpg::RealTimeRoom const &room, gpg::MultiplayerParticipant const &participant) {
    showMsg("OnP2PConnected");
}

void HelloWorld::OnP2PDisconnected(gpg::RealTimeRoom const &room, gpg::MultiplayerParticipant const &participant) {
    showMsg("OnP2PDisconnected");
}

void HelloWorld::OnParticipantStatusChanged(gpg::RealTimeRoom const &room, gpg::MultiplayerParticipant const &participant) {
    showMsg("OnParticipantStatusChanged");
}

void HelloWorld::OnDataReceived(gpg::RealTimeRoom const &room,
                                gpg::MultiplayerParticipant const &from_participant,
                                std::vector<uint8_t> data,
                                bool is_reliable) {
    showMsg("OnDataReceived");
}


