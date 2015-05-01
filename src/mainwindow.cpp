#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTableView.h>
#include <QLayout>
#include <QPushButton>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    dbManager = new DBManager("localhost", 5432, "QPSQL", "Kirill", "Kirill", "", this);
    dbManager->connect();

    QTabWidget* tabWidget = new QTabWidget();
    this->setCentralWidget(tabWidget);

    tableView = new QTableView();
    tableView->setSortingEnabled(true);
    //tableView->setHorizontalHeaderLabels({"Employee Name(LEG)", "Job Title 4Doc (RUS)", "Position Title", "Department", "HIRE DATE", "ID CODE", "NAME", "Reported by"});

    QWidget* peopleView = new QWidget();

    QLayout* tableLayout = new QHBoxLayout();
    tableLayout->addWidget(tableView);

    QLayout* buttonLayout = new QHBoxLayout();
    QPushButton* addPerson = new QPushButton();
    addPerson->setText(" + ");
    addPerson->resize(20, 20);
    addPerson->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    connect (addPerson, SIGNAL( clicked() ), this, SLOT( addPersonButtonClicked() ) );
    QPushButton* removePerson = new QPushButton();
    removePerson->setText(" - ");
    removePerson->resize(20, 20);
    removePerson->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    connect (removePerson, SIGNAL( clicked() ), this, SLOT( removePersonButtonClicked() ) );
    buttonLayout->addWidget(addPerson);
    buttonLayout->addWidget(removePerson);
    buttonLayout->setAlignment(Qt::AlignLeft);

    QLayout* peopleLayout = new QVBoxLayout();
    peopleLayout->addItem(buttonLayout);
    peopleLayout->addItem(tableLayout);

    peopleView->setLayout(peopleLayout);

    employeeModel = dbManager->initModel(ENT_EMPLOYEE);
    tableView->setModel(employeeModel);

//--------

    QTableView* skillsTableView = new QTableView();
    skillsTableView->setSortingEnabled(true);
    QWidget* skillsView = new QWidget();

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

    QWidget* personalView = new QWidget();
    personalView->resize(250, 250);
    personalView->setStyleSheet("background-color:black;");


    tabWidget->addTab(peopleView, "Employees");
    tabWidget->addTab(skillsView, "Skills");
    tabWidget->addTab(personalView, "Employee Card");
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

BaseChart* MainWindow::stackradar(int /* img */, const char **imageMap)
{
    // The data for the chart
    double data0[] = {100, 100, 100, 100, 100};
    double data1[] = {90, 85, 85, 80, 70};
    double data2[] = {80, 65, 65, 75, 45};

    // The labels for the chart
    const char *labels[] = {"Population<*br*><*font=arial.ttf*>6 millions",
        "GDP<*br*><*font=arial.ttf*>120 billions",
        "Export<*br*><*font=arial.ttf*>25 billions",
        "Import<*br*><*font=arial.ttf*>24 billions",
        "Investments<*br*><*font=arial.ttf*>20 billions"};

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
