#ifndef DATARECEIVER_H
#define DATARECEIVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QByteArray>
#include <QHash>
#include <QQmlEngine>

class DataReceiver: public QObject
{
    Q_OBJECT

public:
    explicit DataReceiver(QObject *parent = nullptr);
    ~DataReceiver();

    // 启动监听
    void startListening(quint16 port);

signals:
    // 解析出有效的数据
    void validDataReceived(const QString& clientId, float sensorValue);

private slots:
    // 处理新客户端连接
    void m_newConnection();

    // 读取收取的数据
    void m_readData();

    // 客户端断开连接
    void m_disconnected();

private:
    // TCP服务端
    QTcpServer* m_tcpServer;

    // TCP客户端与专属缓冲区的映射表
    QHash<QTcpSocket*, QByteArray> m_socketBuffers;
};

#endif // DATARECEIVER_H
