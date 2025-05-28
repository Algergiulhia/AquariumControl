import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: optionsPage
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

    // Add Fish ----------------------------------------------------------------------------
    Image {
        id: image1
        source: "images/alivefish.png"
        width: 165
        height: 110
        x: 120 - (image1.width / 2); y: 230 - (image1.height / 2)
        fillMode: Image.Stretch
        MouseArea {
            anchors.fill: parent
            onClicked: changePage("addfish");
        }
    }
    Text {
        x: 120 - (image1.width / 2)
        y: 300
        width: image1.width
        height: 100
        text: "ADD"
        font.pixelSize: 25
        color: "white"
        horizontalAlignment: Text.AlignHCenter
    }

    // Remove Fish ----------------------------------------------------------------------
    Image {
        id: image2
        source: "images/deadfish.png"
        width: 165
        height: 110
        x: 310 - (image2.width / 2); y: 230 - (image2.height / 2)
        fillMode: Image.Stretch
        MouseArea {
            anchors.fill: parent
            onClicked: {
                changePage("removefish");
            }
        }
    }
    Text {
        x: 310 - (image2.width / 2)
        y: 300
        width: image2.width
        height: 100
        text: "REMOVE"
        font.pixelSize: 25
        color: "white"
        horizontalAlignment: Text.AlignHCenter
    }

    // On/Off -----------------------------------------------------------------------
    Image {
        id: image3
        source: "images/gear.png"
        width: 165
        height: 110
        x: 500 - (image3.width / 2); y: 230 - (image3.height/ 2)
        fillMode: Image.PreserveAspectFit
        MouseArea {
            anchors.fill: parent
            onClicked: changePage("settings")
        }
    }
    Text {
        x: 500 - (image3.width / 2)
        y: 300
        width: image3.width
        height: 100
        text: "SETTINGS"
        font.pixelSize: 25
        color: "white"
        horizontalAlignment: Text.AlignHCenter
    }

    // Analysis ------------------------------------------------------------------------
    Image {
        id: image4
        source: "images/read.png"
        width: 165
        height: 110
        x: 680 - (image4.width / 2); y: 230 - (image4.height/ 2)
        fillMode: Image.PreserveAspectFit
        MouseArea {
            anchors.fill: parent
            onClicked: changePage("read");
        }
    }
    Text {
        x: 680 - (image1.width / 2)
        y: 300
        width: image1.width
        height: 100
        text: "ANALYSIS"
        font.pixelSize: 25
        color: "white"
        horizontalAlignment: Text.AlignHCenter
    }

    // Ideal Conditions
    Image {
        id: image5
        source: "images/aquarium.png"
        width: 90
        height: 90
        x: 670; y:20
        fillMode: Image.PreserveAspectFit
        MouseArea {
            anchors.fill: parent
            onClicked: changePage("ideal");
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
