#ifndef CUSTOMPLOTITEM_H
#define CUSTOMPLOTITEM_H

#include <QtQuick/QQuickPaintedItem>
#include <QVariantList>
#include <QHash>
#include "qcustomplot.h"

class CustomPlotItem : public QQuickPaintedItem
{
    Q_OBJECT
    QML_ELEMENT

public:
    CustomPlotItem(QQuickItem* parent = nullptr);
    virtual ~CustomPlotItem();

    // 绘图
    virtual void paint(QPainter* painter) override;

    // 数据更新
    Q_INVOKABLE void updateData(const QString& info, const QVariantList& values);

private:
    // 图表控件
    QCustomPlot* m_customPlot;

    // 客户对应的折线与折线的 X 轴进度
    struct ClientData
    {
        QCPGraph* graph;
        double currentKey;
    };

    // 记录每个客户的IP与对应信息
    QHash<QString, ClientData> m_clientMaps;

    // 颜色池
    QList<QColor> m_colorPool;

    // 颜色索引值
    int m_colorIndex = 0;
};

#endif // CUSTOMPLOTITEM_H
