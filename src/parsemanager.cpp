#include "parsemanager.h"

ParseManager::ParseManager(QObject *parent) : QObject(parent) {
    fileName = "C:\\projects\\qt\\Unilever\\employees.xlsx";
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

bool ParseManager::loadEmployeeCard() {
    QXlsx::Document* doc = new QXlsx::Document(fileName);
    QStringList sheets = doc->sheetNames();
    doc->selectSheet(sheets[2]);
    QSqlQuery query(conn);

    QString id_code = doc->read(4,3).toString();
    QString req = "SELECT id FROM ul_employees WHERE LOWER(code)=LOWER('%1')";
    QString reqP = req.arg(id_code);
    int employee_id = 0;
    if (!query.exec(reqP)) {
        qDebug() << "Unable to select operation" << query.lastError();
    } else {
        query.next();
        employee_id = query.value(0).toInt();
    }

    this->loadStats(doc, 18, 2, employee_id);

    this->loadSkillTable(doc, 5, 10,employee_id, 1);
    this->loadSkillTable(doc, 15, 19,employee_id, 2);

    this->loadSkillTable(doc, 15, 28,employee_id, 3);
    this->loadSkillTable(doc, 15, 37,employee_id, 3);
    this->loadSkillTable(doc, 15, 46,employee_id, 3);
    this->loadSkillTable(doc, 15, 55,employee_id, 3);
    this->loadSkillTable(doc, 15, 64,employee_id, 3);

    this->loadSkillTable(doc, 69, 64,employee_id, 3);
    this->loadSkillTable(doc, 69, 64,employee_id, 3);
    this->loadSkillTable(doc, 69, 64,employee_id, 3);
    this->loadSkillTable(doc, 69, 64,employee_id, 3);
    this->loadSkillTable(doc, 69, 64,employee_id, 3);

    delete doc;
    return true;
}

bool ParseManager::loadPositionSkills() {
    QSqlQuery query(conn);

    QString strF = "INSERT INTO ul_employees (employee_id, skill_id, pillar_id, "
                   "position1, position2, position3, position4, position5, position6, position7, target) "
                   "VALUES(%1, %2, %3, %4, %5, %6, %7, %8, %9, %10, %11);";
    QString str = strF.arg(41)
                      .arg(43)
                      .arg(5)
                      .arg(1)
                      .arg(2)
                      .arg(3)
                      .arg(4)
                      .arg(5)
                      .arg(1)
                      .arg(2)
                      .arg(5);
    if (!query.exec(str)) {
        qDebug() << "Unable to make insert operation" << query.lastError();
    }
    return true;
}

bool ParseManager::loadStats(QXlsx::Document* doc, int row, int col, int empl_id) {
    QSqlQuery query(conn);
    while(doc->read(row,col).toString() != "") {
        QString category =  doc->read(row,col).toString();
        double luti =  doc->read(row,col+1).canConvert(QMetaType::Double) ?
                    doc->read(row,col+1).toDouble() : 0.0;
        double reactive =  doc->read(row,col+2).canConvert(QMetaType::Double) ?
                    doc->read(row,col+2).toDouble() : 0.0;
        double preventive =  doc->read(row,col+3).canConvert(QMetaType::Double) ?
                    doc->read(row,col+3).toDouble() : 0.0;
        double proactive =  doc->read(row,col+4).canConvert(QMetaType::Double) ?
                    doc->read(row,col+4).toDouble() : 0.0;
        double target =  doc->read(row,col+5).canConvert(QMetaType::Double) ?
                    doc->read(row,col+5).toDouble() : 0.0;
        double actual =  doc->read(row,col+6).canConvert(QMetaType::Double) ?
                    doc->read(row,col+6).toDouble() : 0.0;

        QString strF = "INSERT INTO ul_stats ( "
                       "employee_id, "
                       "category, "
                       "luti, "
                       "reactive, "
                       "preventive, "
                       "proactive, "
                       "target, "
                       "actual) "
                       "VALUES(%1, '%2', %3, %4, %5, %6, %7, %8);";
        QString str = strF.arg(empl_id)
                          .arg(category)
                          .arg(luti)
                          .arg(reactive)
                          .arg(preventive)
                          .arg(proactive)
                          .arg(target)
                          .arg(actual);
        if (!query.exec(str)) {
            qDebug() << "Unable to make insert operation" << query.lastError();
        }
        row++;
    }

    return true;
}

bool  ParseManager::loadSkillTable(QXlsx::Document* doc, int row, int col, int empl_id, int unknown_id) {
    QSqlQuery query(conn);
    while(doc->read(row,col).toString() != "") {
        QString skill_name = doc->read(row,col+1).toString();
        QString target = doc->read(row,col+2).toString();

        qDebug() << row << "  " << skill_name;

        QString req = "SELECT id FROM ul_skills WHERE LOWER(description)=LOWER('%1')";
        QString reqP = req.arg(skill_name.trimmed());
        int skill_id = 0;
        if (!query.exec(reqP)) {
            qDebug() << "Unable to select operation" << query.lastError();
        } else {
            query.next();
            skill_id = query.value(0).toInt();
        }

        QString pillar = doc->read(row,col).toString();
        req = "SELECT id FROM ul_pillars WHERE LOWER(name)=LOWER('%1')";
        reqP = req.arg(pillar);
        int pillar_id = 1;
        if (!query.exec(reqP)) {
            qDebug() << "Unable to select operation" << query.lastError();
        } else {
            query.next();
            pillar_id = query.value(0).toInt();
        }

        int mark2015_h2 = doc->read(row,col+3).canConvert(QMetaType::Int) ?
                    doc->read(row,col+3).toInt() : 0;
        int mark2015_h1 =  doc->read(row,col+4).canConvert(QMetaType::Int) ?
                    doc->read(row,col+4).toInt() : 0;
        int mark2014_h2 =  doc->read(row,col+5).canConvert(QMetaType::Int) ?
                    doc->read(row,col+5).toInt() : 0;
        int mark2014_h1 =  doc->read(row,col+6).canConvert(QMetaType::Int) ?
                    doc->read(row,col+6).toInt() : 0;
        int mark2013_h2 =  doc->read(row,col+7).canConvert(QMetaType::Int) ?
                    doc->read(row,col+7).toInt() : 0;



        QString strF = "INSERT INTO ul_employees_x_skills ( "
                       "employee_id, "
                       "skill_id, "
                       "pillar_id, "
                       "target, "
                       "m2013_H2, "
                       "m2014_H1, "
                       "m2014_H2, "
                       "m2015_H1, "
                       "m2015_H2, "
                       "unknown_id) "
                       "VALUES(%1, %2, %3, %4, %5, %6, %7, %8, %9, %10);";
        QString str = strF.arg(empl_id)
                          .arg(skill_id)
                          .arg(pillar_id)
                          .arg(target)
                          .arg(mark2015_h2)
                          .arg(mark2015_h1)
                          .arg(mark2014_h2)
                          .arg(mark2014_h1)
                          .arg(mark2013_h2)
                          .arg(unknown_id);
        if (!query.exec(str)) {
            qDebug() << "Unable to make insert operation" << query.lastError();
        }
        row++;
    }
    return true;
}

bool ParseManager::loadSkills() {
    QXlsx::Document* doc = new QXlsx::Document(fileName);
    QStringList sheets = doc->sheetNames();
    for (int i = 0; i < sheets.count(); i++) {
        qDebug() << "Sheet[" << i << "] = " << sheets[i];
    }
    doc->selectSheet(sheets[0]);
    int row = 2;
    while(doc->read(row,1).toString() != "") {
        QSqlQuery query(conn);
        QString req;
        QString reqP;
        QString number = doc->read(row,1).toString();
        QString level = doc->read(row,2).toString();
        req = "SELECT id FROM ul_skill_levels WHERE LOWER(name)=LOWER('%1')";
        reqP = req.arg(level);
        int level_id = 1;
        if (!query.exec(reqP)) {
            qDebug() << "Unable to select operation" << query.lastError();
        } else {
            query.next();
            level_id = query.value(0).toInt();
        }

        QString type = doc->read(row,3).toString();
        req = "SELECT id FROM ul_skill_types WHERE LOWER(name)=LOWER('%1')";
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
        req = "SELECT id FROM ul_pillars WHERE LOWER(name)=LOWER('%1')";
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
                       //"VALUES(':1', :2, :3, ':4', :5, ':6', ':7', ':8', ':9', ':10', ':11', ':12', ':13', ':14', ':15', ':16', ':17');";
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
                          .arg(suit)
                          .arg(she_eng)
                          .arg(env_eng)
                          .arg(train_coordinator)
                          .arg(finance_spec)
                          .arg(man_ex);
        /*
        query.prepare(strF);
        query.bindValue(":1", number);
        query.bindValue(":2", level_id);
        query.bindValue(":3", type_id);
        query.bindValue(":4", description);
        query.bindValue(":5", pillar_id);
        query.bindValue(":6", prod_proc_eng);
        query.bindValue(":7", shift_leader);
        query.bindValue(":8", logist_eng);
        query.bindValue(":9", technician);
        query.bindValue(":10", quality_eng);
        query.bindValue(":11", planning_eng);
        query.bindValue(":12", suit);
        query.bindValue(":13", she_eng);
        query.bindValue(":14", env_eng);
        query.bindValue(":15", train_coordinator);
        query.bindValue(":16", finance_spec);
        query.bindValue(":17", man_ex);
        */


        if (!query.exec(str)) {
            qDebug() << "ROW[" << row << "]='" << doc->read(row,1).toString() << "'";
            qDebug() << "QUERY: " << query.lastQuery();
            qDebug() << "Unable to make insert operation" << query.lastError();
        }
        row++;
    }

    delete doc;
    return true;
}

bool ParseManager::loadPillars() {
    int count = 11;
    QString pillars[11] = {"PM", "AM", "QC", "FI", "PD", "EEM", "CD", "S", "Env", "Log", "R&D"};

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
    QString pillars[3] = {"BASE", "INTERMEDIATE", "ADVANCED"};

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
    int count = 3;
    QString pillars[3] = {"TOOL", "METHOD", "SKILL"};

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
