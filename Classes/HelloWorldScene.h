#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

//
// Chrono and thread will be used
// to induce delay so that we may see the async behaviour
// of the MAGE SDK
//
#include <chrono>
#include <thread>

//
// We need future to receive the future object
//
#include <future>

//
// Finally, we need MAGE!
//
#include "mage.h"

class HelloWorld : public cocos2d::Layer
{
public:
    HelloWorld();
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

    // The MAGE client
    mage::RPC mageClient;

    std::vector<std::future<void>> magePendings;
};

#endif // __HELLOWORLD_SCENE_H__
