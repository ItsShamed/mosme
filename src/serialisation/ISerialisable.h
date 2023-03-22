//
// Created by Ryuki
//

#pragma once


#include <QJsonObject>

class ISerialisable
{
private:
    ISerialisable() = delete;
    QJsonObject& json;
protected:
    ISerialisable(QJsonObject& json);
    virtual void ApplyState(QJsonObject& json) = 0;
};
