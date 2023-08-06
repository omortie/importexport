#include "setrcpage.h"

SetRCPage::SetRCPage(ImportSettings *settings,QSqlQueryModel *model, QWidget *parent)
    : QWizardPage(parent)
    , settings(settings)
    , model(model)
{
    initFieldsTable();
    setupUI();
}

void SetRCPage::colSelect()
{
    // It will use two loops over the comboboxes and fieldnames to generate a destField that finally will set on the header items of fileTable
    QString destField = "";
    for (int i = 0 ; i < fileData->columnCount() ; i++)
    {
        destField = "";
        for (int j = 0 ; j < fields->rowCount() ; j++)
        {
            QString currentFieldName = combos->at(j)->currentText();
            currentFieldName = currentFieldName.right(currentFieldName.count() - currentFieldName.indexOf(' '));
            if (currentFieldName.toInt() - 1 == i)
            {
                if (destField != "") destField += ",";
                destField += fields->item(j,0)->text();
            }
        }
        if (destField == "") destField = QString::number(i+1);
        fileData->horizontalHeaderItem(i)->setText(destField);
    }
}

void SetRCPage::autofill()
{
    // loops over all of the comboboxes to set the header names and mapping columns by sequence
    for (int i = 0; i < qMin(fileData->columnCount(),fields->rowCount()) ; i++){
        combos->at(i)->setCurrentIndex(i+1);
    }
}

void SetRCPage::clear()
{
    // loops over all of the comboboxes and will clear the header names and mapping data
    for (int i = 0 ; i < fields->rowCount() ; i++){
        combos->at(i)->setCurrentIndex(0);
    }
}

void SetRCPage::initFieldsTable() {
    // Configure the fields Table to show the header names from databse and a field to show the related column to map datas
    fields = new QTableWidget(model->columnCount(),2);
    fields->setHorizontalHeaderLabels({"Destination Fields","Source Fields"});
    for(int i=0;i<model->columnCount();i++)
        fields->setItem(i,0,new QTableWidgetItem(model->headerData(i,Qt::Horizontal).toString()));
    fields->setSelectionMode(QAbstractItemView::SingleSelection);
    if (!settings->getTechnical()) fields->setEditTriggers(QAbstractItemView::NoEditTriggers);
    fields->setSelectionBehavior(QAbstractItemView::SelectRows);
    connect(fields,SIGNAL(cellClicked(int,int)),this,SLOT(fieldSelected(int,int)));
}


void SetRCPage::fieldSelected(int row,int column) {
    if (column != 0) return;
    if (combos->at(row)->currentText() == "") return;
    int mappedIndex = combos->at(row)->currentText().right(1).toInt() - 1;
    int skipNum = skipEdit->text().toInt();
    fileData->selectColumn(mappedIndex);
    for (int i = 0 ; i < qMin(skipNum,fileData->rowCount()) ; i++) fileData->item(i,mappedIndex)->setSelected(false);
}

void SetRCPage::initFileData() {
    // gets datatable from the parsed file data as a table widget stored in settings
    fileData = settings->getDataTable();
    fileData->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    fileData->setSelectionMode(QAbstractItemView::ContiguousSelection);
    fileData->setSelectionBehavior(QAbstractItemView::SelectColumns);
    if(!settings->getTechnical()) fileData->setEditTriggers(QAbstractItemView::NoEditTriggers);
    fileData->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    // giving a header name to the table widget columns based on it's sequence number
    for (int i = 0 ; i < fileData->columnCount() ; i++)
        fileData->setHorizontalHeaderItem(i,new QTableWidgetItem(QString::number(i+1)));
    fileData->horizontalHeader()->setStyleSheet("QHeaderView::section { background-color: rgb(103,222,255) }");
    rcSelLayout->addWidget(fileData,1,0);
}

void SetRCPage::initializePage()
{
    // Implementing File Data Table Widget that has been populated from parsed input file data
    skipEdit->setValue(settings->getSkipRows());

    //take back the data from csv file as a table widget
    initFileData();

    // Populating comboboxes in the fields table with the number of parsed columns in the file
    if (comboNames->count() - 1 != fileData->columnCount()){
        comboNames->clear();
        combos->clear();
        comboNames->append("");
        // populating names of "Field" + index number of source file column
        for (int i = 1 ; i <= fileData->columnCount() ; i++) comboNames->append("Field " + QString::number(i));
        // creating a list of comboboxes and will connect their signal currentIndexChanged to our colSelect slot to manage the interaction of them
        for (int i = 0 ; i < fields->rowCount() ; i++){
            QComboBox *fieldCombo = new QComboBox;
            combos->append(fieldCombo);
            fieldCombo->addItems(*comboNames);
            connect(fieldCombo,SIGNAL(currentIndexChanged(int)),this,SLOT(colSelect())); // it will work like an update signal from comboboxes changes
            fields->setCellWidget(i , 1 , fieldCombo);
        }
        // Auto-Filling the file data columns with the sequence of fields in model
//        autofill();
    }
    fields->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);
}

void SetRCPage::setupUI()
{
    //Header Label
    setTitle(" : Mapping Data");
    setSubTitle("Select field name from the list box, then click at the column to import this field to");

    //Creating the select layout on the top of the fileData table to show and set parameters for mapping columns
    rcSelLayout=new QGridLayout;

    //Horizontal control bar at top of the fileData table
    QHBoxLayout *fillHBox=new QHBoxLayout;
    QIcon iconAutoFill;
    iconAutoFill.addFile(QStringLiteral(":/icons/import/auto.png"));
    QIcon iconClear;
    iconClear.addFile(QStringLiteral(":/icons/import/clean.png"));
    QToolBar *toolbar=new QToolBar();
    QToolButton *autofillButton=new QToolButton;
    autofillButton->setIcon(iconAutoFill);
    autofillButton->setToolTip("Auto Fill");
    connect(autofillButton,SIGNAL(clicked(bool)),this,SLOT(autofill()));
    toolbar->addWidget(autofillButton);
    QToolButton *clearButton=new QToolButton;
    clearButton->setIcon(iconClear);
    clearButton->setToolTip("Clear");
    connect(clearButton,SIGNAL(clicked(bool)),this,SLOT(clear()));
    toolbar->addWidget(clearButton);
    skipEdit=new QSpinBox;
    skipEdit->setValue(settings->getSkipRows());
    skipEdit->setMaximum(2000);
    fillHBox->addWidget(toolbar);
    fillHBox->addStretch();
    fillHBox->addWidget(new QLabel("Skip Line(s):"));
    fillHBox->addWidget(skipEdit);
    QGroupBox *fillGB = new QGroupBox("Options");
    fillGB->setLayout(fillHBox);
    QGridLayout *mainLayout = new QGridLayout;

    //Final Part of Creating Form
    mainLayout->addWidget(fillGB,0,0);
    mainLayout->addWidget(fields,1,0);
    fields->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    rcSelLayout->addWidget(new QLabel("Source file data:"));
    mainLayout->addLayout(rcSelLayout,2,0);
    setLayout(mainLayout);
}

bool SetRCPage::validatePage()
{
    // Populates the QMap keys by user selection
    for (int i = 0 ; i < fields->rowCount() ; i++)
        keys.insert(i,combos->at(i)->currentText().right(combos->at(i)->currentText().count() - combos->at(i)->currentText().indexOf(' ')).toInt()-1);
    settings->setKeyMap(keys);
    settings->setSkipRows(skipEdit->text().toInt());
    return true;
}
