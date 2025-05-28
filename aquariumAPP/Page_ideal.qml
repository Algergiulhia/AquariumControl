import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: addfishconfPage
    width: 800
    height: 480
    color:"transparent"

    property var temp
    property var ph
    property var tds
    property var food
    property int light
    property int feed1
    property int feed2

    // Blue Wallpaper ---------------------------------------------------------------------
    Image {
        id: wallpaper
        source: "images/blueWpp.jpg"
        fillMode: Image.PreserveAspectCrop
        z: -1   //send image to background
    }

    // Page Title ----------------------------------------------------------------------
    Text {
        y: 50
        width: 800
        height: 100
        text: "AQUARIUM IDEAL CONDITIONS"
        font.pixelSize: 40
        color: "white"
        horizontalAlignment: Text.AlignHCenter
    }

    Component.onCompleted: {
        Ideal.calculate
        temp = Ideal.getTemp()
        ph = Ideal.getPh()
        tds = Ideal.getTDS()
        food = Ideal.getFood()
        light = Ideal.getLightTime() / 60
        feed1 = Ideal.getFeedTime(0) / 60
        feed2 = Ideal.getFeedTime(1) / 60
    }

    Text {
        x: 90
        y: 120
        width:700
        height: 300
        text: "Temperature: " + "\n" +
              "pH: " + "\n" +
              "TDS: " + "\n" +
              "Food Quantity: " + "\n" +
              "Lighting Time: " + "\n" +
              "Feeding Time: "
        font.pixelSize: 32
        lineHeight: 1.3
        color: "white"
    }
    Text {
        y: 120
        width:710
        height: 300
        color: "white"
        text: parseFloat(temp).toFixed(2) + " ºC\n" +
              parseFloat(ph).toFixed(2) + "\n" +
              tds + " ppm\n" +
              food + " portions\n" +
              light + "h" + Ideal.getLightTime() % 60 + "\n" +
              feed1 + "h" + Ideal.getFeedTime(0) % 60 +
              " & " + feed2 + "h" + Ideal.getFeedTime(1) % 60
        font.pixelSize: 32
        lineHeight: 1.3
        horizontalAlignment: Text.AlignRight
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
            onClicked: {
                changePage("back")
            }
        }
    }
}
