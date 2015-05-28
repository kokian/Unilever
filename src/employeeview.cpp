#include "employeeview.h"

#include <QtSql>
#include "constants.h"
#include <QDataWidgetMapper>
#include <QTableWidget>
#include <QHeaderView>

#include "string.h"

EmployeeView::EmployeeView(DBManager *db, QWidget *parent) :
    QWidget(parent)
    //ui(new Ui::EmployeeCard)
{
    dbManager = db;
    //ui->setupUi(this);
    QSqlQueryModel* pillarModel = new QSqlQueryModel();
    pillarModel->setQuery("select name from ui_pillars", dbManager->connection);


    QSqlRelationalTableModel* wcmToolsModel = new QSqlRelationalTableModel(0, dbManager->connection);
    wcmToolsModel->setTable("ul_employees_x_skills");
    wcmToolsModel->setFilter("employee_id=41");
    int skill_index = wcmToolsModel->fieldIndex("skill_id");
    int pillar_index = wcmToolsModel->fieldIndex("pillar_id");
    wcmToolsModel->setRelation(skill_index, QSqlRelation("ul_skills", "id", "description"));
    wcmToolsModel->setRelation(pillar_index, QSqlRelation("ul_pillars", "id", "name"));

    wcmToolsModel->select();
    wcmToolsModel->setEditStrategy(QSqlTableModel::OnFieldChange);

    QLayout* mainHLayout = new QHBoxLayout();
    QLayout* leftVLayout = new QVBoxLayout();

    leftVLayout->addWidget(this->createInfoWidget());
    leftVLayout->addWidget(this->createStatWidget());
    QSqlTableModel* statsModel = new QSqlTableModel(0, dbManager->connection);
    statsModel->setTable("ul_stats");
    statsModel->select();
    leftVLayout->addWidget(this->drawStatsRadar(statsModel));

    mainHLayout->addItem(leftVLayout);

    QLayout* rightVLayout = new QVBoxLayout();

    rightVLayout->addWidget(this->drawRadar(wcmToolsModel));

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
    mainHLayout->addItem(rightVLayout);



    this->setLayout(mainHLayout);
}

BaseChart* EmployeeView::stackradar(int, const char **imageMap, QSqlRelationalTableModel *model)
{
    // The data for the chart
    //double data0[] = {100, 100, 100, 100, 100, 100};
    //double data1[] = {90, 85, 85, 80, 70, 60};
    //double data2[] = {80, 65, 65, 75, 45, 30};
    model->setFilter("unknown_id=1");
    int count = model->rowCount();
    double** data = new double*[6];
    for(int j = 0; j < 6; j++)
        data[j] = new double[count];
    char** labels = new char*[count];

    int skill_index = model->fieldIndex("description");
    int unknown_index = model->fieldIndex("unknown_id");
    int target_index = model->fieldIndex("target");
    int m2013_H2_index = model->fieldIndex("m2013_H2");
    int m2014_H1_index = model->fieldIndex("m2014_H1");
    int m2014_H2_index = model->fieldIndex("m2014_H2");
    int m2015_H1_index = model->fieldIndex("m2015_H1");
    int m2015_H2_index = model->fieldIndex("m2015_H2");

    qDebug() << count;

    for(int i = 0; i < count; i++) {
        QSqlRecord rec = model->record(i);
        data[0][i] = rec.value(target_index).toDouble();
        data[1][i] = rec.value(m2013_H2_index).toDouble();
        data[2][i] = rec.value(m2014_H1_index).toDouble();
        data[3][i] = rec.value(m2014_H2_index).toDouble();
        data[4][i] = rec.value(m2015_H1_index).toDouble();
        data[5][i] = rec.value(m2015_H2_index).toDouble();

        const char* src = rec.value(skill_index).toString().toStdString().c_str();
        labels[i] = new char[strlen(src)];
        strcpy(labels[i], src);

        qDebug() << labels[i] << ":" << data[0][i] << "," << data[1][i] << "," << data[2][i] << ","
                               << data[3][i] << "," << data[4][i] << "," << data[5][i];
    }


    // The labels for the chart
    /*
    const char *labels[] = {"Population<*br*><*font=arial.ttf*>6 millions",
        "GDP<*br*><*font=arial.ttf*>120 billions",
        "Export<*br*><*font=arial.ttf*>25 billions",
        "Import<*br*><*font=arial.ttf*>24 billions",
        "Investments<*br*><*font=arial.ttf*>20 billions",
        "Others<*br*><*font=arial.ttf*>10 billions"};
    */
    // Create a PolarChart object of size 480 x 460 pixels. Set background color to
    // silver, with 1 pixel 3D border effect
    PolarChart *c = new PolarChart(500, 500, Chart::silverColor(), 0x000000, 1);

    // Add a title to the chart using 15 pts Times Bold Italic font. The title text
    // is white (ffffff) on a deep green (008000) background
    c->addTitle("Skills Radar", "timesbi.ttf", 15, 0xffffff)->setBackground(
        0x008000);

    // Set plot area center at (240, 270), with 150 pixels radius
    c->setPlotArea(275, 300, 150);

    // Use 1 pixel width semi-transparent black (c0000000) lines as grid lines
    c->setGridColor(0xc0000000, 1, 0xc0000000, 1);

    // Add a legend box at top-center of plot area (240, 35) using horizontal layout.
    // Use 10 pts Arial Bold font, with silver background and 1 pixel 3D border
    // effect.
    LegendBox *b = c->addLegend(240, 35, false, "arialbd.ttf", 8);
    b->setAlignment(Chart::TopCenter);
    b->setBackground(Chart::silverColor(), Chart::Transparent, 1);

    // Add area layers of different colors to represent the data
    c->addAreaLayer(DoubleArray(data[0], count),
        0xcc8880, "Target");
    c->addAreaLayer(DoubleArray(data[1], count),
        0xffd080, "2013_H2");
    c->addAreaLayer(DoubleArray(data[2], count),
        0xa0bce0, "2014_H1");
    c->addAreaLayer(DoubleArray(data[3], count),
        0xc7f5a0, "2014_H2");
    c->addAreaLayer(DoubleArray(data[4], count),
        0x81daf0, "2015_H1");
    c->addAreaLayer(DoubleArray(data[5], count),
        0xe429c0, "2015_H2");

    // Set the labels to the angular axis as spokes.
    c->angularAxis()->setLabels(StringArray(labels, count));
    c->angularAxis()->setLabelStyle("bold", 7);

    // Set radial axis from 0 - 100 with a tick every 20 units
    c->radialAxis()->setLinearScale(0, 5, 1);

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

BaseChart* EmployeeView::statsradar(int, const char **imageMap, QSqlTableModel *model)
{
    // The data for the chart
    int count = model->rowCount();
    double** data = new double*[2];
    for(int j = 0; j < 2; j++)
        data[j] = new double[count];
    char** labels = new char*[count];

    int category_index = model->fieldIndex("category");
    int target_index = model->fieldIndex("target");
    int actual_index = model->fieldIndex("actual");
    qDebug() << count;

    for(int i = 0; i < count; i++) {
        QSqlRecord rec = model->record(i);
        data[0][i] = rec.value(target_index).toDouble();
        data[1][i] = rec.value(actual_index).toDouble();

        const char* src = rec.value(category_index).toString().toStdString().c_str();
        labels[i] = new char[strlen(src)];
        strcpy(labels[i], src);

        qDebug() << labels[i] << ":" << data[0][i] << "," << data[1][i];
    }

    // Create a PolarChart object of size 480 x 460 pixels. Set background color to
    // silver, with 1 pixel 3D border effect
    PolarChart *c = new PolarChart(300, 300, Chart::silverColor(), 0x000000, 1);

    // Add a title to the chart using 15 pts Times Bold Italic font. The title text
    // is white (ffffff) on a deep green (008000) background
    c->addTitle("Skills Radar", "timesbi.ttf", 15, 0xffffff)->setBackground(
        0x008000);

    // Set plot area center at (240, 270), with 150 pixels radius
    c->setPlotArea(159, 150, 90);

    // Use 1 pixel width semi-transparent black (c0000000) lines as grid lines
    c->setGridColor(0xc0000000, 1, 0xc0000000, 1);

    // Add a legend box at top-center of plot area (240, 35) using horizontal layout.
    // Use 10 pts Arial Bold font, with silver background and 1 pixel 3D border
    // effect.
    LegendBox *b = c->addLegend(240, 35, false, "arialbd.ttf", 8);
    b->setAlignment(Chart::TopCenter);
    b->setBackground(Chart::silverColor(), Chart::Transparent, 1);

    // Add area layers of different colors to represent the data
    c->addAreaLayer(DoubleArray(data[0], count),
        0xcc8880, "Target");
    c->addAreaLayer(DoubleArray(data[1], count),
        0xffd080, "Actual");

    // Set the labels to the angular axis as spokes.
    c->angularAxis()->setLabels(StringArray(labels, count));
    c->angularAxis()->setLabelStyle("bold", 7);

    // Set radial axis from 0 - 100 with a tick every 20 units
    c->radialAxis()->setLinearScale(0, 5, 1);

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

QChartViewer* EmployeeView::drawRadar(QSqlRelationalTableModel *model) {
    QChartViewer* chartViewer = new QChartViewer();
    const char *imageMap = 0;
    chartViewer->setChart(this->stackradar(0, &imageMap, model));
    chartViewer->setImageMap(imageMap);
    return chartViewer;
}

QChartViewer* EmployeeView::drawStatsRadar(QSqlTableModel *model) {
    QChartViewer* chartViewer = new QChartViewer();
    const char *imageMap = 0;
    chartViewer->setChart(this->statsradar(0, &imageMap, model));
    chartViewer->setImageMap(imageMap);
    return chartViewer;
}

QWidget* EmployeeView::createInfoWidget() {
    QTableWidget* infoTable = new QTableWidget(10, 2);
    infoTable->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    infoTable->resize(500, 500);
    infoTable->verticalHeader()->hide();
    infoTable->horizontalHeader()->hide();

    infoTable->setColumnWidth(0, 100);
    infoTable->setColumnWidth(1, 100);

    infoTable->setCellWidget(0,0,new QLabel("LAST UPDATED"));
    infoTable->setCellWidget(0,1,new QLabel("31.01.2015"));

    infoTable->setCellWidget(1,0,new QLabel("NAME"));
    infoTable->setCellWidget(1,1,new QLabel("Sergey Logunov"));

    infoTable->setCellWidget(2,0,new QLabel("ID CODE"));
    infoTable->setCellWidget(2,1,new QLabel("000455466"));

    infoTable->setCellWidget(3,0,new QLabel("HIRE DATE"));
    infoTable->setCellWidget(3,1,new QLabel("15.03.2005"));

    infoTable->setCellWidget(4,0,new QLabel("CURRENT POSITION"));
    infoTable->setCellWidget(4,1,new QLabel("Process Engineer"));

    infoTable->setCellWidget(5,0,new QLabel("DEPARTMENT"));
    infoTable->setCellWidget(5,1,new QLabel("Tula Production department"));

    infoTable->setCellWidget(6,0,new QLabel("EX POSITION"));
    infoTable->setCellWidget(6,1,new QLabel("31.01.2015"));

    infoTable->setCellWidget(7,0,new QLabel("MAIN PILLAR"));
    infoTable->setCellWidget(7,1,new QLabel("AM"));

    infoTable->setCellWidget(8,0,new QLabel("OTHER PILLARS"));
    infoTable->setCellWidget(8,1,new QLabel("QC"));

    infoTable->setCellWidget(9,0,new QLabel("REPORTED TO"));
    infoTable->setCellWidget(9,1,new QLabel("Dmitry Samodurov"));
    infoTable->resizeColumnsToContents();

    return infoTable;
}

QWidget* EmployeeView::createStatWidget() {
    QSqlTableModel* model = new QSqlTableModel(0, dbManager->connection);
    model->setTable("ul_stats");
    model->select();

    QTableView* view = new QTableView();
    view->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    view->resize(500, 500);
    view->setModel(model);

    return view;
}

EmployeeView::~EmployeeView()
{
    //delete ui;
}

