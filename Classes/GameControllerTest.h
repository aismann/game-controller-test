/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 Copyright (c) 2022      @aismann; Peter Eismann, Germany; dreifrankensoft
 
 http://www.ax-x.org
 
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

#ifndef __GAMECONTROLLER_TEST_H__
#define __GAMECONTROLLER_TEST_H__

#include "axmol.h"
#include "base/CCGameController.h"

USING_NS_AX;

class GameControllerTest : public ax::Layer
{
public:
    virtual ~GameControllerTest();
    
    CREATE_FUNC(GameControllerTest);

    virtual bool init();
    
    void onKeyDown(ax::Controller* controller, int keyCode, ax::Event* event);
    void onKeyUp(ax::Controller* controller, int keyCode, ax::Event* event);
    void onAxisEvent(ax::Controller* controller, int keyCode, ax::Event* event);
    
    void menuCloseCallback(Ref* sender);
    
private:
    float _visibleCentreX;
    float _visibleCentreY;
    float _visibleQuarterX;
    float _visibleThreeQuarterX;

    int _currControllerCount;

    typedef struct controllerHolder
    {
        ax::Controller *controller;

        ax::Node* _holderNode;

        ax::Sprite* _leftJoystick;
        ax::Sprite* _rightJoystick;

        ax::Sprite* _dpadLeft;
        ax::Sprite* _dpadRight;
        ax::Sprite* _dpadUp;
        ax::Sprite* _dpadDown;

        ax::Sprite* _buttonX;
        ax::Sprite* _buttonY;
        ax::Sprite* _buttonA;
        ax::Sprite* _buttonB;

        ax::Sprite* _buttonL1;
        ax::Sprite* _buttonR1;
        ax::Sprite* _buttonL2;
        ax::Sprite* _buttonR2;

        ax::Sprite* _share;
        ax::Sprite* _options;

        ax::Label* _deviceLabel;
        ax::Label* _externalKeyLabel;
    }ControllerHolder;

    ControllerHolder _firstHolder;
    ControllerHolder _secondHolder;

    ax::EventListenerController* _listener;

    ax::Size _visibleSize;
    ax::Vec2 _visibleOrigin;

    void registerControllerListener();
    void showButtonState(ax::Controller *controller, int keyCode, bool isPressed);

    void createControllerSprite(ControllerHolder& holder);

    void onConnectController(Controller* controller, Event* event);
    void onDisconnectedController(Controller* controller, Event* event);

    void resetControllerHolderState(ControllerHolder& holder);
};

#endif // __GAMECONTROLLER_TEST_H__
