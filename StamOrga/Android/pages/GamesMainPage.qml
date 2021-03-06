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
import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.0
//import QtGraphicalEffects 1.0

import com.watermax.demo 1.0

import "../pages" as MyPages
import "../components" as MyComponents

Page {
    id: mainItemGamesMainPage
    width: parent.width
    height: parent.height
    property UserInterface userIntGames

    GridLayout {
        id: mainColumnGamesMainPage
        anchors.fill: parent
        Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
        flow: GridLayout.TopToBottom
        rows: 3
        columns: 2

        ColumnLayout {
            id: columnLayoutBusyInfo
            spacing: 5
            Layout.topMargin: 10
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

            BusyIndicator {
                id: busyLoadingIndicatorGames
                visible: false
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            }

            Label {
                id: txtInfoLoadingGames
                visible: true
                horizontalAlignment: Text.AlignHCenter
                Layout.alignment:  Qt.AlignHCenter | Qt.AlignVCenter
            }

        }

        TabBar {
                id: tabBarGamesMainPage
                anchors.left: parent.left
                anchors.right: parent.right
                Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
                currentIndex: swipeViewGamesMainPage.currentIndex


                TabButton {
                    text: "Aktuell"
                }
                TabButton {
                    text: "Vergangenheit"
                }
        }

        SwipeView {
            id: swipeViewGamesMainPage
            anchors.top : tabBarGamesMainPage.bottom
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
            Layout.fillHeight: true
            currentIndex: tabBarGamesMainPage.currentIndex
            Layout.columnSpan: 2

            MyPages.GamesListPage {
                id: gamesListPagePresent
                showOnlyPastGames : false
                onShowInfoHeader: showLoadingGameInfos(text);
                onAcceptedChangeGame: startEditGame(dialog);
                onDragStarted: {
                    movedInfoIndex = 1; movedStartY = contentY;
                    movedStartMargin = columnLayoutBusyInfo.Layout.topMargin > 0 ? 10 : -movedInfoHeigth;
                }
                onDragEnded: { movedInfoIndex = 0; checkMovedInfoEnd(movedStartY - contentY); }
                onContentYChanged : checkMovedInfo(1, movedStartY - contentY);

            }
            MyPages.GamesListPage {
                id: gamesListPagePast
                showOnlyPastGames : true
                onShowInfoHeader: showLoadingGameInfos(text);
                onAcceptedChangeGame: startEditGame(dialog);
                onDragStarted: {
                    movedInfoIndex = 2; movedStartY = contentY;
                    movedStartMargin = columnLayoutBusyInfo.Layout.topMargin > 0 ? 10 : -movedInfoHeigth;
                }
                onDragEnded: { movedInfoIndex = 0; checkMovedInfoEnd(movedStartY - contentY); }
                onContentYChanged : checkMovedInfo(2, movedStartY - contentY);
            }
        }
    }

    NumberAnimation {
        id: animateMoveInfoUp
        target: columnLayoutBusyInfo
        property: "Layout.topMargin"
        to: -movedInfoHeigth
        duration: 250
    }
    NumberAnimation {
        id: animateMoveInfoDown
        target: columnLayoutBusyInfo
        property: "Layout.topMargin"
        to: 10
        duration: 250
    }

    function checkMovedInfo(index, diff) {
        if (movedInfoIndex !== index)
            return;

        if (movedStartMargin === 10) {
            if (diff < 0)
                columnLayoutBusyInfo.Layout.topMargin = Math.max(-movedInfoHeigth, (diff * 0.5) + 10);
        } else if (movedStartMargin === -movedInfoHeigth) {

            if (diff > 0)
                columnLayoutBusyInfo.Layout.topMargin = Math.min(10, -movedInfoHeigth + (diff * 0.5));
        }
    }

    function checkMovedInfoEnd(diff){
        if (movedStartMargin === 10) {
            if (diff < -10)
                animateMoveInfoUp.start()
            else
                animateMoveInfoDown.start()
        } else if (movedStartMargin === -movedInfoHeigth){
            if (diff > 10)
                animateMoveInfoDown.start()
            else
                animateMoveInfoUp.start()
        }
    }

    property int movedInfoIndex : 0
    property int movedStartY : 0
    property int movedStartMargin : 10
    property int movedInfoHeigth : txtInfoLoadingGames.height

    property var addGameDialog;

    function toolButtonClicked() {

        if (globalUserData.userIsGameAddingEnabled() || userIntGames.isDebuggingEnabled()) {
            var component = Qt.createComponent("../components/ChangeGameDialog.qml");
            if (component.status === Component.Ready) {
                var dialog = component.createObject(mainItemGamesMainPage,{popupType: 1});
                dialog.headerText = "Neues Spiel";
                dialog.parentHeight = mainItemGamesMainPage.height
                dialog.parentWidth = mainItemGamesMainPage.width
                dialog.homeTeam = "";
                dialog.awayTeam = "";
                dialog.score = "";
                dialog.seasonIndex = 1;
                dialog.competitionIndex = 2;
                dialog.date = "";
                dialog.index = 0;
                dialog.acceptedDialog.connect(acceptedAddGameDialog);
                addGameDialog = dialog
                dialog.open();
            }
        }
    }

    function acceptedAddGameDialog(){
        startEditGame(addGameDialog)
    }

    function startEditGame(dialog) {
        var result = userIntGames.startChangeGame(dialog.index, dialog.seasonIndex,
                                                  dialog.competition, dialog.homeTeam.trim(),
                                                  dialog.awayTeam.trim(), dialog.date.trim(),
                                                  dialog.score.trim());
        if (result !== 1) {
            toastManager.show(userIntGames.getErrorCodeToString(result), 5000)
            busyLoadingIndicatorGames.visible = true
            if (changeGameDialog.index === 0)
                txtInfoLoadingGames.text = "Füge Spiel hinzu"
            else
                txtInfoLoadingGames.text = "Ändere Spiel"
        }
    }

    function notifyUserIntConnectionFinished(result) {}

    function notifyUserIntGamesListFinished(result) {
        if (result !== 1) {
            toastManager.show(userIntGames.getErrorCodeToString(result), 5000)
            busyLoadingIndicatorGames.visible = false
            showListedGames()
        }
    }

    function notifyUserIntGamesInfoListFinished(result) {
        busyLoadingIndicatorGames.visible = false
        if (result === 1)
            toastManager.show("Spiele geladen", 2000)
        else
            toastManager.show(userIntGames.getErrorCodeToString(result), 5000)

        showListedGames()
    }

    function notifyGameChangedFinished(result) {
        if (result === 1) {
            showLoadingGameInfos("Lade Spielinfos")
            userIntGames.startListGettingGames()
            gamesListPagePresent.cleanGameLayout();
            gamesListPagePast.cleanGameLayout();
        }
        else
            toastManager.show(userIntGames.getErrorCodeToString(result), 5000)
    }

    function notifySetGamesFixedTimeFinished(result) {
        if (result === 1) {
            showLoadingGameInfos("Lade Spielinfos")
            userIntGames.startListGettingGames()
            gamesListPagePresent.cleanGameLayout();
            gamesListPagePast.cleanGameLayout();
        }
        else
            toastManager.show(userIntGames.getErrorCodeToString(result), 5000)
    }

    function pageOpenedUpdateView() {

        if (globalUserData.userIsGameAddingEnabled() || userIntGames.isDebuggingEnabled())
            updateHeaderFromMain("StamOrga", "images/add.png")
        else
            updateHeaderFromMain("StamOrga", "")
        showListedGames()
    }

    function showListedGames() {
        gamesListPagePresent.showListedGames()
        gamesListPagePast.showListedGames()

        if (globalUserData.getGamePlayLength() > 0) {
            txtInfoLoadingGames.text = "Letztes Update am " + globalUserData.getGamePlayLastUpdateString()
        } else
            txtInfoLoadingGames.text = "Keine Daten gespeichert\nZiehen zum Aktualisieren"
    }

    function showLoadingGameInfos(text)
    {
        busyLoadingIndicatorGames.visible = true
        txtInfoLoadingGames.text = text
    }

}
