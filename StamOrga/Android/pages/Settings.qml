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

import com.watermax.demo 1.0

Item {
   id: itemSettings

   Component.onDestruction: {
       var saveSettings = false;
       if (globalUserData.lastGamesLoadCount !== spBoxLoadPastGames.value) {
           globalUserData.lastGamesLoadCount = spBoxLoadPastGames.value;
            saveSettings = true;
       }

       if (globalUserData.debugIP !== txtOtherIPAddr.text) {
            globalUserData.debugIP = txtOtherIPAddr.text;
            saveSettings = true;
       }

       if (globalUserData.debugIPWifi !== txtOtherIPAddrWifi.text) {
            globalUserData.debugIPWifi = txtOtherIPAddrWifi.text;
            saveSettings = true;
       }

       if (saveSettings)
           globalUserData.saveGlobalSettings();
   }

   Pane {
       id: mainPaneSettings
       width: parent.width

       ColumnLayout {
           width: parent.width
           spacing: 20

           RowLayout {
//               width: parent.width
               Layout.preferredWidth: parent.width
               Layout.fillWidth: true

               Label {
                   id: text1
                   text: qsTr("Lade beendete Spiele:")
                   Layout.fillWidth: true
                   Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                   font.pixelSize: 14
                   color: "white"
               }
               SpinBox {
                   id: spBoxLoadPastGames
                   to: 50
                   from: 0
                   value: globalUserData.lastGamesLoadCount
                   Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
               }
           }

           RowLayout {
               spacing: 5
               width: parent.width
               Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
               visible: userInt.isDebuggingEnabled()

               Label {
                   text: qsTr("Alternative IP:")
                   Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
               }

               TextField {
                   id: txtOtherIPAddr
                   text: globalUserData.debugIP
                   implicitWidth: parent.width
                   Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                   Layout.fillWidth: true
               }
           }

           RowLayout {
               spacing: 5
               width: parent.width
               Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
               visible: userInt.isDebuggingEnabled() && userInt.isDeviceMobile()

               Label {
                   text: qsTr("Alternative IP Wlan")
                   Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
               }

               TextField {
                   id: txtOtherIPAddrWifi
                   text: globalUserData.debugIPWifi
                   implicitWidth: parent.width
                   Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                   Layout.fillWidth: true

               }
           }
       }
   }

   function pageOpenedUpdateView() {
   }
}
