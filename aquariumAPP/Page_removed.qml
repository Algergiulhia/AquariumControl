import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: addfishconfPage
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

    // Page Title ----------------------------------------------------------------------
    Text {
        y: 150
        width: 800
        height: 100
        text: "'" + aquarium.getName() + "' FISH\nSUCCESSFULLY REMOVED\n FROM THE AQUARIUM"
        font.pixelSize: 40
        color: "white"
        horizontalAlignment: Text.AlignHCenter
    }

    // Return to main ---------------------------------------------------------------
    Image {
        id: back_to_main
        source: "images/return.png"
        width: 70
        height: 70
        x: 700; y: 380
        fillMode: Image.Stretch
        MouseArea {
            anchors.fill: parent
            onClicked: {
                changePage("addDone")
            }
        }
    }
}
