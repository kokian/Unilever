#ifndef EMPLOYEELISTWIDGET_H
#define EMPLOYEELISTWIDGET_H

#include <QWidget>

#include <QTableView.h>
#include <QLayout>
#include <QPushButton>
#include <QFileDialog>

#include "dbmanager.h"
#include "constants.h"
#include "parsemanager.h"

class EmployeeListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit EmployeeListWidget(QWidget *parent = 0);
    EmployeeListWidget(DBManager* dbManager, QWidget *parent = 0);
    ~EmployeeListWidget();
    QTableView* tableView;
    QPushButton* buttonAddPerson;
    QPushButton* buttonRemovePerson;
    QPushButton* buttonLoadList;
    QPushButton* buttonSaveChanges;
    QSqlTableModel* employeeListModel;
private:
    DBManager* db;
signals:

public slots:
    void addPersonButtonClicked();
    void removePersonButtonClicked();
    void loadListButtonClicked();
    void saveListButtonClicked();
};

#endif // EMPLOYEELISTWIDGET_H
