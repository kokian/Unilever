#include "xlsemployeelistmodel.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>

XlsEmployeeListModel::XlsEmployeeListModel(QSqlDatabase* db, QWidget *parent)
    : QStandardItemModel(parent),
      connection(db)
{
    dbModel = new QSqlQueryModel(this);
}

XlsEmployeeListModel::~XlsEmployeeListModel()
{

}

int XlsEmployeeListModel::rowCount(const QModelIndex &parent) const {
    return QStandardItemModel::rowCount(parent);
}
int XlsEmployeeListModel::columnCount(const QModelIndex &parent) const {
    return QStandardItemModel::columnCount(parent);
}

QVariant XlsEmployeeListModel::data(const QModelIndex & index, int role) const {
    qDebug() << "data (" << index.row() << "," << index.column() << ")";

    if (!index.isValid()) {
        return QVariant();
    }
    switch(role) {
    case Qt::BackgroundColorRole:
    {
        return this->identifyBrush(index);
    }
    default:
        return QStandardItemModel::data(index, role);
    }
}

QBrush XlsEmployeeListModel::identifyBrush(const QModelIndex& index) const{
    QString value = index.model()->data(index).toString();

    QModelIndex code_index = index.model()->index(index.row(),5);
    QString code_value = index.model()->data(code_index).toString();

    QString query = QString("SELECT * FROM ul_employees WHERE code='%1'").arg(code_value);
    dbModel->setQuery(query, *connection);

    if(dbModel->rowCount() > 0) {
        QSqlRecord rec = dbModel->record(0);
        QString db_value = rec.field(index.column()).value().toString();
        if (db_value.compare(value) == 0) {
            return QBrush(Qt::gray);
        }
    }
    return QBrush(Qt::white);
}

