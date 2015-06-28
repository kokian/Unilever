#include "testcomboboxitemdelegate.h"
#include <QComboBox>
#include <QDebug>
#include <QStandardItemModel>
#include <QSqlRecord>

TestComboBoxItemDelegate::TestComboBoxItemDelegate(QWidget *parent) : QItemDelegate(parent) {
    query = new QSqlQueryModel(this);
}

TestComboBoxItemDelegate::TestComboBoxItemDelegate(QSqlDatabase* db, QWidget *parent) : QItemDelegate(parent), connection(db) {
    query = new QSqlQueryModel(this);
}

TestComboBoxItemDelegate::~TestComboBoxItemDelegate() {
    delete query;
}

QWidget* TestComboBoxItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QComboBox* editor = new QComboBox(parent);
    query->setQuery("SELECT * FROM ul_skill_levels", *connection);
    editor->setModel(query);
    editor->setModelColumn(1);

    return editor;
}

void TestComboBoxItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
    QString value = index.model()->data(index, Qt::EditRole).toString();
    const QStandardItemModel* model = static_cast<const QStandardItemModel*>(index.model());
    QList<QStandardItem*> results = model->findItems("Senior", Qt::MatchContains, 2);
    foreach(QStandardItem* item, results) {
        qDebug() << item->data(Qt::DisplayRole);
    }
    QComboBox *cBox = static_cast<QComboBox*>(editor);
    cBox->setCurrentIndex(cBox->findText(value));
}

void TestComboBoxItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
    QComboBox *cBox = static_cast<QComboBox*>(editor);
    QSqlRecord record = query->record(cBox->currentIndex());
    int level_id = record.value(0).toInt();
    QString level_name = record.value(1).toString();

    model->setData(index, level_name, Qt::EditRole);
    model->setData(index, level_id, Qt::UserRole);
}

void TestComboBoxItemDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    editor->setGeometry(option.rect);
}
