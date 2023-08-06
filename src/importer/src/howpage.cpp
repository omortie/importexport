#include "howpage.h"

HowPage::HowPage(ImportSettings *isettings, QSqlQueryModel *imodel, QWidget *parent)
    :QWizardPage(parent)
    ,model(imodel)
    ,settings(isettings)
{
    setupUi();
}

void HowPage::allOrNot(bool state)
{
    importCountEdit->setEnabled(!state);
}

void HowPage::afterOrDuring(bool state)
{
    commitafterEdit->setEnabled(!state);
}

void HowPage::logOrNot(bool state)
{
    logFilePath->setEnabled(state);
    logPathBtn->setEnabled(state);
    rewriteChkBox->setEnabled(state);
    showAfterChkBox->setEnabled(state);
}

void HowPage::logpathBrowse()
{
    logFilePath->setText(QFileDialog::getSaveFileName(this,"Select a file for logging","","*.log"));
}

void HowPage::setupUi()
{
    //Header Label
    setTitle(" : Import Mode");
    setSubTitle("Choose the mode, how to import data and apply them to model , also specify"
                " additional commit options");

    //Creating two tabs for final options (Advanced Tab : Mode of Import),
    //(Commit Options : additional Import options)
    CreateAdvancedTab(model);
    CreateCommitTab();
    mainTab->addTab(advancedWidget,"Advanced");
    mainTab->addTab(commitWidget,"Commit Options");
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(mainTab);
    setLayout(mainLayout);
}

void HowPage::CreateAdvancedTab(QSqlQueryModel *model)
{
    //Add import options radio button
    advancedWidget = new QWidget;
    QGroupBox *modeGB=new QGroupBox("Import Mode");
    QGridLayout *modeLayout=new QGridLayout;
    insRadio=new QRadioButton("Insert All");
    insnewRadio=new QRadioButton("Insert New");
    upRadio=new QRadioButton("Update");
    upinsRadio=new QRadioButton("Update or Insert");
    delRadio=new QRadioButton("Delete");
    delinsRadio=new QRadioButton("Delete Or Insert");

    //Adding all of the import-type buttons to a ButtonGroup
    modeGroup=new QButtonGroup;
    modeGroup->addButton(insRadio);
    modeGroup->addButton(insnewRadio);
    modeGroup->addButton(upRadio);
    modeGroup->addButton(upinsRadio);
    modeGroup->addButton(delRadio);
    modeGroup->addButton(delinsRadio);
    modeLayout->addWidget(insRadio);
    modeLayout->addWidget(insnewRadio);
    modeLayout->addWidget(upRadio);
    modeLayout->addWidget(upinsRadio);
    modeLayout->addWidget(delRadio);
    modeLayout->addWidget(delinsRadio);
    modeGB->setLayout(modeLayout);

    //Adding the keyField list widget and populate it by the model's colum names
    keyField=new QListWidget;
    keyField->clear();
    for(int i=0;i<model->columnCount();i++)
    {
        keyField->addItem(model->headerData(i,Qt::Horizontal).toString());
    }

    //Final Design stage
    QGridLayout *advancedLayout=new QGridLayout;
    advancedLayout->addWidget(modeGB);
    advancedLayout->addWidget(new QLabel("Select A Key Field:"));
    advancedLayout->addWidget(keyField);
    advancedWidget->setLayout(advancedLayout);
}

void HowPage::CreateCommitTab()
{
    commitWidget = new QWidget;
    QGridLayout *commitLayout = new QGridLayout;
    QHBoxLayout *optionsHBox = new QHBoxLayout;

    // Creating Commit after a specific number of records or after all option groupbox
    QGroupBox *CommitGB = new QGroupBox("Commit");
    QVBoxLayout *commitVBox = new QVBoxLayout;
    QHBoxLayout *commitafterHBox = new QHBoxLayout;
    commitafterHBox->addWidget(new QLabel("Commit after each"));
    commitafterEdit = new QSpinBox;
    commitafterEdit->setMaximum(maxSpinners);
    commitafterEdit->setValue(settings->getCommitNumber());
    commitafterHBox->addWidget(commitafterEdit);
    commitafterHBox->addWidget(new QLabel("record(s)"));
    commitVBox->addLayout(commitafterHBox);
    commitDoneChkBox = new QCheckBox("Commit when done");
    connect(commitDoneChkBox,SIGNAL(toggled(bool)),this,SLOT(afterOrDuring(bool)));
    commitDoneChkBox->setChecked(settings->getCommitAfter());
    commitVBox->addWidget(commitDoneChkBox);
    commitVBox->addStretch();
    CommitGB->setLayout(commitVBox);

    // Creating a form to specify how many records are going to be imported
    QGroupBox *RecordGB = new QGroupBox("Record count");
    QVBoxLayout *howmanyVBox = new QVBoxLayout;
    importallChkbox = new QCheckBox("Import all records");
    howmanyVBox->addWidget(importallChkbox);
    QHBoxLayout *importonlyHBox = new QHBoxLayout;
    importonlyHBox->addWidget(new QLabel("Import only first"));
    importCountEdit = new QSpinBox;
    importCountEdit->setMaximum(maxSpinners);
    importCountEdit->setValue(settings->getImportRowCount());
    connect(importallChkbox,SIGNAL(toggled(bool)),this,SLOT(allOrNot(bool)));
    importallChkbox->setChecked(settings->getImportAll());
    importonlyHBox->addWidget(importCountEdit);
    importonlyHBox->addWidget(new QLabel("record(s)"));
    howmanyVBox->addLayout(importonlyHBox);
    howmanyVBox->addStretch();
    RecordGB->setLayout(howmanyVBox);

    // Creating a logbook option to write log informations on the file
    QGroupBox *logGB = new QGroupBox("Log");
    QGridLayout *logLayout = new QGridLayout;
    logChkBox = new QCheckBox("Enable log");
    QVBoxLayout *logVBox = new QVBoxLayout;
    logVBox->addWidget(new QLabel("Log file name:"));
    QHBoxLayout *logPathHBox = new QHBoxLayout;
    logFilePath = new QLineEdit("logbook.log");
    logPathBtn = new QPushButton("Browse");
    connect(logPathBtn,SIGNAL(clicked(bool)),this,SLOT(logpathBrowse()));
    logPathHBox->addWidget(logFilePath);
    logPathHBox->addWidget(logPathBtn);
    rewriteChkBox = new QCheckBox("Rewrite log file if it exists");
    showAfterChkBox = new QCheckBox("Show log after import");
    connect(logChkBox,SIGNAL(toggled(bool)),this,SLOT(logOrNot(bool)));
    logChkBox->toggled(settings->getLogActive());
    logChkBox->setChecked(settings->getLogActive());
    logFilePath->setText(settings->getLogFilePath());
    rewriteChkBox->setChecked(settings->getRwLog());
    showAfterChkBox->setChecked(settings->getShowLog());
    logLayout->addWidget(logChkBox,0,0);
    logLayout->addLayout(logPathHBox,1,0);
    logLayout->addWidget(rewriteChkBox,2,0);
    logLayout->addWidget(showAfterChkBox,3,0);
    logGB->setLayout(logLayout);
    optionsHBox->addWidget(CommitGB);
    optionsHBox->addWidget(RecordGB);
    commitLayout->addLayout(optionsHBox,0,0);
    commitLayout->addWidget(logGB,1,0);
    commitWidget->setLayout(commitLayout);
}

bool HowPage::validatePage()
{
    // Populating the settings for import from user inputs
    settings->setKeyField(keyField->currentRow());
    int importcount;
    if (!importallChkbox->isChecked() && (importCountEdit->text().toInt() + settings->getSkipRows() < settings->getDataTable()->rowCount()))
        importcount = importCountEdit->text().toInt() + settings->getSkipRows();
    else
        importcount = settings->getDataTable()->rowCount();
    settings->setImportRowCount(importcount);
    // setting the number of rows to be imported and committed to database everytime
    // !!!not implemented any procedure to manage this number
    if (!commitDoneChkBox->isChecked()) settings->setCommitNumber(commitafterEdit->text().toInt());
    else settings->setCommitNumber(commitafterEdit->text().toInt());
    settings->setSqlId(modeGroup->checkedId());
    // setting log options and correspondance numbers
    settings->setLogActive(logChkBox->isChecked());
    settings->setRwLog(rewriteChkBox->isChecked());
    settings->setShowLog(showAfterChkBox->isChecked());
    settings->setLogFilePath(logFilePath->text());
    settings->setOperation(modeGroup->checkedButton()->text());

    QString err;
    if (!settings->getValid(err)){
        QMessageBox::critical(this,"Error",err);
        return false;
    }

    return true;
}

void HowPage::initializePage()
{
    /* Reads the default setting for mode from ImportSettings class and
     * select the appropriate radio button */
    switch (settings->getSqlId()) {
    case -2:
        insRadio->setChecked(true);
        break;
    case -3:
        insnewRadio->setChecked(true);
        break;
    case -4:
        upRadio->setChecked(true);
        break;
    case -5:
        upinsRadio->setChecked(true);
        break;
    case -6:
        delRadio->setChecked(true);
        break;
    case -7:
        delinsRadio->setChecked(true);
        break;
    default:
        break;
    }
    keyField->setCurrentRow(settings->getKeyField());
}
