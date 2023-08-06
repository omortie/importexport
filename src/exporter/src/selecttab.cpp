#include "selecttab.h"
#include "exportdialog.h"

SelectTab::SelectTab(QWidget *parent)
    : QWidget(parent)
{
    // Creating Input Forms
   setupUi();
}

void SelectTab::add()
{
    if(!availableFieldList->selectedItems().isEmpty())
    {
        xportFieldList->addItem(availableFieldList->currentItem()->text());
        availableFieldList->takeItem(availableFieldList->currentRow());
        xportFieldList->setCurrentRow(0);
    }
}

void SelectTab::addall()
{
    if(!availableFieldList->selectedItems().isEmpty())
    {
        xportFieldList->clear();
        xportFieldList->addItems(fields.values());
        availableFieldList->clear();
        xportFieldList->setCurrentRow(0);
    }
}

void SelectTab::del()
{
    if(!xportFieldList->selectedItems().isEmpty())
    {
        availableFieldList->addItem(xportFieldList->currentItem()->text());
        xportFieldList->takeItem(xportFieldList->currentRow());
        availableFieldList->setCurrentRow(0);
    }
}

void SelectTab::delall()
{
    if(!xportFieldList->selectedItems().isEmpty())
    {
        availableFieldList->clear();
        availableFieldList->addItems(fields.values());
        xportFieldList->clear();
        availableFieldList->setCurrentRow(0);
    }
}

void SelectTab::setupUi()
{
    // Designs two list widgets as exporting field and available fields
    availableFieldList = new QListWidget;
    xportFieldList = new QListWidget;
    // Populating the list of fields in the model and also available field list
    for(int i=0;i<((ExportDialog*)(parent()))->model->columnCount();i++)
    {
        fields.insert(i,((ExportDialog*)(parent()))->model->headerData(i,Qt::Horizontal).toString());
        availableFieldList->addItem(fields.value(i));
    }
    availableFieldList->setCurrentRow(0);

    // Implement operator buttons to add or remove fields from available fields to selected fields for export
    QIcon iconAdd;
    iconAdd.addFile(QStringLiteral(":/icons/export/next.png"));
    QIcon iconDel;
    iconDel.addFile(QStringLiteral(":/icons/export/previous.png"));
    QIcon iconAddAll;
    iconAddAll.addFile(QStringLiteral(":/icons/export/last.png"));
    QIcon iconDelAll;
    iconDelAll.addFile(QStringLiteral(":/icons/export/first.png"));
    QVBoxLayout *operatorVBox = new QVBoxLayout;
    QToolButton *addButton = new QToolButton;
    addButton->setIcon(iconAdd);
    connect(addButton,SIGNAL(clicked(bool)),this,SLOT(add()));
    QToolButton *addallButton = new QToolButton;
    addallButton->setIcon(iconAddAll);
    connect(addallButton,SIGNAL(clicked(bool)),this,SLOT(addall()));
    QToolButton *delButton = new QToolButton;
    delButton->setIcon(iconDel);
    connect(delButton,SIGNAL(clicked(bool)),this,SLOT(del()));
    QToolButton *delallButton = new QToolButton;
    delallButton->setIcon(iconDelAll);
    connect(delallButton,SIGNAL(clicked(bool)),this,SLOT(delall()));
    operatorVBox->addWidget(addButton);
    operatorVBox->addWidget(addallButton);
    operatorVBox->addWidget(delButton);
    operatorVBox->addWidget(delallButton);

    // Final design stage
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(new QLabel("Available Fields"),0,0);
    mainLayout->addWidget(availableFieldList,1,0);
    mainLayout->addLayout(operatorVBox,1,1);
    mainLayout->addWidget(new QLabel("Exporting Fields (empty will select all)"),0,2);
    mainLayout->addWidget(xportFieldList,1,2);
    setLayout(mainLayout);
}
