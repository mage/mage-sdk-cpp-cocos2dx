#include "HelloWorldScene.h"

USING_NS_CC;

using namespace mage;

HelloWorld::HelloWorld() :
    mageClient("game", "192.168.11.5:30261")
{
}

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // create and initialize a label

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);

    // add a label shows "Hello World"
    auto label = LabelTTF::create("Loading...", "Arial", 24);

    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

	Json::Value params;
    std::future<void> ret;

	params["password"] = "geronimo!";

    std::cout << std::this_thread::get_id() << " Running" << std::endl;
    
    ret = mageClient.Call("user.register", params, [this, origin, visibleSize, label](mage::MageError err, Json::Value res){
        
        //
        // We simulate a delay so that we may clearly see
        // the async behaviour
        //
        std::chrono::milliseconds duration(3000);
        std::this_thread::sleep_for(duration);
        
        std::cout << std::this_thread::get_id() << " Ran" << std::endl;
        
        std::string text = "";
        
		if (err.type() == MAGE_RPC_ERROR) {
            std::cout << "MAGE_RPC_ERROR" << std::endl;
            
            std::cout << "Setting text" << std::endl;
            text += "RPC Error: ";
            text += err.what();
            
            std::cout << "Setting color" << std::endl;
			label->setColor(ccc3(255,0,0));
			return;
		} else if (err.type() == MAGE_ERROR_MESSAGE) {
            std::cout << "MAGE_ERROR_MESSAGE" << std::endl;
            
            std::cout << "Setting text" << std::endl;
            text += "User command failed: ";
            text += err.code();
            
            std::cout << "Setting color" << std::endl;
			label->setColor(ccc3(255,0,0));
		} else {
            std::cout << "MAGE_SUCCESS" << std::endl;
            
            std::cout << "Setting text" << std::endl;
            text += "User command succeeded: ";
            
            std::cout << "Setting color" << std::endl;
            label->setColor(ccc3(0,255,0));
            
            // add data
            auto data = LabelTTF::create(res.toStyledString(), "Arial", 24);
            
            // position the label on the center of the screen
            data->setPosition(Vec2(origin.x + visibleSize.width/2,
                                    origin.y + 110));
            
            data->setColor(ccc3(0,255,0));
            
            this->addChild(data, 2);
        }
        
        std::cout << "Setting text to:" << text << std::endl;
        label->setString(text);
	}, true);
    
    //
    // We push the received future in magePendings
    // so that we may return early from this function.
    //
    // This also allow us to have a more granular control
    // over the execution flow: we could, for instance, choose to execute
    // a whole bunch of things then wait until the future has executed. It
    // also allows a developer to set a number of callbacks which MUST complete
    // before the parent function may return.
    //
    magePendings.push_back(std::move(ret));
    
    //
    // Debug to show that we are waiting
    //
    std::cout << "Waiting" << std::endl;
    
    //
    // Early return
    //
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
