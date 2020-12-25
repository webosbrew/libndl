import QtQuick 2.4
import Eos.Window 0.1
import Eos.Items 0.1
import WebOS.Global 1.0

import Sample 1.0

WebOSWindow {
    id: root

    visible: true
    width: 1920
    height: 1080

    keyMask: WebOSWindow.KeyMaskBack | keyMask

    Component.onCompleted: {
        // Window will handle back key event only when this set to true on my TV.
        windowProperties["_WEBOS_ACCESS_POLICY_KEYS_BACK"] = "true"
    }

    Item {
        anchors.fill: parent
        focus: true

        PunchThrough {
            x: 0; y: 0; z: -1
            width: parent.width
            height: parent.height
        }

        Keys.onPressed: {
            // webOS related key codes are defined in qweboskeyextension.h
            if (event.key == WebOS.Key_webOS_Back) {
                Qt.quit()
            }
        }

        MouseArea {
            anchors.fill: parent
            onClicked: backend.test()
        }
    }

    Backend {
        id: backend
    }
}
