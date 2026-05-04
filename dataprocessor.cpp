#include "dataprocessor.h"

// 构造函数
DataProcessor::DataProcessor(QObject *parent) : QObject(parent)
{
    // 定时器实例化
    m_timer = new QTimer(this);

    // 每隔33ms，自动触发一次数据分发
    connect(m_timer, &QTimer::timeout, this, &DataProcessor::onTick);

    // 启动定时器
    m_timer->start(33);
}

// 接收原始数据
void DataProcessor::addRawData(const QString& info, float value)
{
    // 获取数据的滤波结果
    float filteredValue = movingAverageFilter(info, value);

    // 将数据添加进写缓冲区
    m_mutex.lock();
    m_writeBuffers[info].append(filteredValue);
    m_mutex.unlock();
}

// 滑动平均滤波算法
float DataProcessor::movingAverageFilter(const QString& info, float newValue)
{
    // 获取对应传感器的数据容器
    QVector<float>& vector_info = m_filterVectors[info];

    // 将新数据加到窗口列表的末尾
    vector_info.append(newValue);

    // 超过了滑动窗口的最大值
    if (vector_info.size() > m_windowSize)
    {
        //移除前面的数据
        vector_info.removeFirst();
    }

    // 把容器里的数据加起来
    float sum = 0;
    for (float v : vector_info)
    {
        sum += v;
    }

    // 返回平均值
    return sum / vector_info.size();
}

// 定时器触发，分发数据
void DataProcessor::onTick()
{
    // 互斥锁
    m_mutex.lock();

    // 如果写缓冲区是空的，则不处理
    if (m_writeBuffers.isEmpty())
    {
        m_mutex.unlock();
        return;
    }

    // 清空读缓冲区
    m_readBuffers.clear();

    // 交换缓冲区
    m_readBuffers.swap(m_writeBuffers);

    // 解锁
    m_mutex.unlock();

    // 遍历读缓冲区
    for (auto it = m_readBuffers.begin(); it != m_readBuffers.end(); it++)
    {
        // 数据有效
        if (!it.value().isEmpty())
        {
            // 发送数据
            emit dataReady(it.key(), it.value());
        }
    }
}
