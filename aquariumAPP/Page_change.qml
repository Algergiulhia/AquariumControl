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

    Text {
        y: 30
        width: 800
        height: 100
        text: "TIME SETTINGS"
        font.pixelSize: 40
        color: "white"
        horizontalAlignment: Text.AlignHCenter
    }

    property int change_selected: 2

    SpinBox {
        id: change_hours
        x:270; y:115
        width: 150
        height: 250
        font.pixelSize: 60
        value: 12
        from: 0
        to: 23

        contentItem: Text {
            text: change_hours.textFromValue(change_hours.value)
            font.pixelSize: 60
            visible: true
            color: "white"
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }

        background: Rectangle {
            color: "transparent"
        }

        up.indicator: Rectangle {
            height: parent.height / 4
            anchors.top: parent.top
            implicitWidth: parent.width
            color: "transparent"
            radius: 20
            border.width: 3
            border.color: "white"
            Text {
                text: '+'
                font.pixelSize: 50
                color: "white"
                anchors.centerIn: parent
            }
        }
        down.indicator: Rectangle {
            height: parent.height / 4
            anchors.bottom: parent.bottom
            implicitWidth: parent.width
            color: "transparent"
            radius: 20
            border.width: 3
            border.color: "white"
            Text {
                text: '-'
                font.pixelSize: 80
                color: "white"
                anchors.centerIn: parent
            }
        }
    }

    Text {
        x: 432; y: 185
        width: 10
        height: 40
        text: ":"
        font.pixelSize: 80
        color: "white"
    }

    SpinBox {
        id: change_minutes
        x:470 ; y:115
        width: 150
        height: 250
        font.pixelSize: 60
        value: 12
        from: 0
        to: 59

        contentItem: Text {
            text: change_minutes.textFromValue(change_minutes.value)
            font.pixelSize: 60
            visible: true
            color: "white"
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }

        background: Rectangle {
            color: "transparent"
        }

        up.indicator: Rectangle {
            height: parent.height / 4
            anchors.top: parent.top
            implicitWidth: parent.width
            color: "transparent"
            radius: 20
            border.width: 3
            border.color: "white"
            Text {
                text: '+'
                font.pixelSize: 50
                color: "white"
                anchors.centerIn: parent
            }
        }
        down.indicator: Rectangle {
            height: parent.height / 4
            anchors.bottom: parent.bottom
            implicitWidth: parent.width
            color: "transparent"
            radius: 20
            border.width: 3
            border.color: "white"
            Text {
                text: '-'
                font.pixelSize: 80
                color: "white"
                anchors.centerIn: parent
            }
        }
    }

    Column {
        spacing: 30
        x: 50
        anchors.verticalCenter: parent.verticalCenter

        Rectangle {
            id: change_light
            width: 150
            height: 80
            color: "transparent"
            border.color: "white"
            border.width: 5
            radius: 15
            Text {
                width: parent.width
                height: parent.height
                text: "LIGHT\nTIME"
                font.pixelSize: 25
                color: "white"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    change_selected = 2
                    change_light.border.width = 5
                    change_feed1.border.width = 2
                    change_feed2.border.width = 2
                    change_light_duration.border.width = 2
                }
            }
        }

        Rectangle {
            id: change_feed1
            width: 150
            height: 80
            color: "transparent"
            border.color: "white"
            border.width: 2
            radius: 15
            Text {
                width: parent.width
                height: parent.height
                text: "1st FEED\nTIME"
                font.pixelSize: 25
                color: "white"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    change_selected = 0
                    change_feed1.border.width = 5
                    change_feed2.border.width = 2
                    change_light.border.width = 2
                    change_light_duration.border.width = 2
                }
            }
        }

        Rectangle {
            id: change_feed2
            width: 150
            height: 80
            color: "transparent"
            border.color: "white"
            border.width: 2
            radius: 15
            Text {
                width: parent.width
                height: parent.height
                text: "2nd FEED\nTIME"
                font.pixelSize: 25
                color: "white"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    change_selected = 1
                    change_feed2.border.width = 5
                    change_light.border.width = 2
                    change_feed1.border.width = 2
                    change_light_duration.border.width = 2
                }
            }
        }

        Rectangle {
            id: change_light_duration
            width: 150
            height: 80
            color: "transparent"
            border.color: "white"
            border.width: 2
            radius: 15
            Text {
                width: parent.width
                height: parent.height
                text: "LIGHT\nDURATION"
                font.pixelSize: 25
                color: "white"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    change_selected = 3
                    change_light_duration.border.width = 5
                    change_feed1.border.width = 2
                    change_feed2.border.width = 2
                    change_light.border.width = 2
                }
            }
        }
    }

    Image {
        id:confirm_change
        source: "images/check.png"
        anchors.verticalCenter: parent.verticalCenter
        x: 685;
        width: 80
        height: 80
        fillMode: Image.PreserveAspectCrop
        MouseArea {
            anchors.fill: parent
            onClicked: {
                var newHours = change_hours.textFromValue(change_hours.value)
                var newMinutes = change_minutes.textFromValue(change_minutes.value)
                switch (change_selected) {
                case 0:
                    console.log(change_selected)
                    Ideal.setFeedTime(newHours, newMinutes, change_selected)
                    break
                case 1:
                    console.log(change_selected)
                    Ideal.setFeedTime(newHours, newMinutes, change_selected)
                    break
                case 2:
                    console.log(change_selected)
                    Ideal.setLightTime(newHours, newMinutes)
                    break
                case 3:
                    console.log(change_selected)
                    Ideal.setLightTimeDuration(newHours, newMinutes)
                    break
                }
                changePage("back")
            }
        }
    }

    // Return ---------------------------------------------------------------
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
