#include "widget.h"

#include <QTableView>
#include <QPushButton>
#include <QLayout>
#include <QLineEdit>
#include <QSqlDatabase>
#include <QApplication>
#include <QFile>
#include <QMessageBox>
#include <QComboBox>
#include <QSqlError>
#include <QSqlTableModel>

#include "exportdialog.h"
#include "importwizard.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    //Create Main Window Form
    createForm();
    //Setup Database Connection and the respected model
    setupModel();
}

void Widget::createForm()
{
    //Display Database Information in a QTableView Widget
    table = new QTableView;

    //Import - Export Command Buttons
    QPushButton *xportButton = new QPushButton("Export");
    connect(xportButton,SIGNAL(clicked(bool)),this,SLOT(xport()));
    QPushButton *importButton = new QPushButton("Import");
    connect(importButton,SIGNAL(clicked(bool)),this,SLOT(import()));
    QPushButton *refreshButton = new QPushButton("Refresh");
    connect(refreshButton,SIGNAL(clicked(bool)),this,SLOT(refresh()));

    //Final Main Widget Design
    auto *mainLayout = new QGridLayout;
    mainLayout->addWidget(table);
    tableEdit = new QComboBox();
    mainLayout->addWidget(tableEdit);
    mainLayout->addWidget(xportButton);
    mainLayout->addWidget(importButton);
    mainLayout->addWidget(refreshButton);
    setLayout(mainLayout);
    this->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Minimum);
}

bool Widget::setupModel()
{
    /* in this part we try to open database so we can extract the data from it to display in the
     * viewer table and to create a Model from it and send it to the export or import components */
    //Using SQLite Driver
    QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE");
    //Setting Database Path Address
    //using local database in the project root directory/examples/databse dir
    QString prefix = QApplication::applicationDirPath();
    db.setDatabaseName(prefix + "/../examples/database/testdb.sqlite");
    QFile dbFile(db.databaseName());
    if(!dbFile.exists()) {QMessageBox::warning(this,"Error Opening",db.databaseName() + " Database doesn't exists");return false;}
    if(!db.open()) {QMessageBox::warning(this,"Error Opening",db.databaseName() + " Database Cannot be Opened!!!\n" + db.lastError().text());return false;}

    // We used QSqlQueryModel for the preview section of the main widget becuase it's simple and read-only
    // in this section we are not wiling to edit anything in the database, we just want to read data
    model = new QSqlTableModel(this);
    // refresh slot will refresh the data informations extracted from the database and sets the general query on model
    tableEdit->addItems(db.tables());
    refresh();
    //Setting the table
    table->setModel(model);
    table->resizeColumnsToContents();
    return true;
}

void Widget::xport()
{
    xporter = new ExportDialog(model,table->currentIndex().row(),this);
    /* we have used Export Component as static library , so we can use it's main window class xportDialog
     * that inherited from QDialog main class so it will control the rest of procedures of export */
    xporter->show();
}

void Widget::import()
{
    importer = new ImportWizard(tableEdit->currentText());
    /* we have used Import Component as static library , so we can use it's main window class importWizard
     * that inherited from QWizard main class so it will control the rest of procedures of import */
    importer->show();
}

void Widget::refresh()
{
    // We generate another query and set all the header names again for refreshing the data displayed in the table
    model->setTable(tableEdit->currentText());
    while(model->canFetchMore())
        model->fetchMore();
    model->select();
}
