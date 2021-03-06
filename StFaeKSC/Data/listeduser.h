/*
*	This file is part of StamOrga
*   Copyright (C) 2017 Markus Schneider
*
*	This program is free software; you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation; either version 3 of the License, or
*   (at your option) any later version.
*
*	StamOrga is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.

*    You should have received a copy of the GNU General Public License
*    along with StamOrga.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef LISTEDUSER_H
#define LISTEDUSER_H


#include <QtCore/QCryptographicHash>
#include <QtCore/QList>
#include <QtCore/QMutex>
#include <QtCore/QSettings>

#include "configlist.h"

class UserLogin : public ConfigItem
{
public:
    QString m_password;
    QString m_salt;
    QString m_readName;
    quint32 m_properties;
};

// clang-format off
#define LOGIN_PASSWORD      "password"
#define LOGIN_SALT          "salt"
#define LOGIN_READNAME      "readname"
#define LOGIN_PROPERTIES    "properties"

#define USER_INDEX_GROUP    "IndexCount"
#define USER_MAX_COUNT      "CurrentCount"

#define DEFAULT_LOGIN_PROPS 0x0

#define MIN_SIZE_USERNAME   5
// clang-format on

class ListedUser : public ConfigList
{
public:
    ListedUser();
    ~ListedUser();

    int addNewUser(const QString& name, const QString& password = "", quint32 props = DEFAULT_LOGIN_PROPS);
    int showAllUsers();

    bool userCheckPassword(QString name, QString passw);
    bool userCheckPasswordHash(QString name, QString hash, QString random);
    bool userChangePassword(QString name, QString passw);
    bool userChangePasswordHash(QString name, QString passw);
    bool userChangeProperties(QString name, quint32 props);
    bool userChangeReadName(QString name, QString readName);
    quint32 getUserProperties(QString name);
    QString getReadableName(QString name);
    QString getSalt(QString name);

    ConfigItem* getRequestConfigItemFromListIndex(int index)
    {
        Q_UNUSED(index)
        return NULL;
    }

private:
    void saveCurrentInteralList() override;

    bool addNewUserLogin(QString name, qint64 timestamp, quint32 index, QString password, QString salt, quint32 prop, QString readname, bool checkUser = true);
    void addNewUserLogin(QString name, qint64 timestamp, quint32 index, QString password, QString salt, quint32 prop, QString readname, QList<ConfigItem*>* pList);

    QString createHashPassword(const QString passWord, const QString salt);
    QCryptographicHash* m_hash;
};

#endif // LISTEDUSER_H
