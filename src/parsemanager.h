#ifndef PARSEMANAGER_H
#define PARSEMANAGER_H

#include <QObject>
#include <QtCore>
#include <QtXlsx>
#include <QtSql>

class ParseManager : public QObject {
    Q_OBJECT
public:
    ParseManager(QObject *parent = 0);
    ParseManager(QObject *parent, QSqlDatabase conn);

    QString fileName;

    bool loadEmployeeList();
    bool loadPillars();
    bool loadSkillLevels();
    bool loadSkillTypes();
    bool loadSkills();
    bool loadEmployeeCard();

    ~ParseManager();
private:
    QSqlDatabase conn;

    bool loadSkillTable(QXlsx::Document* doc, int row, int col, int empl_id, int unknown_id);
    bool loadStats(QXlsx::Document* doc, int row, int col, int empl_id);
};

#endif // PARSEMANAGER_H
