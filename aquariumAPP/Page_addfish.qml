import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

Rectangle{
    id: addfishPage
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
        y: 25
        width: 800
        height: 80
        text: "FISH SPECIES TO ADD"
        font.pixelSize: 40
        color: "white"
        horizontalAlignment: Text.AlignHCenter
    }

    ListView {
        id: list
        x:150; y:90
        width: 500
        height: 310
        interactive: false // disable scrolling

        property int selectedCOLOR : -1
        property int selectedID

        model: ListModel {
            id: allModel
            ListElement { data: "" }

            Component.onCompleted: {
                SendToDaemon.requestMQ("ALLFISH")
                var allFishQList = ReadFromDaemon.getQStrList()
                all.setDataList(allFishQList)
                allModel.remove(0)
                var currData = all.getCurrentData()
                for (var i = 0; i < currData.length; ++i)
                    allModel.append({ "data": currData[i] })
            }
        }

        delegate: Item {
            width: list.width
            height: 44

            Rectangle {
                border.color: "white"
                width: parent.width
                height: parent.height
                color: index === list.selectedCOLOR ? "light blue" : "transparent"

                Text {
                    x: 15
                    anchors.centerIn: parent
                    font.pixelSize: 20
                    color: "white"
                    text: model.data
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        list.selectedCOLOR = index
                        confirm_button.visible = true
                        list.selectedID = index + (all.getCurrentIndex() * all.getChunkSize())
                        all.setNameId(model.data, list.selectedID)
                    }
                }
            }
        }
    }

    Image {
        id:up_arrow
        source: "images/up_arrow.png"
        x: 40; y: 185
        width: 70
        height: 70
        fillMode: Image.PreserveAspectCrop
        MouseArea {
            anchors.fill: parent
            onClicked: {
                list.selectedCOLOR = -1
                confirm_button.visible = false
                all.showPreviousData()
                allModel.clear()
                var currData = all.getCurrentData()
                for (var i = 0; i < currData.length; ++i) {
                    allModel.append({ "data": currData[i] })
                }
            }
        }
    }

    Image {
        id:down_arrow
        source: "images/down_arrow.png"
        x: 40; y: 275
        width: 70
        height: 70
        fillMode: Image.PreserveAspectCrop
        MouseArea {
            anchors.fill: parent
            onClicked: {
                list.selectedCOLOR = -1
                confirm_button.visible = false
                all.showNextData()
                allModel.clear()
                var currData = all.getCurrentData()
                for (var i = 0; i < currData.length; ++i) {
                    allModel.append({ "data": currData[i] })
                }
            }
        }
    }

    // Confirmation Button ---------------------------------------------------------
    Image {
        id:confirm_button
        Component.onCompleted: confirm_button.visible = false
        source: "images/check.png"
        x: 685; y: 100
        width: 80
        height: 80
        fillMode: Image.PreserveAspectCrop
        MouseArea {
            anchors.fill: parent
            onClicked: {
                parent.visible = false
                changePage("addfishconf")

            }
        }
    }

    // Return ---------------------------------------------------------------
    Image {
        id: back_add
        source: "images/return.png"
        width: 70
        height: 70
        x: 700; y: 380
        fillMode: Image.Stretch
        MouseArea{
            anchors.fill: parent
            onClicked:{
                confirm_button.visible = false
                changePage("back")
            }
        }
    }
}

