//
// Created by Ryuki
//

#include "ConfigStorage.h"
#include <QtGlobal>
#include <QtDebug>
#include <fmt/format.h>

using namespace std;
using namespace nlohmann;
using namespace fmt;

namespace mosme
{
    void ConfigStorage::setFileStorage(const string &f)
    {
        delete storage;
        filesystem::path path(f);
        storage = new QFile(path);
    }

    ConfigStorage::~ConfigStorage()
    {
        delete storage;
    }

    ConfigStorage::ConfigStorage(const string &f)
    {
        Load(f);
    }

    void ConfigStorage::Load()
    {
        qInfo() << "Loading config...";
        if (!checkStorage())
            return;
        qDebug() << "Trying to open config file for reading...";
        if (!storage->open(QFile::ReadOnly))
        {
            qWarning() << "Could not open config file!";
            return;
        }

        qDebug() << "Trying to read from MessagePack JSON...";
        try
        {
            QByteArray buf = storage->readAll();
            if (!buf.size()) // Buffer size of zero
            {
                storage->close();
                qInfo() << "Config file is empty!";
                return;
            }
            json j = json::from_msgpack(buf.constData());
            j.at("host").get_to(Host);
            j.at("guest").get_to(Guest);

            if (!Guest)
            {
                j.at("username").get_to(Username);
                j.at("password").get_to(Password);
                j.at("session").get_to(Session);
            }
        }
        catch (json::parse_error &)
        {
            qCritical() << "Could not parse config file!";
            return;
        }
        storage->close();
        qInfo() << "OK";
    }

    void ConfigStorage::Save() const
    {
        if (!PersistentStorage)
        {
            qInfo() << "User requested to not save config.";
            return;
        }

        qInfo() << "Saving config...";
        if (!checkStorage())
            return;

        qDebug() << "Trying to open config file for writing...";
        if (!storage->open(QFile::WriteOnly | QFile::Truncate))
        {
            qWarning() << "Could not open config file!";
            return;
        }

        json j = Guest
                 ? json{
                        {"host",     Host},
                        {"guest",    Guest},
                        {"session",  ""},
                        {"username", ""},
                        {"password", ""},
                }
                 : json{
                        {"host",     Host},
                        {"username", Username},
                        {"password", Password},
                        {"guest",    Guest},
                        {"session",  Session}
                };

        // Because the vector returned by json::to_msgpack contains unsigned char/bytes
        // We need to change them into signed char/bytes because that's what QFile::write()
        // expects to receive. Bitwise, this doesn't change anything, it's just an OOP
        // nitpick from C++.
        vector<uint8_t> data = json::to_msgpack(j);
        storage->write(reinterpret_cast<const char*>(data.data()), data.size());
        storage->close();
        qInfo() << "OK";
    }

    bool ConfigStorage::checkStorage() const
    {
        qDebug() << "Checking storage integrity..."; // Not quite actually :)
        qDebug() << "Checking if storage is pointed.";
        if (!storage) // The pointer to the QFile is NULL = 0
        {
            qWarning() << "No storage available";
            return false;
        }

        qDebug() << "Checking if config file exists...";
        if (!storage->exists())
        {
            qWarning() << "Storage not present. Creating new one...";
            if (!storage->open(QFile::WriteOnly))
            {
                qCritical() << "Cannot create config file!";
                return false;
            }
            storage->close();
            qInfo() << "Storage created!";
        }
        qInfo() << "OK";
        return true;
    }

    ostream &operator<<(ostream &out, ConfigStorage &config)
    {
        out << format("Config(Host={0}, Guest={1}, Username={2}, Password={3}, Session={4})", config.Host,
                      config.Guest ? "yes" : "no", config.Username, config.Password, config.Session);
        return out;
    }

    bool ConfigStorage::GetSessionCookie(QNetworkCookie* cookie)
    {
        if (!PersistentStorage)
            return false;

        try
        {
            QString name("memo_session");
            QString value(Session.c_str());
            cookie = new QNetworkCookie(name.toUtf8(), value.toUtf8());
        }
        catch (exception &)
        {
            return false;
        }
        return true;
    }
} // mosme