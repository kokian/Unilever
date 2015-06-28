#ifndef XLSEMPLOYEELISTMODEL_H
#define XLSEMPLOYEELISTMODEL_H

#include <QWidget>
#include <QStandardItemModel>
#include <QSqlQueryModel>
#include <QSqlDatabase>

class XlsEmployeeListModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit XlsEmployeeListModel(QSqlDatabase* db, QWidget *parent = 0);
    ~XlsEmployeeListModel();
    QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;

private:
    QSqlQueryModel* dbModel;
    QSqlDatabase* connection;
    QBrush identifyBrush(const QModelIndex& index) const;
signals:

public slots:
};

#endif // XLSEMPLOYEELISTMODEL_H
