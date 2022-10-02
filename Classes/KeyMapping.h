    /**
    Copyright (c) 2022      @aismann; Peter Eismann, Germany; dreifrankensoft
     works with PS4 (dualshock), Xbox One Controller
     PS3 works with the original Controller::Key settings
     */
    enum Key  // overwrite Controller::Key 
    {
        KEY_NONE = 0,

        JOYSTICK_LEFT_X = 0,
        JOYSTICK_LEFT_Y = 1,
        JOYSTICK_RIGHT_X =2,
        JOYSTICK_RIGHT_Y =3,

        BUTTON_A = 0,
        BUTTON_B = 1,
        BUTTON_C = 7, //share
        BUTTON_X = 2,
        BUTTON_Y = 3,
        BUTTON_Z = 6, //options

        BUTTON_DPAD_UP = 10 ,
        BUTTON_DPAD_DOWN = 12,
        BUTTON_DPAD_LEFT = 13,
        BUTTON_DPAD_RIGHT = 11,
        BUTTON_DPAD_CENTER,

        BUTTON_LEFT_SHOULDER = 4,
        BUTTON_RIGHT_SHOULDER =5,

        AXIS_LEFT_TRIGGER = 4,
        AXIS_RIGHT_TRIGGER =5,

        BUTTON_LEFT_THUMBSTICK = 8,
        BUTTON_RIGHT_THUMBSTICK = 9,

        BUTTON_START,
        BUTTON_SELECT,

        BUTTON_PAUSE,
        KEY_MAX
    };