import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: mainPage
    width: 800
    height: 480
    color:"transparent"

    // Fish Wallpaper ---------------------------------------------------------------------
    Image {
        id: mainWallpaper
        source: "/images/fish.jpeg"
        fillMode: Image.PreserveAspectCrop
        z: -1   //send image to background
    }

    // Title ---------------------------------------------------------------------------
    Text {
        id: title
        width: 800
        height: 100
        y: 50
        text: "AQUARIUM CONTROL"
        font.pixelSize: 45
        color: "white"
        horizontalAlignment: Text.AlignHCenter
    }

    property var error
    property var readValues: []

    Component.onCompleted: {
        aq.requestReading()
        readValues = aq.requestValues()
    }

    Timer {
        id: timer
        interval: 5000     // 10 seconds
        running: true
        repeat: true

        onTriggered: {
            readValues = aq.requestValues()
            error = aq.verifyError()
            checkError(error)
        }
    }

    // Temperature Display ---------------------------------------------------------------
    Rectangle {
        id: tempDisp
        width: 140
        height: 100
        x: 330; y: 330
        color: "transparent"
        border.color: "white"
        radius: 15
        Text {
            id: temp
            width: parent.width
            height: parent.height / 2
            text: parseFloat(readValues[0]).toFixed(2)
            font.pixelSize: 40
            color: "white"
            verticalAlignment: Text.AlignTop
            horizontalAlignment: Text.AlignHCenter
        }
    }
    Text {
        width: tempDisp.width
        height: tempDisp.height
        x: tempDisp.x; y: tempDisp.y + (temp.height / 2)
        text: "TEMP"
        //font.family: mainFont.font.family
        font.pixelSize: 35
        color: "white"
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
    }

    // pH Display ---------------------------------------------------------------------
    Rectangle {
        id: phDisp
        width: 140
        height: 100
        x: 80; y: 330
        color: "transparent"
        border.color: "white"
        radius: 15
        Text {
            id: phvalue
            width: parent.width
            height: parent.height / 2
            text: parseFloat(readValues[1]).toFixed(2)
            font.pixelSize: 40
            color: "white"
            verticalAlignment: Text.AlignTop
            horizontalAlignment: Text.AlignHCenter
        }
    }
    Text {
        width: phDisp.width
        height: phDisp.height
        x: phDisp.x; y: phDisp.y + (phvalue.height / 2)
        text: "PH"
        font.pixelSize: 35
        color: "white"
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
    }

    // TDS Display -------------------------------------------------------------------
    Rectangle {
        id: tdsDisp
        width: 140
        height: 100
        x: 580; y: 330
        color: "transparent"
        border.color: "white"
        radius: 15
        Text {
            id: tds
            width: parent.width
            height: parent.height / 2
            text: parseFloat(readValues[2]).toFixed(1)
            font.pixelSize: 40
            color: "white"
            verticalAlignment: Text.AlignTop
            horizontalAlignment: Text.AlignHCenter
        }
    }
    Text {
        width: tdsDisp.width
        height: tdsDisp.height
        x: tdsDisp.x; y: tdsDisp.y + (tds.height / 2)
        text: "TDS"
        //font.family: mainFont.font.family
        font.pixelSize: 35
        color: "white"
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
    }

    // Warning
    Rectangle {
        id: warning
        width: 500
        height: 180
        x: 150; y: 120
        color: "transparent"
        border.color: "red"
        border.width: 5
        radius: 15
        z: -1;
        visible: false;
        Image {
            id: warningImage
            x: -150; y: -120
            width: 800
            height: 480
            source: "/images/blueWpp.jpg"
            fillMode: Image.PreserveAspectCrop
            z: -1   //send image to background
        }

        Text {
            id: errTDS
            width: parent.width
            height: parent.height
            color: "red"
            font.pixelSize: 35
            text: "Unacceptable value\nof TDS\nPlease change\nthe aquarium water"
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            visible: false
        }

        Text {
            id: errPH
            width: parent.width
            height: parent.height
            color: "red"
            font.pixelSize: 35
            text: "Unacceptable value\nof pH\nPlease adjust the\naquarium water pH"
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            visible: false
        }

        Text {
            id: errBOTH
            width: parent.width
            height: parent.height
            color: "red"
            font.pixelSize: 35
            text: "Unacceptable value\nof pH and TDS\nPlease change\nthe aquarium water"
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            visible: false
        }

    }

    // Options Button ----------------------------------------------------------------
    Image {
        id: optionsButton
        width: 45
        height: 45
        x: 700; y: 30
        source: "/images/options.png"
        fillMode: Image.PreserveAspectCrop
        MouseArea {
            anchors.fill: parent
            onClicked: changePage("options");
        }
    }


    function checkError(err) {
        switch(err) {
        case 0:
            warning.visible = false;
            break;
        case 1:
            warning.visible = true
            errTDS.visible = true;
            errPH.visible = false;
            errBOTH.visible = false;
            break;
        case 2:
            warning.visible = true;
            errTDS.visible = false;
            errPH.visible = true;
            errBOTH.visible = false;
            break;
        case 3:
            warning.visible = true;
            errTDS.visible = false;
            errPH.visible = false;
            errBOTH.visible = true;;
            break;
        }
    }
}

