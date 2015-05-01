#include "parsemanager.h"

ParseManager::ParseManager(QObject *parent) : QObject(parent) {
    fileName = "/Users/Kirill/Works/Unilever/employees3.xlsx";
}

ParseManager::ParseManager(QObject *parent, QSqlDatabase conn) : ParseManager(parent) {
    //loadXlsxDocument(conn);
    conn = conn;
}

ParseManager::~ParseManager() {

}


bool ParseManager::loadEmployeeList() {
    QXlsx::Document* doc = new QXlsx::Document(fileName);
    QStringList sheets = doc->sheetNames();
    for (int i = 0; i < sheets.count(); i++) {
        qDebug() << "Sheet[" << i << "] = " << sheets[i];
    }
    doc->selectSheet(sheets[1]);
    int row = 2;
    while(doc->read(row,1).toString() != "") {
        QSqlQuery query(conn);
        QString name_leg = doc->read(row,2).toString();
        QString job_title = doc->read(row,3).toString();
        QString position_title = doc->read(row,4).toString();
        QString department = doc->read(row,5).toString();
        QString hire_date = doc->read(row,6).toString();
        QString code = doc->read(row,7).toString();
        QString name = doc->read(row,8).toString();
        QString reported = doc->read(row,9).toString();

        QString strF = "INSERT INTO ul_employees (name_leg, job_title, position_title, department, hire_date, code, name, reported) "
                       "VALUES('%1', '%2', '%3', '%4', '%5', '%6', '%7', '%8');";
        QString str = strF.arg(name_leg)                          
                          .arg(job_title)
                          .arg(position_title)
                          .arg(department)
                          .arg(hire_date)
                          .arg(code)
                          .arg(name)
                          .arg(reported);
        if (!query.exec(str)) {
            qDebug() << "Unable to make insert operation" << query.lastError();
        }
        row++;
    }

    delete doc;
    return true;
}

bool ParseManager::loadSkills() {
    QXlsx::Document* doc = new QXlsx::Document(fileName);
    QStringList sheets = doc->sheetNames();
    for (int i = 0; i < sheets.count(); i++) {
        qDebug() << "Sheet[" << i << "] = " << sheets[i];
    }
    doc->selectSheet(sheets[0]);
    int row = 4;
    while(doc->read(row,1).toString() != "") {
        QSqlQuery query(conn);
        QString req;
        QString reqP;
        QString number = doc->read(row,1).toString();
        QString level = doc->read(row,2).toString();
        req = "SELECT id FROM ul_skill_levels WHERE name='%1'";
        reqP = req.arg(level);
        int level_id = 1;
        if (!query.exec(reqP)) {
            qDebug() << "Unable to select operation" << query.lastError();
        } else {
            query.next();
            level_id = query.value(0).toInt();
        }

        QString type = doc->read(row,3).toString();
        req = "SELECT id FROM ul_skill_types WHERE name='%1'";
        reqP = req.arg(type);
        int type_id = 1;
        if (!query.exec(reqP)) {
            qDebug() << "Unable to select operation" << query.lastError();
        } else {
            query.next();
            type_id = query.value(0).toInt();
        }

        QString description = doc->read(row,4).toString();

        QString pillar = doc->read(row,5).toString();
        req = "SELECT id FROM ul_pillars WHERE name='%1'";
        reqP = req.arg(pillar);
        int pillar_id = 1;
        if (!query.exec(reqP)) {
            qDebug() << "Unable to select operation" << query.lastError();
        } else {
            query.next();
            pillar_id = query.value(0).toInt();
        }

        QString prod_proc_eng = doc->read(row,6).toString();
        QString shift_leader = doc->read(row,7).toString();
        QString logist_eng = doc->read(row,8).toString();
        QString technician = doc->read(row,9).toString();
        QString quality_eng = doc->read(row,10).toString();
        QString planning_eng = doc->read(row,11).toString();
        QString suit = doc->read(row,12).toString();
        QString she_eng = doc->read(row,13).toString();
        QString env_eng = doc->read(row,14).toString();
        QString train_coordinator = doc->read(row,15).toString();
        QString finance_spec = doc->read(row,16).toString();
        QString man_ex = doc->read(row,17).toString();


        QString strF = "INSERT INTO ul_skills ("
                       "number, "
                       "level_id, "
                       "type_id, "
                       "description, "
                       "pillar_id, "
                       "production_process_engineer, "
                       "shift_leader, "
                       "logistic_engineer, "
                       "technician, "
                       "quality_engineer, "
                       "planning_engineer, "
                       "suit, "
                       "she_engineer, "
                       "env_engineer, "
                       "training_coordinator, "
                       "finance_specialist, "
                       "man_ex ) "
                       "VALUES('%1', %2, %3, '%4', %5, '%6', '%7', '%8', '%9', '%10', '%11', '%12', '%13', '%14', '%15', '%16', '%17');";

        QString str = strF.arg(number)
                          .arg(level_id)
                          .arg(type_id)
                          .arg(description)
                          .arg(pillar_id)
                          .arg(prod_proc_eng)
                          .arg(shift_leader)
                          .arg(logist_eng)
                          .arg(technician)
                          .arg(quality_eng)
                          .arg(planning_eng)
                          .arg(she_eng)
                          .arg(env_eng)
                          .arg(train_coordinator)
                          .arg(finance_spec)
                          .arg(man_ex);

        if (!query.exec(str)) {
            qDebug() << "Unable to make insert operation" << query.lastError();
        }
        row++;
    }

    delete doc;
    return true;
}

bool ParseManager::loadPillars() {
    int count = 10;
    QString pillars[10] = {"PM", "AM", "QC", "FI", "PD", "EEM", "CD", "Safety", "Env", "Log"};

    for(int i = 0; i < count; i++) {
        QSqlQuery query(conn);
        QString strF = "INSERT INTO ul_pillars (name) "
                       "VALUES('%1');";
        QString str = strF.arg(pillars[i]);
        if (!query.exec(str)) {
            qDebug() << "Unable to make insert operation" << query.lastError();
            return false;
        }
    }
    return true;
}

bool ParseManager::loadSkillLevels() {
    int count = 3;
    QString pillars[3] = {"BASE", "INTERMEDIA", "ADVANCED"};

    for(int i = 0; i < count; i++) {
        QSqlQuery query(conn);
        QString strF = "INSERT INTO ul_skill_levels (name) "
                       "VALUES('%1');";
        QString str = strF.arg(pillars[i]);
        if (!query.exec(str)) {
            qDebug() << "Unable to make insert operation" << query.lastError();
            return false;
        }
    }
    return true;
}

bool ParseManager::loadSkillTypes() {
    int count = 2;
    QString pillars[2] = {"TOOL", "METHOD"};

    for(int i = 0; i < count; i++) {
        QSqlQuery query(conn);
        QString strF = "INSERT INTO ul_skill_types (name) "
                       "VALUES('%1');";
        QString str = strF.arg(pillars[i]);
        if (!query.exec(str)) {
            qDebug() << "Unable to make insert operation" << query.lastError();
            return false;
        }
    }
    return true;
}
