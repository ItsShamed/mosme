//
// Created by Ryuki
//

#pragma once

#include <QWidget>
#include "wobjectimpl.h"

namespace mosme
{
    template <typename TModel>
    class IModelBackedWidget
    {
    public:
        virtual void SetModel(const TModel&) = 0;
        virtual TModel* GetModel() const = 0;
    };
} // mosme
