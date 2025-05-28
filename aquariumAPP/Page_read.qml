import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

Rectangle{
    id: readPage
    width: 800
    height: 480
    color:"transparent"

    // Blue Wallpaper ---------------------------------------------------------------------
    Image {
        id: wallpaper
        source: "images/blueWpp.jpg"
        fillMode: Image.PreserveAspectCrop
        z: -1   //send image to background
    }

    Rectangle {
        id: reqRead
        width: 450
        height: 120
        x: 175; y: 100
        color: "transparent"
        border.color: "white"
        border.width: 5
        radius: 15

        Text {
            width: parent.width
            height: parent.height
            text: "REQUEST SENSORS\nREADING"
            font.pixelSize: 40
            color: "white"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                aq.requestReading()
                changePage("back")
            }
        }
    }

    Rectangle {
        id: reqRecordings
        width: 450
        height: 120
        x: 175; y: 260
        color: "transparent"
        border.color: "white"
        border.width: 5
        radius: 15

        Text {
            width: parent.width
            height: parent.height
            text: "ANALYZE\nSENSORS DATA"
            font.pixelSize: 40
            color: "white"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                changePage("graph")
            }
        }
    }



    // Return to main ---------------------------------------------------------------
    Image {
        id: back_op
        source: "images/return.png"
        width: 70
        height: 70
        x: 700; y: 380
        fillMode: Image.Stretch
        MouseArea {
            anchors.fill: parent
            onClicked: changePage("back")
        }
    }
}
