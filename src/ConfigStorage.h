//
// Created by Ryuki
//

#pragma once

#include <QObject>
#include <QFile>
#include <nlohmann/json.hpp>

using namespace std;
using namespace nlohmann;

namespace mosme
{

    /*!
     * @brief Used to manage local session storage
     */
    class ConfigStorage : private QObject
    {
    Q_OBJECT
    friend ostream& operator<<(ostream&, ConfigStorage&);
    private:
        QFile* storage = nullptr;
    private:
        void setFileStorage(const string&);
        bool checkStorage() const;
    public:
        /*!
         * @brief Base domain of the memo instance
         */
        string Host;
        string Username;
        string Password;
        /*!
         * @brief Session cookie, used to access the user account without prompting any password
         */
        string Session;
        /*!
         * @brief Whether the application should (not) authenticate itself.
         */
        bool Guest;
    public:
        ConfigStorage() = delete; // Prohibit the usage of the default ctor in favor for the one below
        explicit ConfigStorage(const string&);
        
        void Load();
        void Load(const string& f)
        {
            setFileStorage(f);
            Load();
        }
        
        void Save() const;
        void Save(const string& f)
        {
            setFileStorage(f);
            Save();
        }
        ~ConfigStorage() override;
    };
} // mosme
