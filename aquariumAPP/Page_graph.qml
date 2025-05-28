import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtCharts 2.15

Rectangle{
    id: graphPage
    width: 800
    height: 480
    color:"transparent"

    property var tdsData: []
    property var phData: []
    property var tempData: []
    property var currentData: []


    ChartView {
        id: sensorChart
        title: "Line"
        anchors.fill: parent
        antialiasing: true
        backgroundColor: "transparent"
        plotArea: Qt.rect(100,100,600,250)

        ValueAxis {
            id: axisY
            gridVisible: true
            tickCount: 5
            min: 0
            max: 1
        }

        ValueAxis {
            id: axisX
            min: 0
            max: 1
            gridVisible: false
            tickCount: 0
        }
    }

    function showData(sensor) {
        var line = sensorChart.createSeries(ChartView.SeriesTypeLine, "", axisX, axisY)

        var higherValue = sensor[0]
        var lowerValue = sensor[0]
        for (var i = 0; i < sensor.length; ++i) {
            line.append(i, sensor[i])
            if (sensor[i] > higherValue) {
                higherValue = sensor[i]
            }
            if(sensor[i] < lowerValue) {
                lowerValue = sensor[i]
            }
        }

        line.color = "white"
        axisX.min = 0
        axisX.max = sensor.length
        axisX.color = "white"
        axisY.min = lowerValue
        axisY.max = higherValue
        axisY.color = "white"
    }

    Component.onCompleted: {
        SendToDaemon.requestMQ("DATA temp")
        tempData = ReadFromDaemon.getQStrList();
        sensorChart.removeAllSeries()
        showData(tempData)
    }

    // Blue Wallpaper ---------------------------------------------------------------------
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
        text: "Sensors Data"
        font.pixelSize: 40
        color: "white"
        horizontalAlignment: Text.AlignHCenter
    }

    Rectangle {
        id: tdsRecord
        width: 120
        height: 80
        x: 600-tdsRecord.width/2 ; y: 390
        color: "transparent"
        border.color: "white"
        border.width: 2
        radius: 15

        Text {
            width: parent.width
            height: parent.height
            text: "TDS"
            font.pixelSize: 30
            color: "white"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                SendToDaemon.requestMQ("DATA tds")
                tdsData = ReadFromDaemon.getQStrList();
                console.log(tdsData)
                sensorChart.removeAllSeries()
                showData(tdsData)
                parent.border.width = 6;
                phRecord.border.width = 2;
                tempRecord.border.width = 2;
            }
        }
    }

    Rectangle {
        id: tempRecord
        width: 120
        height: 80
        x: 400-tdsRecord.width/2 ; y: 390
        color: "transparent"
        border.color: "white"
        border.width: 6
        radius: 15

        Text {
            width: parent.width
            height: parent.height
            text: "TEMP"
            font.pixelSize: 30
            color: "white"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                SendToDaemon.requestMQ("DATA temp")
                tempData = ReadFromDaemon.getQStrList();
                console.log(tempData)
                sensorChart.removeAllSeries()
                showData(tempData)
                parent.border.width = 6;
                phRecord.border.width = 2;
                tdsRecord.border.width = 2;
            }
        }
    }

    Rectangle {
        id: phRecord
        width: 120
        height: 80
        x: 200-tdsRecord.width/2 ; y: 390
        color: "transparent"
        border.color: "white"
        border.width: 2
        radius: 15

        Text {
            width: parent.width
            height: parent.height
            text: "PH"
            font.pixelSize: 30
            color: "white"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                SendToDaemon.requestMQ("DATA ph")
                phData = ReadFromDaemon.getQStrList();
                console.log(phData)
                sensorChart.removeAllSeries()
                showData(phData)
                parent.border.width = 6;
                tdsRecord.border.width = 2;
                tempRecord.border.width = 2;
            }
        }
    }

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
