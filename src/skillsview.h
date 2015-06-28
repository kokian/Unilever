#ifndef SKILLSVIEW_H
#define SKILLSVIEW_H

#include <QWidget>
#include <QTableView>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QStandardItemModel>

class SkillsView : public QWidget
{
    Q_OBJECT
public:
    explicit SkillsView(QWidget *parent = 0);
    SkillsView(QSqlDatabase* db, QWidget *parent = 0);
    ~SkillsView();

    QSqlDatabase* connection;

private:
    QTableView* skillsTableView;
signals:

public slots:
};

#endif // SKILLSVIEW_H
