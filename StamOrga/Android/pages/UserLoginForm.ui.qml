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

Rectangle {
    id: mainRectangleGame
    property var m_gamePlayItem

    signal clickedCurrentGame(var sender)
    signal pressedAndHoldCurrentGame(var sender)

    width: 340
    height: childrenRect.height * 1.05
    property color gradColorStart: "#101010"
//    property color colorTest: "#f30707"
    property color gradColorStop: "#501050"
    gradient: Gradient {
        GradientStop {
            position: 0
            color: "#ffffff"
        }

        GradientStop {
            position: 0.5
            color: gradColorStop
        }
    }
    radius: 8
    border.color: "#b049f1"
    border.width: 2
    ColumnLayout {
        id: columnLayout
        width: mainRectangleGame.width
        spacing: 1

        RowLayout {
            id: layoutDateLine

            anchors.right: parent.right
            anchors.left: parent.left
            Layout.preferredWidth: columnLayout.width
            Layout.fillWidth: true
            spacing: 5

            Label {
                id: labelLineDate
                text: qsTr("Date")
                leftPadding: 5
                topPadding: 3
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignLeft | Qt.AlignHCenter
            }

            Image {
                id: imageMeetingIsPresent
                anchors.top : parent.top
                Layout.rightMargin: acceptedMeetingItem.visible ? 0 : 5
                anchors.topMargin: 3
                Layout.alignment: Qt.AlignRight
                Layout.preferredWidth: labelLineDate.height / 1.2
                Layout.preferredHeight: labelLineDate.height / 1.2
                source: "../images/place.png";
                visible: false
            }

            RowLayout {
                id: acceptedMeetingItem
                visible: false
                spacing: 0
                Layout.rightMargin: freeTicketsItem.visible ? 10 : 5
                Label {
                    id: labelAcceptedMeeting
                    text: "2"
                    topPadding: 3
                    Layout.alignment: Qt.AlignRight
                }

                Item {
                    Layout.preferredHeight: labelAcceptedMeeting.height / 1.2
                    Layout.preferredWidth: labelAcceptedMeeting.height / 1.2
                    anchors.top : parent.top
                    anchors.right: parent. right

                    anchors.topMargin: 3
                    Layout.alignment: Qt.AlignRight
                    Image {
                        id: imageAccpetedMeeting
                        anchors.fill: parent
                        source: "../images/done.png";
                    }
//                    ColorOverlay {
//                        anchors.fill: imageAccpetedMeeting
//                        source: imageAccpetedMeeting
//                        color: "green"
//                    }
                }
            }

            RowLayout {
                id: freeTicketsItem
                visible: false
                spacing: 4
                Label {
                    id: labelFreeTickets
                    text: "2"
                    topPadding: 3
                    Layout.alignment: Qt.AlignRight
                }

                Rectangle {
                    anchors.top : parent.top
                    anchors.right: parent. right
                    anchors.rightMargin: 5
                    anchors.topMargin: 5
                    Layout.alignment: Qt.AlignRight
                    width: labelFreeTickets.height / 1.5
                    height: labelFreeTickets.height / 1.5
                    radius: width * 0.5
                    color: "green"
                }
            }
        }

        Label {
            id: labelLineWhat
            text: qsTr("What")
            leftPadding: 5
            Layout.alignment: Qt.AlignLeft | Qt.AlignHCenter
        }

        Label {
            id: labelLineHome
            text: qsTr("Home")
            leftPadding: 5
            topPadding: 5
            font.bold: true
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignLeft | Qt.AlignHCenter
        }

        RowLayout {
            id: layoutExplicitGame

            anchors.right: parent.right
            anchors.left: parent.left
            Layout.preferredWidth: columnLayout.width
            Layout.fillWidth: true

            Label {
                id: labelLineAway
                text: qsTr("Away")
                leftPadding: 5
                font.bold: true
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignLeft | Qt.AlignHCenter
            }

            Label {
                id: labelLineScore
                text: qsTr("")
                font.bold: true
                rightPadding: layoutExplicitGame.width / 100 * 30
                Layout.alignment: Qt.AlignRight | Qt.AlignHCenter
            }
        }


    }
}
