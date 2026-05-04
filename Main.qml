import QtQuick
import QtQuick.Controls
import Data_Visualization

Window
{
    width: 800
    height: 600
    visible: true
    title: qsTr("数据采集可视化平台")
    color: "#1e1e1e"

    // 顶部状态文字
    Text
    {
        id: valueText
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.margins: 20
        text: "尚未开启"
        color: "#00ff00"
        font.pixelSize: 20
        font.bold: true
        horizontalAlignment: Text.AlignHCenter
    }

    // 绘图
    CustomPlotItem
    {
        id: wavePlot
        anchors.top: valueText.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 20
    }

    Connections
    {
        target: backend
        function onDataReady(info, processedData)
        {
            // 信号长度有效
            if (processedData.length > 0)
            {
                // 显示最新数据
                let latestValue = processedData[processedData.length - 1];
                valueText.text = "最新数值: " + latestValue.toFixed(2);

                // 更新绘图数据
                wavePlot.updateData(info, processedData)
            }
        }
    }
}
