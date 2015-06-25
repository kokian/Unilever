#ifndef EMPLOYEEPLANWIDGET_H
#define EMPLOYEEPLANWIDGET_H

#include <QWidget>
#include <QTreeView>
#include <QStandardItemModel>

class EmployeePlanWidget : public QWidget
{
    Q_OBJECT
public:
    explicit EmployeePlanWidget(QWidget *parent = 0);
    ~EmployeePlanWidget();

    QTreeView* treeView;
    QStandardItemModel* model;

private:
    QList<QStandardItem *> prepareRow(const QString &first,
                                                    const QString &second,
                                                    const QString &third);

signals:

public slots:
};

#endif // EMPLOYEEPLANWIDGET_H
