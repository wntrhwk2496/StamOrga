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

#ifndef GLOBALDATA_H
#define GLOBALDATA_H

#include <QtCore/QList>
#include <QtCore/QMutex>
#include <QtCore/QObject>
#include <QtCore/QSettings>
#include <QtNetwork/QHostInfo>

#include "../../Common/General/backgroundcontroller.h"
#include "../../Common/General/globalfunctions.h"
#include "../../Common/General/logging.h"
#include "gameplay.h"
#include "meetinginfo.h"
#include "seasonticket.h"


#define USER_IS_ENABLED(val) ((this->m_UserProperties & val) > 0 ? true : false)

class GlobalData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString userName READ userName WRITE setUserName NOTIFY userNameChanged)
    Q_PROPERTY(QString passWord READ passWord WRITE setPassWord NOTIFY passWordChanged)
    Q_PROPERTY(QString readableName READ readableName WRITE setReadableName NOTIFY readableNameChanged)
    Q_PROPERTY(QString ipAddr READ ipAddr WRITE setIpAddr NOTIFY ipAddrChanged)
    Q_PROPERTY(quint32 conMasterPort READ conMasterPort WRITE setConMasterPort NOTIFY conMasterPortChanged)
    Q_PROPERTY(bool bIsConnected READ bIsConnected WRITE setbIsConnected NOTIFY bIsConnectedChanged)

    friend class GlobalSettings;

public:
    explicit GlobalData(QObject* parent = 0);

    void loadGlobalSettings();

    QString userName()
    {
        QMutexLocker lock(&this->m_mutexUser);
        return this->m_userName;
    }
    void setUserName(const QString& user)
    {
        if (this->m_userName != user) {
            {
                QMutexLocker lock(&this->m_mutexUser);
                this->m_userName = user;
            }
            emit userNameChanged();
        }
    }

    QString getSalt() { return this->m_salt; }
    void setSalt(QString salt) { this->m_salt = salt; }

    QString readableName()
    {
        QMutexLocker lock(&this->m_mutexUser);
        return this->m_readableName;
    }
    void setReadableName(const QString& name)
    {
        if (this->m_readableName != name) {
            {
                QMutexLocker lock(&this->m_mutexUser);
                this->m_readableName = name;
            }
            emit readableNameChanged();
        }
    }

    QString passWord()
    {
        QMutexLocker lock(&this->m_mutexUser);
        return this->m_passWord;
    }
    void setPassWord(const QString& passw)
    {
        if (this->m_passWord != passw) {
            {
                QMutexLocker lock(&this->m_mutexUser);
                this->m_passWord = passw;
            }
            emit passWordChanged();
        }
    }

    QString ipAddr()
    {
        QMutexLocker lock(&this->m_mutexUser);
        return this->m_ipAddress;
    }
    void setIpAddr(const QString& ip)
    {
        if (this->m_ipAddress != ip) {
            {
                QMutexLocker lock(&this->m_mutexUser);
                this->m_ipAddress = ip;
            }
            emit ipAddrChanged();
        }
    }

    quint32 conMasterPort()
    {
        QMutexLocker lock(&this->m_mutexUser);
        return this->m_uMasterPort;
    }
    void setConMasterPort(quint32 port)
    {
        if (this->m_uMasterPort != port) {
            {
                QMutexLocker lock(&this->m_mutexUser);
                this->m_uMasterPort = port;
            }
            emit conMasterPortChanged();
        }
    }

    quint32 conDataPort()
    {
        QMutexLocker lock(&this->m_mutexUser);
        return this->m_uDataPort;
    }
    void setConDataPort(quint32 port)
    {
        QMutexLocker lock(&this->m_mutexUser);
        if (this->m_uDataPort != port) {
            this->m_uDataPort = port;
        }
    }

    quint32 userIndex()
    {
        QMutexLocker lock(&this->m_mutexUser);
        return this->m_userIndex;
    }
    void setUserIndex(quint32 userIndex)
    {
        QMutexLocker lock(&this->m_mutexUser);
        if (this->m_userIndex != userIndex) {
            this->m_userIndex = userIndex;
        }
    }

    bool bIsConnected() { return this->m_bIsConnected; }
    void setbIsConnected(bool enable)
    {
        if (this->m_bIsConnected != enable) {
            this->m_bIsConnected = enable;
            emit this->bIsConnectedChanged();
        }
    }

    Q_INVOKABLE bool userIsDebugEnabled();
    Q_INVOKABLE bool userIsGameAddingEnabled();
    Q_INVOKABLE bool userIsGameFixedTimeEnabled();
    void SetUserProperties(quint32 value);

    void saveGlobalUserSettings();

    Q_INVOKABLE QString getCurrentLoggingList(int index)
    {
        return this->m_logApp->getCurrentLoggingList(index);
    }

    Q_INVOKABLE QStringList getCurrentLogFileList()
    {
        return this->m_logApp->getLogFileDates();
    }

    Q_INVOKABLE void copyTextToClipBoard(QString text);

    void saveCurrentGamesList(qint64 timestamp);

    void startUpdateGamesPlay(const qint16 updateIndex);
    void addNewGamePlay(GamePlay* gPlay, const qint16 updateIndex = 0);
    Q_INVOKABLE quint32 getGamePlayLength()
    {
        QMutexLocker lock(&this->m_mutexGame);
        return this->m_lGamePlay.size();
    }
    GamePlay* getGamePlay(const quint32 gameIndex);
    Q_INVOKABLE GamePlay* getGamePlayFromArrayIndex(int index);
    Q_INVOKABLE QString getGamePlayLastUpdateString();
    qint64              getGamePlayLastLocalUpdate();
    qint64              getGamePlayLastServerUpdate();


    void saveCurrentSeasonTickets(qint64 timestamp);

    void startUpdateSeasonTickets(const quint16 updateIndex);
    void addNewSeasonTicket(SeasonTicketItem* sTicket, const quint16 updateIndex = 0);
    Q_INVOKABLE quint32 getSeasonTicketLength()
    {
        QMutexLocker lock(&this->m_mutexTicket);
        return this->m_lSeasonTicket.size();
    }
    Q_INVOKABLE SeasonTicketItem* getSeasonTicketFromArrayIndex(int index);
    SeasonTicketItem* getSeasonTicket(quint32 ticketIndex);
    Q_INVOKABLE QString getSeasonTicketLastLocalUpdateString();
    qint64              getSeasonTicketLastLocalUpdate();
    qint64              getSeasonTicketLastServerUpdate();

    Q_INVOKABLE MeetingInfo* getMeetingInfo() { return &this->m_meetingInfo; }

signals:
    void
    userNameChanged();
    void passWordChanged();
    void readableNameChanged();
    void ipAddrChanged();
    void conMasterPortChanged();
    void bIsConnectedChanged();


public slots:

private slots:
    void callBackLookUpHost(const QHostInfo& host);

private:
    QString m_userName;
    QString m_passWord;
    QString m_salt;
    QString m_readableName;
    QString m_ipAddress;
    quint32 m_uMasterPort;
    quint16 m_uDataPort;
    quint32 m_userIndex;

    quint32 m_UserProperties;

    QMutex m_mutexUser;
    QMutex m_mutexUserIni;
    QMutex m_mutexGame;
    QMutex m_mutexTicket;

    bool m_bIsConnected;

    QSettings* m_pMainUserSettings;

    QList<GamePlay*> m_lGamePlay;
    qint64           m_gpLastLocalUpdateTimeStamp;
    qint64           m_gpLastServerUpdateTimeStamp;
    bool             m_bGamePlayLastUpdateDidChanges;

    QList<SeasonTicketItem*> m_lSeasonTicket;
    qint64                   m_stLastLocalUpdateTimeStamp;
    qint64                   m_stLastServerUpdateTimeStamp;
    bool                     m_bSeasonTicketLastUpdateDidChanges;

    MeetingInfo m_meetingInfo;

    Logging*             m_logApp;
    BackgroundController m_ctrlLog;
};

#endif // GLOBALDATA_H
