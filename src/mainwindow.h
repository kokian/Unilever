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
    BaseChart* stackradar(int /* img */, const char **imageMap);
    ~MainWindow();
public slots:
    void addPersonButtonClicked();
    void removePersonButtonClicked();
private:
    DBManager* dbManager;
    ParseManager* parseManager;

    QSqlTableModel* employeeModel;
    QTableView* tableView;

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
