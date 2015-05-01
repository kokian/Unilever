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

    bool loadEmployeeList();
    bool loadPillars();
    bool loadSkillLevels();
    bool loadSkillTypes();
    bool loadSkills();

    ~ParseManager();
private:
    QString fileName;
    QSqlDatabase conn;
};

#endif // PARSEMANAGER_H
