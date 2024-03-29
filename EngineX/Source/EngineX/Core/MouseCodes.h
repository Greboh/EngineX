﻿#pragma once
#include <cstdint>

namespace EngineX
{
    using MouseCode = uint16_t;

    namespace Mouse
    {
        enum : MouseCode
        {
            // From glfw3.h
            Button0 = 0,
            Button1 = 1,
            Button2 = 2,
            Button3 = 3,
            Button4 = 4,
            Button5 = 5,
            Button6 = 6,
            Button7 = 7,

            ButtonLast = Button7,
            LeftClick = Button0,
            RightClick = Button1,
            MouseWheelClick = Button2
        };
    }
}
