//
// Created by Ryuki
//

#pragma once

#include <QByteArray>
#include <QString>
#include <nlohmann/json.hpp>

using namespace nlohmann;

namespace mosme
{

    class JsonUtils
    {
    public:
        static inline QString JsonToQString(const json &j)
        {
            return {j.dump().c_str()};
        }

        static inline QByteArray JsonToQByteArray(const json &j)
        {
            return {JsonToQString(j).toUtf8()};
        }
        
        static inline json QStringToJson(const QString &s)
        {
            return json::parse(s.toStdString());
        }
        
        static inline json QByteArrayToJson(const QByteArray &arr)
        {
            qDebug() << arr.toStdString().c_str();
            return json::parse(arr.toStdString());
        }
    };
} // mosme
