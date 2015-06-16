#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "dbmanager.h"
#include "constants.h"
#include "parsemanager.h"
#include <QTableView>

#include "chartdir.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    QString fileName;
    BaseChart* stackradar(int /* img */, const char **imageMap);
    QWidget* createView(VIEW_NAME vName);
    ~MainWindow();

private:
    DBManager* dbManager;
    ParseManager* parseManager;

    QSqlTableModel* employeeModel;
    QTableView* tableView;

    QWidget* createEmployeesView();
    QWidget* createSkillsView();
    QWidget* createEmployeeCardView();
    QWidget* createEmployeePlanView();
    QWidget* createEmployeeRadarView();

    Ui::MainWindow *ui;

public slots:
    void addPersonButtonClicked();
    void removePersonButtonClicked();

private slots:
    void on_action_triggered();
};

#endif // MAINWINDOW_H
