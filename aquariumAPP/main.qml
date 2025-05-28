import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

Window {
    id: mainWindow
    width: 800
    height: 480
    visible: true
    title: qsTr("AQUARIUM CONTROL")
    flags: Qt.FramelessWindowHint

    StackView {
        id: stack
        anchors.fill: parent
        initialItem: page_main
    }

    Component {
        id: page_main
        Page_main{}
    }
    Component {
        id: page_options
        Page_options{}
    }
    Component {
        id: page_addfish
        Page_addfish{}
    }
    Component {
        id: page_addfish_conf
        Page_addfish_conf{}
    }
    Component {
        id: page_removefish
        Page_removefish{}
    }
    Component {
        id: page_onoff
        Page_onoff{}
    }
    Component {
        id: page_read
        Page_read{}
    }
    Component {
        id: page_added
        Page_added{}
    }
    Component {
        id: page_removed
        Page_removed{}
    }
    Component {
        id: page_ideal
        Page_ideal{}
    }
    Component {
        id: page_graph
        Page_graph{}
    }
    Component {
        id: page_change
        Page_change{}
    }
    Component {
        id: page_settings
        Page_settings{}
    }

    function changePage(page) {
        switch (page) {         
        case "options":
            stack.push(page_options)
            break
        case "addfish":
            stack.push(page_addfish)
            break
        case "addfishconf":
            stack.push(page_addfish_conf)
            break
        case "removefish":
            stack.push(page_removefish)
            break
        case "onoff":
            stack.push(page_onoff)
            break
        case "read":
            stack.push(page_read)
            break
        case "addSuccess":
            stack.push(page_added)
            break
        case "removeSuccess":
            stack.push(page_removed)
            break
        case "ideal":
            stack.push(page_ideal)
            break
        case "graph":
            stack.push(page_graph)
            break
        case "change":
            stack.push(page_change)
            break
        case "settings":
            stack.push(page_settings)
            break
        case "back":
            stack.pop()
            break
        case "addDone":
            stack.pop()
            stack.pop()
            stack.pop()
            stack.pop()
            break
        case "removeDone":
            stack.pop()
            stack.pop()
            stack.pop()
            break
        }
    }
}
