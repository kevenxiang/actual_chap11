//
//  TileMapTestScene.cpp
//  actual_chap11
//
//  Created by xiang on 2017/8/2.
//
//

#include "TileMapTestScene.hpp"

Scene *TileMapTestScene::createScene() {
    auto scene = Scene::create();
    auto layer = TileMapTestScene::create();
    scene->addChild(layer);
    return scene;
}

bool TileMapTestScene::init() {
    if (!Layer::init()) {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    // 关闭菜单
    auto closeItem = MenuItemImage::create("CloseNormal.png", "CloseSelected.png", CC_CALLBACK_1(TileMapTestScene::menuCloseCallback, this));
    closeItem->setPosition(Vec2(origin.x+visibleSize.width-closeItem->getContentSize().width/2, origin.y+closeItem->getContentSize().height/2));
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    // 获取屏幕宽度和高度
    _screenWidth = visibleSize.width;
    _screenHeight = visibleSize.height;
    
    // 调用TMXTiledMap的函数读取TileMap.tmx文件创建一个瓦片地图对象
    _tileMap = TMXTiledMap::create("TileMap.tmx");
    // 把地图的锚点和位置都设置为原点，这样可以使地图的左下角与屏幕的左下角对齐
    _tileMap->setAnchorPoint(Vec2::ZERO);
    _tileMap->setPosition(Vec2::ZERO);
    // 将地图添加为当前层的子节点
    this->addChild(_tileMap, -1);
    
    // 调用TMXTiledMap的getObjectGroup方法获取对象层
    auto objects = _tileMap->getObjectGroup("Objects");
    // 根据对象名称获取对象的信息
    ValueMap map = objects->getObject("player");
    // 获取地图中设置的player对象的x和y值
    int x = map.at("x").asInt();
    int y = map.at("y").asInt();
    
    // 创建玩家精灵，并将地图中对象的x和y值作为玩家精灵对象出现的位置
    _player = Sprite::create("player.png");
    _player->setPosition(x, y);
    // 将玩家精灵添加为地图的子节点
    _tileMap->addChild(_player);
    
    // 创建事件监听器
    auto planeListener = EventListenerTouchOneByOne::create();
    // 响应触摸事件函数
    planeListener->onTouchBegan = [](Touch *touch, Event *event){
        return true;
    };
    planeListener->onTouchEnded = [this](Touch *touch, Event *event) {
        // OpenGL坐标
        Vec2 touchLocation = touch->getLocation();
        // 相对Node的坐标
        Vec2 nodeLocation = this->convertToNodeSpace(touchLocation);
        // 镜像反转，使玩家面对移动的方向
        if (_player->getPosition().x < nodeLocation.x) {
            if (!_player->isFlippedX()) {
                _player->setFlippedX(true);
            }
        } else {
            if (_player->isFlippedX()) {
                _player->setFlippedX(false);
            }
        }
        
        // 重新设置玩家位置
//        _player->setPosition(nodeLocation);
        
        MoveTo *moveTo = MoveTo::create(2, nodeLocation);
        _player->runAction(moveTo);
        
        // 调整视角中心位置让地图随玩家一起移动
        this->setViewpointCenter(_player->getPosition());
    };
    
    // 添加场景优先事件监听器
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(planeListener, this);
    
    return true;
}

// 把玩家作为视觉中心来显示，让地图随玩家一起移动
void TileMapTestScene::setViewpointCenter(Vec2 position) {
    // 让地图跟随精灵移动：超过屏幕中心的一半的时候需要移动
    int x = MAX(position.x, _screenWidth/2);
    int y = MAX(position.y, _screenHeight/2);
    // 不让显示区域超出地图的边界
    x = MIN(x, (_tileMap->getMapSize().width *
                _tileMap->getTileSize().width-_screenWidth/2));
    y = MIN(y, (_tileMap->getMapSize().height *
                _tileMap->getTileSize().height-_screenHeight/2));
    
    // 实际移动的位置坐标
    Point actualPosition = Vec2(x, y);
    // 屏幕中心位置坐标
    Point centerOfView = Vec2(_screenWidth/2, _screenHeight/2);
    // 计算中心位置和玩家位置两点之间的差值
    Point viewPoint = centerOfView - actualPosition;
    // 重新设置屏幕位置
    this->setPosition(viewPoint);

}

void TileMapTestScene::menuCloseCallback(Ref* pSender)
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





































