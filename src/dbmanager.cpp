#include "dbmanager.h"

DBManager::DBManager(QObject *parent) : QObject(parent) {

}

DBManager::~DBManager() {

}

DBManager::DBManager(QString host, int port, QString driver, QString name, QString user, QString password, QObject *parent) : DBManager(parent) {
    db_host = host;
    db_port = port;
    db_driver = driver;
    db_name = name;
    db_user = user;
    db_password = password;
}

bool DBManager::createConnection(QString host, int port, QString driver, QString name, QString user, QString password) {
    db_host = host;
    db_port = port;
    db_driver = driver;
    db_name = name;
    db_user = user;
    db_password = password;
    return connect();
}

bool DBManager::connect() {
    if(connection.isOpen()) {
        connection.close();
    }
    connection = QSqlDatabase::addDatabase(db_driver);
    connection.setDatabaseName(db_name);
    connection.setPort(db_port);
    connection.setHostName(db_host);
    connection.setUserName(db_name);
    connection.setPassword(db_password);
    if (!connection.open()) {
        qDebug() << "Cannot open database:" << connection.lastError();
        return false;
    }
    /*
    QSqlQuery dropQuery(connection);
    if (!dropQuery.exec(SQL_DROP_ALL_TABLES)) {
        qDebug() << "Unable to drop all tables" << dropQuery.lastError();
    }
    */
    parseManager = new ParseManager(this, connection);
    for(int i = 0; i < ENT_COUNT; i++) {
        createTable((ENTITY_NAME)i);
    }
    return true;
}

void DBManager::fillTable(ENTITY_NAME ename) {
    QSqlQuery query(connection);
    switch (ename) {
    case ENT_EMPLOYEE:
        parseManager->loadEmployeeList();
        break;
    case ENT_PILLARS:
        parseManager->loadPillars();
        break;
    case ENT_SKILL_LEVELS:
        parseManager->loadSkillLevels();
        break;
    case ENT_SKILL_TYPES:
        parseManager->loadSkillTypes();
        break;
    case ENT_SKILLS:
        parseManager->loadSkills();
        break;
    case ENT_EMPLOYEES_X_SKILLS:
        parseManager->loadEmployeeCard();
        break;
    default:
        break;
    }

}

void DBManager::createTable(ENTITY_NAME ename) {
    QSqlQuery query(connection);
    switch (ename) {
    case ENT_EMPLOYEE:
        if (!query.exec(SQL_CREATE_EMLOYEES_TABLE)) {
            qDebug() << "Unable to create a table ul_employees" << query.lastError();
            return;
        } else {
            break;
        }
    case ENT_PILLARS:
        if (!query.exec(SQL_CREATE_PILLARS_TABLE)) {
            qDebug() << "Unable to create a table ul_pillars" << query.lastError();
            return;
        } else {
            break;
        }
    case ENT_SKILL_LEVELS:
        if (!query.exec(SQL_CREATE_SKILL_LEVELS_TABLE)) {
            qDebug() << "Unable to create a table ul_skill_levels" << query.lastError();
            return;
        } else {
            break;
        }
    case ENT_SKILL_TYPES:
        if (!query.exec(SQL_CREATE_SKILL_TYPES_TABLE)) {
            qDebug() << "Unable to create a table ul_skill_types" << query.lastError();
            return;
        } else {
            break;
        }
    case ENT_SKILLS:
        if (!query.exec(SQL_CREATE_SKILLS_TABLE)) {
            qDebug() << "Unable to create a table ul_skills" << query.lastError();
            return;
        } else {
            break;
        }
    case ENT_EMPLOYEES_X_SKILLS:
        if (!query.exec(SQL_CREATE_EMPLOYEES_X_SKILLS_TABLE)) {
            qDebug() << "Unable to create a table ul_employees_x_skills" << query.lastError();
            return;
        } else {
            if (!query.exec(SQL_CREATE_STATS_TABLE)) {
                qDebug() << "Unable to create a table ul_stats" << query.lastError();
                return;
            }
            break;
        }
    default:
        return;
    }
    fillTable(ename);
}

QSqlTableModel* DBManager::initModel(ENTITY_NAME ename) {
    QSqlTableModel* model; //= new QSqlTableModel(0, connection);
    switch (ename) {
    case ENT_EMPLOYEE:
        model = new QSqlTableModel(0, connection);
        model->setTable("ul_employees");
        break;
    case ENT_PILLARS:
        model = new QSqlTableModel(0, connection);
        model->setTable("ul_pillars");
        break;
    case ENT_SKILLS:
        model = new QSqlRelationalTableModel(0, connection);
        model->setTable("ul_skills");
        ((QSqlRelationalTableModel*)model)->setRelation(2, QSqlRelation("ul_skill_levels", "id", "name"));
        ((QSqlRelationalTableModel*)model)->setRelation(3, QSqlRelation("ul_skill_types", "id", "name"));
        ((QSqlRelationalTableModel*)model)->setRelation(5, QSqlRelation("ul_pillars", "id", "name"));
         model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
         model->setHeaderData(1, Qt::Horizontal, QObject::tr("NO"));
         model->setHeaderData(2, Qt::Horizontal, QObject::tr("LEVEL"));
         model->setHeaderData(3, Qt::Horizontal, QObject::tr("TYPE"));
         model->setHeaderData(4, Qt::Horizontal, QObject::tr("DESCRIPTION"));
         model->setHeaderData(5, Qt::Horizontal, QObject::tr("PILLAR"));
        break;
    default:
        model = new QSqlTableModel(0, connection);
        model->setTable("ul_employees");
        break;
    }
    model->select();
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    return model;
}

void DBManager::insertEmployee() {
    QSqlQuery query(connection);
    QString strF = "INSERT INTO ul_employees (name_leg, job_title, position_title, department, hire_date, code, name, reported) "
                   "VALUES('%1', '%2', '%3', '%4', '%5', '%6', '%7', '%8');";
    QString str = strF.arg("name_leg")
                      .arg("job_title")
                      .arg("position_title")
                      .arg("department")
                      .arg("01.01.2010")
                      .arg("0x12")
                      .arg("name")
                      .arg("reported");
    if (!query.exec(str)) {
        qDebug() << "Unable to make insert operation" << query.lastError();
    }
}

void DBManager::deleteEmployee(int id) {
    QSqlQuery query(connection);
    QString strF = "DELETE FROM ul_employees WHERE id = %1;";
    QString str = strF.arg(id);
    if (!query.exec(str)) {
        qDebug() << "Unable to make insert operation" << query.lastError();
    }
}
