#ifndef DATAPROCESSOR_H
#define DATAPROCESSOR_H

#include <QObject>
#include <QVector>
#include <QMutex>
#include <QTimer>
#include <QHash>

class DataProcessor: public QObject
{
    Q_OBJECT

public:
    explicit DataProcessor(QObject *parent = nullptr);

    // 将原始数据放入缓冲区
    void addRawData(const QString& info, float value);

signals:
    // 每隔一定时间，把处理好的一批数据发给 UI
    void dataReady(const QString& info, const QVector<float>& processedData);

private slots:
    // 内部定时机制
    void onTick();

private:
    // 数据清洗：滑动平均滤波
    float movingAverageFilter(const QString& info, float newValue);

    // 写缓冲区
    QHash<QString, QVector<float>> m_writeBuffers;

    // 读缓冲区
    QHash<QString, QVector<float>> m_readBuffers;

    // 互斥锁
    QMutex m_mutex;

    // 定时器
    QTimer* m_timer;

    // 滤波数据存放的容器
    QHash<QString, QVector<float>> m_filterVectors;

    // 滑动窗口
    const int m_windowSize = 10;
};

#endif // DATAPROCESSOR_H
