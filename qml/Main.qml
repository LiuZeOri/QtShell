import QtQuick
import QtQuick.Controls

ApplicationWindow
{
    visible: true
    width: 640
    height: 480
    title: qsTr("Qt Shell")

    Column
    {
        anchors.fill: parent

        TextArea
        {
            id: outputArea
            anchors.fill: parent
            anchors.bottom: inputField.top
            font.family: "Courier"
            font.pointSize: 12

            readOnly: true
            wrapMode: TextEdit.Wrap
            text:""
        }

        TextField
        {
            id: inputField
            width: parent.width
            anchors.bottom: parent.bottom

            font.family: "Courier"
            font.pointSize: 12
            placeholderText: "请输入Shell命令"

            onAccepted:
            {
                let inputText = inputField.text;
                if (inputText.length > 0)
                {
                    outputArea.append("Shell> " + inputText + "\n");
                    inputField.text = "";

                    controller.executeCommand(inputText);
                }
            } 
        }
    }

    Connections
    {
        target: controller
        onCommandOutput:
        {
            outputArea.append(output)
        }
    }
}
