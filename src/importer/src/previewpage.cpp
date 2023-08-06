#include "previewpage.h"

PreviewPage::PreviewPage(Importer *importer, QSqlTableModel *model, QWidget *parent)
    : QWizardPage(parent)
    , model(model)
    , importer(importer)
{
    setupUi();
}

void PreviewPage::initializePage() {
    // Clearing previous generated logs if the user clicked back button and wanted to start another operation
    importer->logger->clearLog();
    // Again selects all of the data in the database table
    model->select();
    // Determining the mode of import from setting's SqlId
    Importer::Modes cusMode;
    switch(importer->settings->getSqlId())
    {
    case -2:
        cusMode=Importer::InsertAll;
        break;
    case -3:
        cusMode=Importer::InsertNew;
        break;
    case -4:
        cusMode=Importer::Update;
        break;
    case -5:
        cusMode=Importer::UpdateOrInsert;
        break;
    case -6:
        cusMode=Importer::Delete;
        break;
    case -7:
        cusMode=Importer::DeleteOrInsert;
        break;
    }
    //execute the import operation fileData -> model (source -> destination)
    importer->execy(cusMode);
}

void PreviewPage::setupUi() {
    //Header Label
    setTitle(" : Preview Panel");
    setSubTitle("Preview the changes that are going to apply to the database");
    //Create a table to show the last condition of model to user
    QTableView *previewer=new QTableView;
    previewer->setModel(model);
    if(!importer->settings->getTechnical()) previewer->setEditTriggers(QAbstractItemView::NoEditTriggers);
    previewer->resizeColumnsToContents();
    QGridLayout *mainLayout=new QGridLayout;
    mainLayout->addWidget(previewer);
    setLayout(mainLayout);
}
