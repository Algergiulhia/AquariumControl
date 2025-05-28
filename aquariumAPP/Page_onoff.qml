import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: onoffPage
    width: 800
    height: 480
    color:"transparent"

    Image {
        id: wallpaper
        source: "images/blueWpp.jpg"
        fillMode: Image.PreserveAspectCrop
        z: -1   //send image to background
    }

    Text {
        y: 30
        width: 800
        height: 100
        text: "System Control"
        font.pixelSize: 40
        color: "white"
        horizontalAlignment: Text.AlignHCenter
    }

    Switch {
        id: switch_onoff
        x: 260; y:175
        width: 280
        height: 130
        hoverEnabled: true

        onCheckedChanged: aq.shutdown()

        indicator: Rectangle {
            width: 280
            height: 130
            radius: height / 2
            color: switch_onoff.checked ? "black" : "white"

            // smooth color transition
            Behavior on color {
                ColorAnimation {duration: 600}
            }

            Text {
                x: 35
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 40
                color: "white"
                text: "OFF"
            }
            Text {
                x: 170
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 40
                color: "black"
                text: "ON"
            }

            Rectangle {
                x: switch_onoff.checked ? parent.width - width - 2 : 4
                width: parent.height - 6
                height: width
                radius: width / 2
                anchors.verticalCenter: parent.verticalCenter
                color: switch_onoff.checked ? "white" : "black"

                // smooth color and movement transitions
                Behavior on x {
                    NumberAnimation { duration: 600 }
                }
                Behavior on color {
                    ColorAnimation {duration: 600}
                }
            }
        }
    }
    // Return ---------------------------------------------------------------
    Image {
        id: back
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
