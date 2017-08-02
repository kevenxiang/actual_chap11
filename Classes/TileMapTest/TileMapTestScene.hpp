//
//  TileMapTestScene.hpp
//  actual_chap11
//
//  Created by xiang on 2017/8/2.
//
//

#ifndef TileMapTestScene_hpp
#define TileMapTestScene_hpp

#include <stdio.h>
#include "cocos2d.h"

USING_NS_CC;

class TileMapTestScene : public cocos2d::Layer {
private:
    TMXTiledMap *_tileMap;  //地图
    Sprite *_player;  //玩家
    int _screenWidth,_screenHeight;  // 屏幕的宽度和高度
    
public:
    static cocos2d::Scene *createScene();
    
    virtual bool init();
    
    // 把玩家作为视觉中心来显示，让地图随玩家一起移动
    void setViewpointCenter(Vec2 position);
    
    // 选择器回调函数
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    CREATE_FUNC(TileMapTestScene);

};

#endif /* TileMapTestScene_hpp */
































