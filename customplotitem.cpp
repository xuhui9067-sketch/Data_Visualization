#include "customplotitem.h"

// 构造函数
CustomPlotItem::CustomPlotItem(QQuickItem *parent) : QQuickPaintedItem(parent)
{
    // 实例化
    m_customPlot = new QCustomPlot();

    // 设置背景
    m_customPlot->setBackground(QBrush(QColor("#1e1e1e")));

    // 设置坐标轴
    m_customPlot->xAxis->setBasePen(QPen(Qt::white));
    m_customPlot->yAxis->setBasePen(QPen(Qt::white));
    m_customPlot->xAxis->setTickPen(QPen(Qt::white));
    m_customPlot->yAxis->setTickPen(QPen(Qt::white));
    m_customPlot->xAxis->setTickLabelColor(Qt::white);
    m_customPlot->yAxis->setTickLabelColor(Qt::white);

    // 设置坐标轴范围
    m_customPlot->xAxis->setRange(0, 100);
    m_customPlot->yAxis->setRange(10, 40);

    // 颜色池
    m_colorPool << QColor("#00ff00") << QColor("#00aaff") << QColor("#ffff00")
                << QColor("#ff00ff")<< QColor("#ff4444") << QColor("#00ffff");
}

// 析构函数
CustomPlotItem::~CustomPlotItem()
{
    delete m_customPlot;
}

// 将 QWidgetlode 图表转移到 QML 里
void CustomPlotItem::paint(QPainter *painter)
{
    // 图表不为空
    if (m_customPlot)
    {
        // 同步尺寸
        m_customPlot->setViewport(QRect(0, 0, width(), height()));

        // 创建一张完全空白的图片，大小和当前界面一样
        QPixmap picture(boundingRect().size().toSize());

        // 透明色
        picture.fill(Qt::transparent);

        // 准备画笔
        QCPPainter qcpPainter(&picture);

        // 绘图
        m_customPlot->toPainter(&qcpPainter);
        painter->drawPixmap(QPointF(0, 0), picture);
    }
}

// 处理接收到的数据并刷新图表
void CustomPlotItem::updateData(const QString& info, const QVariantList& values)
{
    // 没有新数据，则不处理
    if (!m_customPlot) return;

    // 哈希表里没有这个客户端
    if (!m_clientMaps.contains(info))
    {
        // 在图表上添加一条新线
        QCPGraph *newGraph = m_customPlot->addGraph();

        // 从颜色池里挑一个颜色
        QColor color = m_colorPool[m_colorIndex % m_colorPool.size()];

        // 颜色索引+1
        m_colorIndex++;

        // 设置画笔
        newGraph->setPen(QPen(color, 2));

        // 登记到哈希表里
        ClientData newData;
        newData.graph = newGraph;
        newData.currentKey = 0.0;
        m_clientMaps.insert(info, newData);
    }

    // 获取折线和 X 轴进度
    ClientData& data = m_clientMaps[info];

    // 把新数据逐个添加到图表的折线中
    for (int i = 0; i < values.size(); i++)
    {
        // 添加数据
        data.graph->addData(data.currentKey, values[i].toDouble());

        // X坐标+1
        data.currentKey += 1.0;
    }

    // 更新坐标轴范围
    m_customPlot->xAxis->setRange(data.currentKey, 300, Qt::AlignRight);

    // 刷新界面
    update();
}
