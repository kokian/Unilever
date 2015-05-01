#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QObject>
#include <QtCore>
#include <QtSql>
#include "constants.h"
#include "parsemanager.h"

class DBManager : public QObject
{
    Q_OBJECT
public:
    explicit DBManager(QObject *parent = 0);
    DBManager(QString host, int port, QString driver, QString name, QString user, QString password, QObject *parent = 0);

    bool createConnection(QString host, int port, QString driver, QString name, QString user, QString password);
    bool connect();
    QSqlTableModel* initModel(ENTITY_NAME ename);

    void insertEmployee();
    void deleteEmployee(int id);

    QSqlDatabase connection;

    ~DBManager();
private:
    QString db_host;
    int db_port;
    QString db_driver;
    QString db_user;
    QString db_password;
    QString db_name;

    ParseManager* parseManager;


    void createTable(ENTITY_NAME ename);
    void fillTable(ENTITY_NAME ename);

signals:

public slots:
};

#endif // DBMANAGER_H
