#include "datareceiver.h"
#include <QDebug>
#include <QtEndian>

// 构造函数
DataReceiver::DataReceiver(QObject *parent): QObject(parent)
{
    // TCP服务端
    m_tcpServer = new QTcpServer(this);

    // 处理新客户端连接
    connect(m_tcpServer, &QTcpServer::newConnection, this, &DataReceiver::m_newConnection);
}

// 析构函数
DataReceiver::~DataReceiver()
{
    // 遍历所有客户端
    for (QTcpSocket* socket: m_socketBuffers.keys())
    {
        // 当前还有客户端连接
        if(socket)
        {
            // 断开连接
            socket->disconnectFromHost();
        }
    }
}

// 启动网络监听
void DataReceiver::startListening(quint16 port)
{
    // 监听成功
    if (m_tcpServer->listen(QHostAddress::Any, port))
    {
        qDebug() << "开始监听TCP服务端" << port;
    }

    // 监听失败
    else
    {
        qDebug() << "监听失败";
    }
}

// 处理新连接
void DataReceiver::m_newConnection()
{
    // 获取客户信息
    QTcpSocket* socket = m_tcpServer->nextPendingConnection();
    qDebug() << "新客户连接：" << socket->peerAddress().toString();

    // 分配一个空字节数组
    m_socketBuffers.insert(socket, QByteArray());

    // 读取数据
    connect(socket, &QTcpSocket::readyRead, this, &DataReceiver::m_readData);

    // 断开连接
    connect(socket, &QTcpSocket::disconnected, this, &DataReceiver::m_disconnected);
}

// 处理接收到的数据
void DataReceiver::m_readData()
{
    // 获取客户信息
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());

    // 没有客户，或哈希表里没有该用户，则不处理
    if (!socket || !m_socketBuffers.contains(socket)) return;

    // 获取该客户端专属的缓冲区
    QByteArray& buffer = m_socketBuffers[socket];

    // 获取全部数据
    buffer.append(socket->readAll());

    // 客户端信息
    QString info = socket->peerAddress().toString() + ":" + QString::number(socket->peerPort());

    // Modbus最小报文长度
    const int MODBUS_MIN_SIZE = 13;

    // 持续处理数据
    while (buffer.size() >= MODBUS_MIN_SIZE)
    {
        // 解析 MBAP 报头里的长度字段
        int length = (static_cast<unsigned char>(buffer.at(4)) << 8) | static_cast<unsigned char>(buffer.at(5));

        // MBAP前6字节 + 长度字段定义的后续字节
        int fullPacketSize = 6 + length;

        // 数据还没收全，继续等（半包现象）
        if (buffer.size() < fullPacketSize)
        {
            break;
        }

        // 提取功能码
        unsigned char funcCode = static_cast<unsigned char>(buffer.at(7));

        // 确认是 03 功能码
        if (funcCode == 0x03)
        {
            // 获取数据长度
            int dataLength = static_cast<unsigned char>(buffer.at(8));

            // 处理数据
            QByteArray data = buffer.mid(9, dataLength);

            // 转成本机端序
            float sensorValue = qFromBigEndian<float>(data.constData());

            // 解析数据成功，发送出去
            emit validDataReceived(info, sensorValue);

            // 删掉已处理好的数据
            buffer.remove(0, fullPacketSize);
        }

        // 非法功能码，丢弃一个字节继续找
        else
        {
            buffer.remove(0, 1);
        }
    }
}

// 断开连接
void DataReceiver::m_disconnected()
{
    // 获取客户信息
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());

     // 没有客户，或哈希表里没有该用户，则不处理
    if (!socket || !m_socketBuffers.contains(socket)) return;

    // 客户信息IP与Port
    QString info = socket->peerAddress().toString() + ":" + QString::number(socket->peerPort());
    qDebug() << "客户断开连接，回收包厢：" << info;

    // 从哈希表里删除
    m_socketBuffers.remove(socket);

    // 释放Socket内存
    socket->deleteLater();
}
