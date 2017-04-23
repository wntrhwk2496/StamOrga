import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.0

import com.watermax.demo 1.0

import "../components" as MyComponents

Flickable {
   id: flickableGames
   property UserInterface userIntGames
   contentHeight: mainPaneGames.height
   Pane {
       id: mainPaneGames
       width: parent.width

       ColumnLayout {
           id: mainColumnLayoutGames
           anchors.right: parent.right
           anchors.left: parent.left
           width: parent.width
           ColumnLayout {
               id: columnLayoutBusyInfo
               spacing: 5
               Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

               BusyIndicator {
                   id: busyLoadingIndicatorGames
                   visible: false
                   Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
               }

               Label {
                   id: txtInfoLoadingGames
                   visible: true
                   Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
               }
           }
           Column {
               id: columnLayoutGames
               anchors.right: parent.right
               anchors.left: parent.left
               width: parent.width
               spacing: 10
               Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
           }
       }
   }


    function toolButtonClicked() {
        busyLoadingIndicatorGames.visible = true
        txtInfoLoadingGames.text = "Lade Spielliste"
        userIntGames.startGettingGamesList();
    }

    function pageOpenedUpdateView() {
        showListedGames()
    }

    function notifyUserIntGamesListFinished(result) {
        busyLoadingIndicatorGames.visible = false
        if (result === 1) {
            showListedGames()
        } else {
            txtInfoLoadingGames.text = userIntGames.getErrorCodeToString(result);
        }
    }

   function showListedGames() {

       for (var j = columnLayoutGames.children.length; j > 0; j--) {
           columnLayoutGames.children[j-1].destroy()
       }

       if (globalUserData.getGamePlayLength() > 0) {
           for (var i=0; i<globalUserData.getGamePlayLength(); i++) {
               var sprite = gameView.createObject(columnLayoutGames)
               sprite.showGamesInfo(globalUserData.getGamePlay(i))
           }
           txtInfoLoadingGames.text = "Letzes Update am " + globalUserData.getGamePlayLastUpdate()
       } else
           txtInfoLoadingGames.text = "Keine Daten zum anzeigen"
   }

   function notifyUserIntVersionRequestFinished(result, msg) {
       if (result === 5) {
           var component = Qt.createComponent("../components/VersionDialog.qml");
           if (component.status === Component.Ready) {
               var dialog = component.createObject(parent,{popupType: 1});
               dialog.versionText = msg;
               dialog.parentHeight = flickableGames.height
               dialog.parentWidth = flickableGames.width
               dialog.open();
           }
       }
   }
   function notifyUserIntConnectionFinished(result) {}

   Component {
       id: gameView

       MyComponents.Games {}
   }

   ScrollIndicator.vertical: ScrollIndicator { }
}