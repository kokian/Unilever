#ifndef TESTCOMBOBOXITEMDELEGATE_H
#define TESTCOMBOBOXITEMDELEGATE_H

#include <QObject>
#include <QItemDelegate>
#include <QModelIndex>
#include <QSqlQueryModel>

class TestComboBoxItemDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit TestComboBoxItemDelegate(QWidget *parent = 0);
    TestComboBoxItemDelegate(QSqlDatabase* db, QWidget *parent = 0);
    ~TestComboBoxItemDelegate();

    QSqlQueryModel* query;
    QSqlDatabase* connection;

    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;


private:

signals:
public slots:
};

#endif // TESTCOMBOBOXITEMDELEGATE_H
