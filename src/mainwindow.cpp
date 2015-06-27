#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTableView.h>
#include <QLayout>
#include <QPushButton>

#include <QFileDialog>

#include "qchartviewer.h"

#include "employeeview.h"
#include "employeelistwidget.h"
#include "itemcolordelegate.h"
#include "employeeplanwidget.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    dbManager = new DBManager("188.166.126.89", 5432, "QPSQL", "postgres", "postgres", "74507", this);
    dbManager->connect();

    QTabWidget* tabWidget = new QTabWidget();
    this->setCentralWidget(tabWidget);

    QString titles[] = {"People", "Skills", "Personal Card", "Personal Radar", "Personal Plan"};

    for(int i = 0; i < (int)VIEW_COUNT; i++) {
        QWidget* wdg = this->createView((VIEW_NAME)i);
        tabWidget->addTab(wdg,titles[i]);
    }
}

void MainWindow::addPersonButtonClicked() {
    dbManager->insertEmployee();
    employeeModel->select();
    tableView->scrollToBottom();
}

void MainWindow::removePersonButtonClicked() {
    QModelIndex index = tableView->selectionModel()->currentIndex();
    int row = index.row();
    int id = index.sibling(row,0).data().toInt();
    dbManager->deleteEmployee(id);
    employeeModel->select();
}

BaseChart* MainWindow::stackradar(int, const char **imageMap)
{
    // The data for the chart
    double data0[] = {100, 100, 100, 100, 100, 100};
    double data1[] = {90, 85, 85, 80, 70, 60};
    double data2[] = {80, 65, 65, 75, 45, 30};

    // The labels for the chart
    const char *labels[] = {"Population<*br*><*font=arial.ttf*>6 millions",
        "GDP<*br*><*font=arial.ttf*>120 billions",
        "Export<*br*><*font=arial.ttf*>25 billions",
        "Import<*br*><*font=arial.ttf*>24 billions",
        "Investments<*br*><*font=arial.ttf*>20 billions",
        "Others<*br*><*font=arial.ttf*>10 billions"};

    // Create a PolarChart object of size 480 x 460 pixels. Set background color to
    // silver, with 1 pixel 3D border effect
    PolarChart *c = new PolarChart(480, 460, Chart::silverColor(), 0x000000, 1);

    // Add a title to the chart using 15 pts Times Bold Italic font. The title text
    // is white (ffffff) on a deep green (008000) background
    c->addTitle("Economic Growth", "timesbi.ttf", 15, 0xffffff)->setBackground(
        0x008000);

    // Set plot area center at (240, 270), with 150 pixels radius
    c->setPlotArea(240, 270, 150);

    // Use 1 pixel width semi-transparent black (c0000000) lines as grid lines
    c->setGridColor(0xc0000000, 1, 0xc0000000, 1);

    // Add a legend box at top-center of plot area (240, 35) using horizontal layout.
    // Use 10 pts Arial Bold font, with silver background and 1 pixel 3D border
    // effect.
    LegendBox *b = c->addLegend(240, 35, false, "arialbd.ttf", 10);
    b->setAlignment(Chart::TopCenter);
    b->setBackground(Chart::silverColor(), Chart::Transparent, 1);

    // Add area layers of different colors to represent the data
    c->addAreaLayer(DoubleArray(data0, (int)(sizeof(data0) / sizeof(data0[0]))),
        0xcc8880, "Year 2004");
    c->addAreaLayer(DoubleArray(data1, (int)(sizeof(data1) / sizeof(data1[0]))),
        0xffd080, "Year 1994");
    c->addAreaLayer(DoubleArray(data2, (int)(sizeof(data2) / sizeof(data2[0]))),
        0xa0bce0, "Year 1984");

    // Set the labels to the angular axis as spokes.
    c->angularAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(
        labels[0]))));

    // Set radial axis from 0 - 100 with a tick every 20 units
    c->radialAxis()->setLinearScale(0, 100, 20);

    // Just show the radial axis as a grid line. Hide the axis labels by setting the
    // label color to Transparent
    c->radialAxis()->setColors(0xc0000000, Chart::Transparent);

    // Output the chart
    c->makeChart();

    //include tool tip for the chart
    *imageMap = c->getHTMLImageMap("clickable", "",
        "title='Current {label}: {value}% in {dataSetName}'");

    return c;
}


MainWindow::~MainWindow()
{
    delete ui;
}

QWidget* MainWindow::createView(VIEW_NAME vName) {
    QWidget* view = 0;
    switch(vName) {
    case VIEW_EMPLOYEES:
        view = this->createEmployeesView();
        break;
    case VIEW_SKILLS:
        view = this->createSkillsView();
        break;
    case VIEW_EMPLOYEE_CARD:
        view = this->createEmployeeCardView();
        break;
    case VIEW_EMPLOYEE_RADAR:
        view = this->createEmployeeRadarView();
        break;
    case VIEW_EMPLOYEE_PLAN:
        view = this->createEmployeePlanView();
        break;
    default:
        view = new QWidget();
        break;
    }
    return view;
}

QWidget* MainWindow::createEmployeesView() {
    return new EmployeeListWidget(dbManager, this);
}

QWidget* MainWindow::createSkillsView() {
    QWidget* skillsView = new QWidget();
    skillsView->setWindowTitle("Skills");

    QTableView* skillsTableView = new QTableView();
    skillsTableView->setItemDelegate(new ItemColorDelegate());

    skillsTableView->setSortingEnabled(true);

    QLayout* skillsTableLayout = new QHBoxLayout();
    skillsTableLayout->addWidget(skillsTableView);

    QLayout* skillsButtonLayout = new QHBoxLayout();
    QPushButton* addSkill = new QPushButton();
    addSkill->setText("Добавить");
    addSkill->resize(100, addSkill->height());
    QPushButton* removeSkill = new QPushButton();
    removeSkill->setText("Удалить");
    removeSkill->resize(100, removeSkill->height());
    skillsButtonLayout->addWidget(addSkill);
    skillsButtonLayout->addWidget(removeSkill);

    QLayout* skillsLayout = new QVBoxLayout();
    skillsLayout->addItem(skillsButtonLayout);
    skillsLayout->addItem(skillsTableLayout);

    skillsView->setLayout(skillsLayout);

    skillsTableView->setModel(dbManager->initModel(ENT_SKILLS));
    skillsTableView->setItemDelegate(new QSqlRelationalDelegate(skillsTableView));
    return skillsView;
}

QWidget* MainWindow::createEmployeeCardView() {
    return new EmployeeView(dbManager);
}

QWidget* MainWindow::createEmployeeRadarView() {
    QWidget* personalView = new QWidget();

    /*
    personalView->setWindowTitle("Employee Radar");

    personalView->resize(250, 250);
    //personalView->setStyleSheet("background-color:black;");
    QLayout* rightVLayout = new QVBoxLayout();

    rightVLayout->addWidget(this->drawRadar(wcmToolsModel));
    rightVLayout->addWidget(this->drawStatsRadar(statsModel));

    changePillarComboBox = new QComboBox();
    QSqlTableModel *rel = wcmToolsModel->relationModel(pillar_index);
    changePillarComboBox->setModel(rel);
    changePillarComboBox->setModelColumn(rel->fieldIndex("name"));
    rightVLayout->addWidget(changePillarComboBox);

    skillSetTableView = new QTableView();
    skillSetTableView->setModel(wcmToolsModel);
    skillSetTableView->setColumnHidden(0, true);
    skillSetTableView->setColumnHidden(1, true);
    skillSetTableView->setColumnHidden(5, true);
    rightVLayout->addWidget(skillSetTableView);
    */
    return personalView;
}

QWidget* MainWindow::createEmployeePlanView() {
    return new EmployeePlanWidget();
}

void MainWindow::on_action_triggered()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setNameFilter(tr("Excel files (*.xslx)"));
    dialog.setViewMode(QFileDialog::Detail);
    QStringList fileNames;
    if (dialog.exec()) {
        fileNames = dialog.selectedFiles();
    }
    if(fileNames.count() > 0) {
        this->fileName = fileNames[0];
    }

}
