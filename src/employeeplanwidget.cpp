#include "employeeplanwidget.h"

EmployeePlanWidget::EmployeePlanWidget(QWidget *parent) : QWidget(parent) {
    treeView = new QTreeView(this);
    model = new QStandardItemModel();

    QList<QStandardItem *> preparedRow = prepareRow("first", "second", "third");
    QStandardItem *item = model->invisibleRootItem();
    // adding a row to the invisible root item produces a root element
    item->appendRow(preparedRow);

    QList<QStandardItem *> secondRow = prepareRow("111", "222", "333");
    // adding a row to an item starts a subtree
    preparedRow.first()->appendRow(secondRow);

    treeView->setModel(model);
    treeView->expandAll();
}

EmployeePlanWidget::~EmployeePlanWidget()
{

}

QList<QStandardItem *> EmployeePlanWidget::prepareRow(const QString &first,
                                                const QString &second,
                                                const QString &third)
{
    QList<QStandardItem *> rowItems;
    rowItems << new QStandardItem(first);
    rowItems << new QStandardItem(second);
    rowItems << new QStandardItem(third);
    return rowItems;
}

