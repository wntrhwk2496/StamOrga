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
#include <QDebug>
#include <QtCore/QDateTime>

#include "../Common/General/globalfunctions.h"
#include "../Common/General/globaltiming.h"
#include "../Common/Network/messagecommand.h"
#include "../Common/Network/messageprotocol.h"
#include "connectionhandling.h"

#define TIMER_DIFF_MSEC 10 * 1000

ConnectionHandling::ConnectionHandling(QObject* parent)
    : QObject(parent)
{
    this->m_pMainCon             = NULL;
    this->m_lastSuccessTimeStamp = 0;
}

QString mainConRequestPassWord;
QString mainConRequestSalt;
QString mainConRequestRandom;

qint32 ConnectionHandling::startMainConnection(QString name, QString passw)
{
    if (name != this->m_pGlobalData->userName()) {
        this->m_lastSuccessTimeStamp = 0;
        emit this->sSendNewBindingPortRequest();
        QThread::msleep(10);
    }

    if (passw != this->m_pGlobalData->passWord())
        this->stopDataConnection();

    qint64 now = QDateTime::currentMSecsSinceEpoch();
    if (this->isMainConnectionActive()
        && (now - this->m_lastSuccessTimeStamp) < (CON_RESET_TIMEOUT_MSEC - TIMER_DIFF_MSEC)) {

        if (this->m_pGlobalData->bIsConnected()
            && (now - this->m_lastSuccessTimeStamp) < (CON_LOGIN_TIMEOUT_MSEC - TIMER_DIFF_MSEC)) {
            emit this->sNotifyConnectionFinished(ERROR_CODE_SUCCESS);
            qInfo().noquote() << "Did not log in again, should already be succesfull";
            return ERROR_CODE_NO_ERROR;
        }
        this->startDataConnection();
        QThread::msleep(10);
        this->sendLoginRequest(passw);
        return ERROR_CODE_SUCCESS;
    }

    this->stopDataConnection();
    QThread::msleep(20);

    if (this->m_pMainCon == NULL) {
        this->m_pMainCon = new MainConnection(this->m_pGlobalData);
        connect(this->m_pMainCon, &MainConnection::connectionRequestFinished, this, &ConnectionHandling::slMainConReqFin);
        connect(this, &ConnectionHandling::sStartSendMainConRequest, this->m_pMainCon, &MainConnection::slotSendNewMainConRequest);
        connect(this, &ConnectionHandling::sSendNewBindingPortRequest, this->m_pMainCon, &MainConnection::slotNewBindingPortRequest);
        this->m_ctrlMainCon.Start(this->m_pMainCon, false);
        QThread::msleep(20);
    }

    mainConRequestPassWord = passw;
    emit this->sStartSendMainConRequest(name);

    return ERROR_CODE_SUCCESS;
}

qint32 ConnectionHandling::startGettingVersionInfo()
{
    DataConRequest req(OP_CODE_CMD_REQ::REQ_GET_VERSION);
    this->sendNewRequest(req);
    return ERROR_CODE_SUCCESS;
}

qint32 ConnectionHandling::startGettingUserProps()
{
    DataConRequest req(OP_CODE_CMD_REQ::REQ_GET_USER_PROPS);
    this->sendNewRequest(req);
    return ERROR_CODE_SUCCESS;
}

bool ConnectionHandling::startUpdatePassword(QString newPassWord)
{
    DataConRequest req(OP_CODE_CMD_REQ::REQ_USER_CHANGE_LOGIN);
    req.m_lData.append(newPassWord);
    this->sendNewRequest(req);
    return true;
}

qint32 ConnectionHandling::startUpdateReadableName(QString name)
{
    DataConRequest req(OP_CODE_CMD_REQ::REQ_USER_CHANGE_READNAME);
    req.m_lData.append(name);
    this->sendNewRequest(req);
    return ERROR_CODE_SUCCESS;
}

qint32 ConnectionHandling::startListGettingGames()
{
    DataConRequest req(OP_CODE_CMD_REQ::REQ_GET_GAMES_LIST);
    this->sendNewRequest(req);
    return ERROR_CODE_SUCCESS;
}

qint32 ConnectionHandling::startListGettingGamesInfo()
{
    DataConRequest req(OP_CODE_CMD_REQ::REQ_GET_GAMES_INFO_LIST);
    this->sendNewRequest(req);
    return ERROR_CODE_SUCCESS;
}

qint32 ConnectionHandling::startSetFixedGameTime(const quint32 gameIndex, const quint32 fixedTime)
{
    DataConRequest req(OP_CODE_CMD_REQ::REQ_SET_FIXED_GAME_TIME);
    req.m_lData.append(QString::number(gameIndex));
    req.m_lData.append(QString::number(fixedTime));
    this->sendNewRequest(req);
    return ERROR_CODE_SUCCESS;
}

qint32 ConnectionHandling::startRemoveSeasonTicket(quint32 index)
{
    DataConRequest req(OP_CODE_CMD_REQ::REQ_REMOVE_TICKET);
    req.m_lData.append(QString::number(index));
    this->sendNewRequest(req);
    return ERROR_CODE_SUCCESS;
}

qint32 ConnectionHandling::startEditSeasonTicket(quint32 index, QString name, QString place, quint32 discount)
{
    DataConRequest req(OP_CODE_CMD_REQ::REQ_CHANGE_TICKET);
    req.m_lData.append(QString::number(index));
    req.m_lData.append(name);
    req.m_lData.append(place);
    req.m_lData.append(QString::number(discount));
    this->sendNewRequest(req);
    return ERROR_CODE_SUCCESS;
}

qint32 ConnectionHandling::startAddSeasonTicket(QString name, quint32 discount)
{
    DataConRequest req(OP_CODE_CMD_REQ::REQ_ADD_TICKET);
    req.m_lData.append(QString::number(discount));
    req.m_lData.append(name);
    this->sendNewRequest(req);
    return ERROR_CODE_SUCCESS;
}

qint32 ConnectionHandling::startListSeasonTickets()
{
    DataConRequest req(OP_CODE_CMD_REQ::REQ_GET_TICKETS_LIST);
    this->sendNewRequest(req);
    return ERROR_CODE_SUCCESS;
}

qint32 ConnectionHandling::startChangeSeasonTicketState(quint32 tickedIndex, quint32 gameIndex, quint32 state, QString name)
{
    DataConRequest req(OP_CODE_CMD_REQ::REQ_STATE_CHANGE_SEASON_TICKET);
    req.m_lData.append(QString::number(tickedIndex));
    req.m_lData.append(QString::number(gameIndex));
    req.m_lData.append(QString::number(state));
    req.m_lData.append(name);
    this->sendNewRequest(req);
    return ERROR_CODE_SUCCESS;
}

qint32 ConnectionHandling::startListAvailableTicket(quint32 gameIndex)
{
    /* check if update was more than one our ago, than update items */
    //    qint64 oneHourAgo = QDateTime::currentDateTime().addSecs(-(60 * 60)).toMSecsSinceEpoch();
    //    if (oneHourAgo > this->m_pGlobalData->getSeasonTicketLastLocalUpdate())
    //        return this->startListSeasonTickets();

    DataConRequest req(OP_CODE_CMD_REQ::REQ_GET_AVAILABLE_TICKETS);
    req.m_lData.append(QString::number(gameIndex));
    this->sendNewRequest(req);
    return ERROR_CODE_SUCCESS;
}

qint32 ConnectionHandling::startChangeGame(const quint32 index, const quint32 sIndex,
                                           const QString competition, const QString home,
                                           const QString away, const QString date,
                                           const QString score)
{
    if (sIndex > 34 || competition == "" || home == "" || away == "" || date == "") {
        qWarning().noquote() << "Standart parameter for changing game are wrong";
        return ERROR_CODE_WRONG_PARAMETER;
    }
    if (home.contains(";") || away.contains(";") || date.contains(";")) {
        qWarning().noquote() << "String parameter for changing game contain semicolons";
        return ERROR_CODE_WRONG_PARAMETER;
    }

    CompetitionIndex compIndex = getCompetitionIndex(competition);
    if (compIndex == NO_COMPETITION)
        return ERROR_CODE_WRONG_PARAMETER;

    QString        val = QString("%1;%2;%3;%4;%5;%6;%7").arg(home, away, date, score).arg(index).arg(sIndex).arg(compIndex);
    DataConRequest req(OP_CODE_CMD_REQ::REQ_CHANGE_GAME);
    req.m_lData.append(val);
    this->sendNewRequest(req);
    return ERROR_CODE_SUCCESS;
}

qint32 ConnectionHandling::startSaveMeetingInfo(const quint32 gameIndex, const QString when, const QString where, const QString info)
{
    MeetingInfo* pInfo = this->m_pGlobalData->getMeetingInfo();

    if (pInfo->when() != when || pInfo->where() != where || pInfo->info() != info) {
        DataConRequest req(OP_CODE_CMD_REQ::REQ_CHANGE_MEETING_INFO);
        req.m_lData.append(QString::number(gameIndex));
        req.m_lData.append(when);
        req.m_lData.append(where);
        req.m_lData.append(info);
        this->sendNewRequest(req);

        return ERROR_CODE_SUCCESS;
    }
    return ERROR_CODE_NO_ERROR;
}

qint32 ConnectionHandling::startLoadMeetingInfo(const quint32 gameIndex)
{
    DataConRequest req(OP_CODE_CMD_REQ::REQ_GET_MEETING_INFO);
    req.m_lData.append(QString::number(gameIndex));
    this->sendNewRequest(req);

    return ERROR_CODE_SUCCESS;
}

qint32 ConnectionHandling::startAcceptMeetingInfo(const quint32 gameIndex, const quint32 accept, const QString name, const quint32 acceptIndex)
{
    DataConRequest req(OP_CODE_CMD_REQ::REQ_ACCEPT_MEETING);
    req.m_lData.append(QString::number(gameIndex));
    req.m_lData.append(QString::number(accept));
    req.m_lData.append(QString::number(acceptIndex));
    req.m_lData.append(name);
    this->sendNewRequest(req);

    return ERROR_CODE_SUCCESS;
}

/*
 * Answer function after connection with username
 */
void ConnectionHandling::slMainConReqFin(qint32 result, const QString msg, const QString salt, const QString random)
{
    if (result > ERROR_CODE_NO_ERROR) {
        qInfo().noquote() << QString("Trying login request with port %1").arg(result);
        this->m_pGlobalData->setConDataPort((quint16)result);
        this->startDataConnection();
        QThread::msleep(20);
        mainConRequestSalt   = salt;
        mainConRequestRandom = random;
        this->sendLoginRequest(mainConRequestPassWord);
        this->m_lastSuccessTimeStamp = QDateTime::currentMSecsSinceEpoch();
    } else {
        qWarning().noquote() << QString("Error main connecting: %1").arg(msg);
        this->m_ctrlMainCon.Stop();
        this->m_pMainCon = NULL;
        this->stopDataConnection();
        emit this->sNotifyConnectionFinished(result);

        while (this->m_lErrorMainCon.size() > 0) {
            DataConRequest request = this->m_lErrorMainCon.last();
            request.m_result       = result;
            this->slDataConLastRequestFinished(request);
            this->m_lErrorMainCon.removeLast();
        }
    }
}

/*
 * Functions for login
 */
void ConnectionHandling::sendLoginRequest(QString password)
{
    this->startDataConnection(); // call it every time, if it is already started it just returns

    DataConRequest req;
    req.m_request = OP_CODE_CMD_REQ::REQ_LOGIN_USER;
    req.m_lData.append(password);
    req.m_lData.append(mainConRequestSalt);

    emit this->sStartSendNewRequest(req);
}

void ConnectionHandling::sendNewRequest(DataConRequest request)
{
    qint64 now = QDateTime::currentMSecsSinceEpoch();
    if ((now - this->m_lastSuccessTimeStamp) < (CON_RESET_TIMEOUT_MSEC - TIMER_DIFF_MSEC)) {
        this->startDataConnection();

        if ((now - this->m_lastSuccessTimeStamp) < (CON_LOGIN_TIMEOUT_MSEC - TIMER_DIFF_MSEC)) {
            emit this->sStartSendNewRequest(request);
            return;
        } else {
            this->m_lErrorMainCon.prepend(request);
            if (this->m_lErrorMainCon.size() == 1) {
                qInfo().noquote() << QString("Trying to reconnect from ConnectionHandling");
                this->m_pGlobalData->setbIsConnected(false);
                this->sendLoginRequest(this->m_pGlobalData->passWord());
            }
        }
    } else {
        this->m_lErrorMainCon.prepend(request);
        if (this->m_lErrorMainCon.size() == 1) {
            qInfo().noquote() << QString("Trying to restart connection from ConnectionHandling");
            this->startMainConnection(this->m_pGlobalData->userName(), this->m_pGlobalData->passWord());
        }
    }
}


void ConnectionHandling::slDataConLastRequestFinished(DataConRequest request)
{
    this->checkTimeoutResult(request.m_result);

    switch (request.m_request) {
    case OP_CODE_CMD_REQ::REQ_LOGIN_USER:
        if (request.m_result == ERROR_CODE_SUCCESS) {
            this->startGettingVersionInfo();
            this->startGettingUserProps();
            QThread::msleep(10);
            this->m_pGlobalData->setbIsConnected(true);
            this->checkTimeoutResult(request.m_result); // call again to set last successfull timer
            while (this->m_lErrorMainCon.size() > 0) {
                this->sendNewRequest(this->m_lErrorMainCon.last());
                this->m_lErrorMainCon.removeLast();
            }
        } else {
            qWarning().noquote() << QString("Error Login: %1").arg(getErrorCodeString(request.m_result));
            while (this->m_lErrorMainCon.size() > 0) {
                DataConRequest newReq = this->m_lErrorMainCon.last();
                newReq.m_result       = request.m_result;
                this->slDataConLastRequestFinished(newReq);
                this->m_lErrorMainCon.removeLast();
            }
        }

        emit this->sNotifyConnectionFinished(request.m_result);
        break;

    case OP_CODE_CMD_REQ::REQ_GET_VERSION:
        emit this->sNotifyVersionRequest(request.m_result, request.m_returnData);
        break;

    case OP_CODE_CMD_REQ::REQ_USER_CHANGE_LOGIN:
        if (request.m_result == ERROR_CODE_SUCCESS) {
            this->m_pGlobalData->setPassWord(request.m_returnData);
            this->m_pGlobalData->saveGlobalUserSettings();
        }

        emit this->sNotifyUpdatePasswordRequest(request.m_result, request.m_returnData);
        break;

    case OP_CODE_CMD_REQ::REQ_USER_CHANGE_READNAME:
        if (request.m_result == ERROR_CODE_SUCCESS) {
            this->m_pGlobalData->setReadableName(request.m_returnData);
            this->m_pGlobalData->saveGlobalUserSettings();
        }

        emit this->sNotifyCommandFinished(request.m_request, request.m_result);
        break;

    case OP_CODE_CMD_REQ::REQ_GET_GAMES_INFO_LIST: {

        static quint32 retryGetGamesInfoCount = 0;
        if (request.m_result == ERROR_CODE_UPDATE_LIST) {
            if (retryGetGamesInfoCount < 3) {
                this->startListGettingGames();
                retryGetGamesInfoCount++;
                return;
            }
        }
        emit this->sNotifyCommandFinished(request.m_request, request.m_result);
        retryGetGamesInfoCount = 0;
        break;
    }

    case OP_CODE_CMD_REQ::REQ_GET_AVAILABLE_TICKETS: {

        static quint32 retryGetTicketCount = 0;
        if (request.m_result == ERROR_CODE_UPDATE_LIST) {
            if (retryGetTicketCount < 3) {
                this->startListSeasonTickets();
                retryGetTicketCount++;
                return;
            }
        }
        emit this->sNotifyCommandFinished(request.m_request, request.m_result);
        retryGetTicketCount = 0;
        break;
    }
    case OP_CODE_CMD_REQ::REQ_GET_MEETING_INFO:
        emit this->sNotifyCommandFinished(request.m_request, request.m_result);
        if (request.m_result == ERROR_CODE_NOT_FOUND)
            return;
        break;


    default:
        emit this->sNotifyCommandFinished(request.m_request, request.m_result);
        break;
    }

    if (request.m_result != ERROR_CODE_SUCCESS)
        qWarning().noquote() << QString("Error receiving Command 0x%1 has result \"%2\"")
                                    .arg(QString::number(request.m_request, 16))
                                    .arg(getErrorCodeString(request.m_result));
}

void ConnectionHandling::checkTimeoutResult(qint32 result)
{
    if (result == ERROR_CODE_TIMEOUT) {
        this->stopDataConnection();
        this->m_lastSuccessTimeStamp = 0;
        emit this->sSendNewBindingPortRequest();
    } else if (this->m_pGlobalData->bIsConnected())
        this->m_lastSuccessTimeStamp = QDateTime::currentMSecsSinceEpoch();
}

void ConnectionHandling::startDataConnection()
{
    if (this->isDataConnectionActive()) {
        this->m_pDataCon->setRandomLoginValue(mainConRequestRandom);
        return;
    }

    this->m_pDataCon = new DataConnection(this->m_pGlobalData);

    connect(this, &ConnectionHandling::sStartSendNewRequest,
            this->m_pDataCon, &DataConnection::startSendNewRequest);
    connect(this->m_pDataCon, &DataConnection::notifyLastRequestFinished,
            this, &ConnectionHandling::slDataConLastRequestFinished);

    this->m_ctrlDataCon.Start(this->m_pDataCon, false);
}

void ConnectionHandling::stopDataConnection()
{
    this->m_pGlobalData->setbIsConnected(false);

    if (!this->isDataConnectionActive())
        return;

    disconnect(this, &ConnectionHandling::sStartSendNewRequest,
               this->m_pDataCon, &DataConnection::startSendNewRequest);
    disconnect(this->m_pDataCon, &DataConnection::notifyLastRequestFinished,
               this, &ConnectionHandling::slDataConLastRequestFinished);

    this->m_ctrlDataCon.Stop();
}


ConnectionHandling::~ConnectionHandling()
{
    if (this->isDataConnectionActive())
        this->stopDataConnection();

    if (this->isMainConnectionActive())
        this->m_ctrlMainCon.Stop();
}
