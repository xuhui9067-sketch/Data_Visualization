#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QThread>
#include <QQmlEngine>

#include "datareceiver.h"
#include "dataprocessor.h"
#include "databasemanager.h"

int main(int argc, char *argv[])
{
    // 新建事件循环载体
    QApplication app(argc, argv);

    // 实例化对象
    DataReceiver receiver;   // 数据接收
    DataProcessor processor; // 数据处理
    DatabaseManager* dbWorker = new DatabaseManager(); // 数据库管理
    QThread* dbThread = new QThread(); // 线程

    // 开始监听
    receiver.startListening(8888);

    // 处理数据
    QObject::connect(&receiver, &DataReceiver::validDataReceived, &processor, &DataProcessor::addRawData);

    // 将数据库处理部分移入子线程
    dbWorker->moveToThread(dbThread);

    // 将子线程与数据库对象进行绑定
    QObject::connect(dbThread, &QThread::started, dbWorker, &DatabaseManager::initDatabase);
    QObject::connect(dbThread, &QThread::finished, dbWorker, &QObject::deleteLater);

    // 将处理好的数据保存到子线程里的数据库
    QObject::connect(&processor, &DataProcessor::dataReady, dbWorker, &DatabaseManager::onSaveData);

    // 启动数据库子线程
    dbThread->start();

    // 声明 QML 渲染对象
    QQmlApplicationEngine engine;

    // 让 QML 拿到处理好的数据
    engine.rootContext()->setContextProperty("backend", &processor);

    // 程序报错，则安全退出
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    // 加载 QML 界面
    engine.loadFromModule("Data_Visualization", "Main");

    // 启动事件循环
    int execReturn = app.exec();

    // 关闭并清理子线程
    dbThread->quit();
    dbThread->wait();
    delete dbThread;

    // 返回程序关闭的结果
    return execReturn;
}
