import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: addfishconfPage
    width: 800
    height: 480
    color:"transparent"

    property var strListParams: []
    property bool verify

    // Blue Wallpaper ---------------------------------------------------------------------
    Image {
        id: wallpaper
        source: "qrc:/images/blueWpp.jpg"
        fillMode: Image.PreserveAspectCrop
        z: -1   //send image to background
    }

    // Page Title ----------------------------------------------------------------------
    Text {
        y: 25
        width: 800
        height: 80
        text: "CONFIRM NEW FISH ADDITION"
        font.pixelSize: 40
        //font.family: titleFont.font.family
        color: "white"
        horizontalAlignment: Text.AlignHCenter
    }

    Component.onCompleted: {
        var infofish = "INFO " + all.getName()
        SendToDaemon.requestMQ(infofish)
        strListParams = ReadFromDaemon.getFishInfo()
        verify = Ideal.verifyAddition()
        console.log(verify)
        if (verify === true)
            confirm_fish.visible = false
        else if (verify === false)
            confirm_fish.visible = true
    }

    Text {
        x:100; y:120
        width: 800
        height: 80
        text: "Fish to add: " + all.getName() + "\n\n" +
              "Ideal Food Portions: " + strListParams[0] + "\n\n" +
              "Ideal Temperature: " + strListParams[1] + " ºC\n\n" +
              "Ideal pH: " + strListParams[2]
        font.pixelSize: 35
        color: "white"
    }

    Image {
        id: confirm_fish
        source: "qrc:/images/check.png"
        width: 70
        height: 70
        x: 600; y: 380
        fillMode: Image.Stretch
        MouseArea{
            anchors.fill: parent
            onClicked:{
                var addfish = "ADD " + all.getName()
                console.log(addfish)
                SendToDaemon.requestMQ(addfish)
                Ideal.calculateIdeal()
                changePage("addSuccess")
            }
        }
    }


    // Return ---------------------------------------------------------------
    Image {
        id: back_conf
        source: "qrc:/images/return.png"
        width: 70
        height: 70
        x: 700; y: 380
        fillMode: Image.Stretch
        MouseArea {
            anchors.fill: parent
            onClicked: {
                changePage("back")
            }
        }
    }
}
