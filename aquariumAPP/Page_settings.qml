import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: settingsPage
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

    // Remove Fish ----------------------------------------------------------------------
    Image {
        id: imageClock
        source: "images/clock.png"
        width: 180
        height: 180
        x: 270 - (imageClock.width / 2); y: 230 - (imageClock.height / 2)
        fillMode: Image.PreserveAspectFit
        MouseArea {
            anchors.fill: parent
            onClicked: {
                changePage("change");
            }
        }
    }
    Text {
        x: 270 - (imageClock.width / 2)
        y: 320
        width: imageClock.width
        height: 100
        text: "TIME\nSETTINGS"
        font.pixelSize: 30
        color: "white"
        horizontalAlignment: Text.AlignHCenter
    }

    // On/Off -----------------------------------------------------------------------
    Image {
        id: imageOnOff
        source: "images/playpause.png"
        width: 165
        height: 150
        x: 540 - (imageOnOff.width / 2); y: 230 - (imageOnOff.height/ 2)
        fillMode: Image.PreserveAspectFit
        MouseArea {
            anchors.fill: parent
            onClicked: changePage("onoff")
        }
    }
    Text {
        x: 540 - (imageOnOff.width / 2)
        y: 320
        width: imageOnOff.width
        height: 100
        text: "SYSTEM\nCONTROL"
        font.pixelSize: 30
        color: "white"
        horizontalAlignment: Text.AlignHCenter
    }

    // Return to ---------------------------------------------------------------
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
