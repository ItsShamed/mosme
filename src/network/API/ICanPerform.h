//
// Created by Ryuki
//

#pragma once

namespace mosme
{
    template<typename T>
    class ICanPerform
    {
    public:
        virtual void Perform(T*) = 0;
    };
} // mosme
