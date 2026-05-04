#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVector>

class DatabaseManager : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseManager(QObject *parent = nullptr);
    ~DatabaseManager();

public slots:
    // 数据库初始化
    void initDatabase();

    // 保存数据
    void onSaveData(const QString& info, const QVector<float>& data);

private:
    // 数据库管理
    QSqlDatabase m_db;

    // 文件名
    const QString m_dbName = "sensor_data.db";
};

#endif // DATABASEMANAGET_H
