#include "typetab.h"

TypeTab::TypeTab(ExportSettings *settings, QWidget *parent)
    : QWidget(parent)
    , settings(settings)
{
    // Creating Input Forms
    setupUi();
}

void TypeTab::setupUi()
{
    QWidget *typesTab = new QWidget;
    QWidget *optionsTab = new QWidget;

    //Creating Export Type Layout
    QVBoxLayout *typeLayout = new QVBoxLayout;
    typeGroup = new QButtonGroup;
    csvRadio = new QRadioButton(".csv - CSV File");
    typeGroup->addButton(csvRadio);
    typeLayout->addWidget(csvRadio);
    textRadio = new QRadioButton(".txt - Text File");
    typeGroup->addButton(textRadio);
    typeLayout->addWidget(textRadio);
    typeLayout->addStretch(1);
    typesTab->setLayout(typeLayout);
    csvRadio->setChecked((settings->getFileType() == "csv"));
    textRadio->setChecked((settings->getFileType() == "txt"));

    // Creating Constraints Group Box
    QVBoxLayout *optionsLayout = new QVBoxLayout;
    QGroupBox *constraintsGB = new QGroupBox("Constraints");
    QVBoxLayout *constraintsLayout = new QVBoxLayout;
    gotofirstChkbox = new QCheckBox("Go to the first record");
    gotofirstChkbox->setChecked(settings->getGotoFirstRecord());
    selectcurrentChkbox = new QCheckBox("Select Current Record only");
    selectcurrentChkbox->setChecked(settings->getCurrentOnly());
    constraintsLayout->addWidget(gotofirstChkbox);
    constraintsLayout->addWidget(selectcurrentChkbox);
    constraintsLayout->addStretch(1);
    constraintsGB->setLayout(constraintsLayout);

    optionsLayout->addWidget(constraintsGB);
    optionsTab->setLayout(optionsLayout);

    // Creating Skipping Records Part
    QGroupBox *recordsGB = new QGroupBox("Select Records:");
    QHBoxLayout *recordsLayout = new QHBoxLayout;
    skipEdit = new QSpinBox;
    skipEdit->setMaximum(maxSpinners);
    skipEdit->setValue(settings->getSkipRow());
    QHBoxLayout *skipHBox = new QHBoxLayout;
    skipHBox->addWidget(new QLabel("Skip "));
    skipHBox->addWidget(skipEdit);
    skipHBox->addWidget(new QLabel("records"));
    QVBoxLayout *exportVBox=new QVBoxLayout;
    exportallRadio = new QRadioButton("Export All Records");
    exportonlyRadio = new QRadioButton("Export only ");
    exportonlyEdit = new QSpinBox;
    exportonlyEdit->setMaximum(maxSpinners);
    exportonlyEdit->setValue(settings->getExportOnly());
    exportallRadio->setChecked(settings->getExportAll());
    exportonlyEdit->setEnabled(!settings->getExportAll());
    connect(exportonlyRadio,SIGNAL(toggled(bool)),exportonlyEdit,SLOT(setEnabled(bool)));
    QHBoxLayout *exportonlyHBox = new QHBoxLayout;
    exportonlyHBox->addWidget(exportonlyRadio);
    exportonlyHBox->addWidget(exportonlyEdit);
    exportonlyHBox->addWidget(new QLabel("records"));
    exportVBox->addWidget(exportallRadio);
    exportVBox->addLayout(exportonlyHBox);
    exportVBox->addStretch(1);
    recordsLayout->addLayout(skipHBox);
    recordsLayout->addLayout(exportVBox);
    recordsGB->setLayout(recordsLayout);
    optionsLayout->addWidget(recordsGB);

    // Final Design Stage
    // Implementing Sub-Tab widget system framework
    QTabWidget *subTab = new QTabWidget;
    subTab->addTab(typesTab, "Type");
    subTab->addTab(optionsTab, "Options");
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(subTab);
    setLayout(mainLayout);
}
