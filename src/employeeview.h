#ifndef EMPLOYEEVIEW_H
#define EMPLOYEEVIEW_H

#include <QWidget>
//#include "ui_employeecard.h"
#include "dbmanager.h"
#include <QTableView>
#include <QLayout>

#include "chartdir.h"
#include "qchartviewer.h"

class EmployeeView : public QWidget
{
    Q_OBJECT
public:
    explicit EmployeeView(QWidget *parent = 0);
    EmployeeView(DBManager* db, QWidget *parent = 0);
    BaseChart* stackradar(int, const char **imageMap, QSqlRelationalTableModel* model);
    BaseChart* statsradar(int, const char **imageMap, QSqlTableModel* model);
    QChartViewer* drawRadar(QSqlRelationalTableModel* model);
    QChartViewer* drawStatsRadar(QSqlTableModel* model);
    ~EmployeeView();

signals:

public slots:
    void switchcall(const QString&);

private:
    DBManager* dbManager;

    QTableView* skillSetTableView;
    QComboBox* changePillarComboBox;
    QChartViewer* radarWidget;

    QWidget* createInfoWidget();
    QWidget* createStatWidget();

    QSqlRelationalTableModel* wcmToolsModel;

    //Ui::EmployeeCard* ui;
};

#endif // EMPLOYEEVIEW_H
