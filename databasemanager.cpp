#include "databasemanager.h"
#include <QSqlError>
#include <QDebug>
#include <QDateTime>
#include <QCoreApplication>
#include <QDir>

// 构造函数
DatabaseManager::DatabaseManager(QObject *parent) : QObject(parent) {}

// 析构函数
DatabaseManager::~DatabaseManager()
{
    // 断开数据库连接
    if (m_db.isOpen()) m_db.close();
}

// 建库与建表
void DatabaseManager::initDatabase()
{
    // 获取当前程序运行的目录
    QString appPath = QCoreApplication::applicationDirPath();

    // 得到完整绝对路径
    QString dbPath = appPath + "/sensor_data.db";

    // 配置驱动与连接名
    m_db = QSqlDatabase::addDatabase("QSQLITE", "WorkerConnection");

    // 路径指定
    m_db.setDatabaseName(dbPath);
    qDebug() << "数据库文件当前锁定的绝对路径是：" << dbPath << Qt::endl;

    // 如果打不开文件，则不处理
    if (!m_db.open())
    {
        qDebug() << "数据库打开失败:" << m_db.lastError().text();
        return;
    }

    // 绑定数据库
    QSqlQuery query(m_db);

    // 如果表不存在就创建
    query.exec("CREATE TABLE IF NOT EXISTS sensor_logs ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "info TEXT,"
               "timestamp TEXT, "
               "value REAL)");
}

// 存储数据
void DatabaseManager::onSaveData(const QString& info, const QVector<float>& data)
{
    // 没有数据或打不开文件，则不处理
    if (data.isEmpty() || !m_db.isOpen()) return;

    // 开启事务
    m_db.transaction();

    // 绑定数据库
    QSqlQuery query(m_db);

    // 获取时间
    QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz");

    // 数据存入的模板
    query.prepare("INSERT INTO sensor_logs (info, timestamp, value) VALUES (?, ?, ?)");

    // 遍历传进来的数组
    for (float val : data)
    {
        // 按照信息、时间、数值的顺序添加
        query.addBindValue(info);
        query.addBindValue(currentTime);
        query.addBindValue(val);

        // 执行
        query.exec();
    }

    // 提交事务
    if (!m_db.commit())
    {
        qDebug() << "提交失败:" << m_db.lastError().text();
        m_db.rollback();
    }
}
