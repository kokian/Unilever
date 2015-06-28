#include "employeelistwidget.h"

#include <QMessageBox>
#include <QHeaderView>

EmployeeListWidget::EmployeeListWidget(QWidget *parent) : QWidget(parent) {

}

EmployeeListWidget::EmployeeListWidget(DBManager* dbManager, QWidget *parent) {
    db = dbManager;
    this->setWindowTitle("Employees");

    tableView = new QTableView();
    tableView->setSortingEnabled(true);

    QLayout* tableLayout = new QHBoxLayout();
    tableLayout->addWidget(tableView);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonAddPerson = new QPushButton();
    buttonAddPerson->setText(" + ");
    buttonAddPerson->resize(20, 20);
    buttonAddPerson->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    connect (buttonAddPerson, SIGNAL( clicked() ), this, SLOT( addPersonButtonClicked() ) );

    buttonRemovePerson = new QPushButton();
    buttonRemovePerson->setText(" - ");
    buttonRemovePerson->resize(20, 20);
    buttonRemovePerson->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    connect (buttonRemovePerson, SIGNAL( clicked() ), this, SLOT( removePersonButtonClicked() ) );

    buttonLoadList = new QPushButton();
    buttonLoadList->setText("load");
    buttonLoadList->resize(20, 20);
    buttonLoadList->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    connect (buttonLoadList, SIGNAL( clicked() ), this, SLOT( loadListButtonClicked() ) );

    buttonSaveChanges = new QPushButton();
    buttonSaveChanges->setText("save");
    buttonSaveChanges->resize(20, 20);
    buttonSaveChanges->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    connect (buttonSaveChanges, SIGNAL( clicked() ), this, SLOT( saveListButtonClicked() ) );

    buttonLayout->addWidget(buttonAddPerson);
    buttonLayout->addWidget(buttonRemovePerson);
    buttonLayout->addStretch();
    buttonLayout->addWidget(buttonLoadList);
    buttonLayout->addWidget(buttonSaveChanges);
    buttonLayout->setAlignment(Qt::AlignLeft);

    QLayout* peopleLayout = new QVBoxLayout();
    peopleLayout->addItem(buttonLayout);
    peopleLayout->addItem(tableLayout);

    this->setLayout(peopleLayout);

    employeeListModel = db->initModel(ENT_EMPLOYEE);
    tableView->setModel(employeeListModel);
    int columns = tableView->horizontalHeader()->count();
    for (int c = 0; c < columns; c++) {
        tableView->horizontalHeader()->setSectionResizeMode(c, QHeaderView::Stretch);
    }
}

EmployeeListWidget::~EmployeeListWidget() {

}

void EmployeeListWidget::addPersonButtonClicked() {
    db->insertEmployee();
    employeeListModel->select();
    tableView->scrollToBottom();
}

void EmployeeListWidget::removePersonButtonClicked() {
    QModelIndex index = tableView->selectionModel()->currentIndex();
    int row = index.row();
    int id = index.sibling(row,0).data().toInt();
    db->deleteEmployee(id);
    employeeListModel->select();
}

void EmployeeListWidget::loadListButtonClicked() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Excel file"), "", tr("Excel Files (*.xlsx)"));
    ParseManager* p = new ParseManager(this, db->connection);
    p->fileName = fileName;
    QStandardItemModel* model = p->loadEmployeeListToModel();
    //test_delegate = new TestComboBoxItemDelegate(&db->connection,this);
    tableView->setModel(model);
    //tableView->setItemDelegate(test_delegate);

    delete p;
    /*
    QMessageBox msgBox;
    msgBox.setText("The document has wrong format.");
    msgBox.exec();
    */
}

void EmployeeListWidget::saveListButtonClicked() {

}
