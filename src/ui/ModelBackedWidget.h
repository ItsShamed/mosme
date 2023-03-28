//
// Created by Ryuki
//

#pragma once

#include <QWidget>

namespace mosme
{
    template <typename TModel>
    class ModelBackedWidget : public QWidget
    {
    private:
        TModel& model;
    public:
        virtual void SetModel(const TModel&) = 0;
        virtual TModel GetModel() const = 0;
    };
} // mosme
