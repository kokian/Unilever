#include "skillsview.h"
#include "testcomboboxitemdelegate.h"
#include "constants.h"
#include <QLayout>
#include <QPushButton>
#include <QSqlRelationalTableModel>
#include <QSqlRelationalDelegate>

SkillsView::SkillsView(QWidget *parent) : QWidget(parent) {
}

SkillsView::SkillsView(QSqlDatabase* db, QWidget *parent): QWidget(parent), connection(db) {
    this->setWindowTitle("Skills");

    skillsTableView = new QTableView();
    skillsTableView->setItemDelegateForColumn(0, new TestComboBoxItemDelegate(connection, this));

    skillsTableView->setSortingEnabled(true);

    QLayout* skillsTableLayout = new QHBoxLayout();
    skillsTableLayout->addWidget(skillsTableView);

    this->setLayout(skillsTableLayout);

    QSqlRelationalTableModel* model = new QSqlRelationalTableModel(this, *connection);
    model->setTable("ul_skills");
    model->setRelation(2, QSqlRelation("ul_skill_levels", "id", "name"));
    model->setRelation(3, QSqlRelation("ul_skill_types", "id", "name"));
    model->setRelation(5, QSqlRelation("ul_pillars", "id", "name"));
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("NO"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("LEVEL"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("TYPE"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("DESCRIPTION"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("PILLAR"));
    skillsTableView->setModel(model);
    skillsTableView->setItemDelegate(new QSqlRelationalDelegate(skillsTableView));
    model->select();
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    //skillsTableView->setItemDelegateForColumn(0, new TestComboBoxItemDelegate(connection, this));
}

SkillsView::~SkillsView()
{

}

