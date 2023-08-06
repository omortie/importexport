#include "captiontab.h"
#include "exportdialog.h"

CaptionTab::CaptionTab(ExportSettings *settings, QWidget *parent)
    : QWidget(parent)
    , settings(settings)
{
   setupUi();
}

void CaptionTab::setupUi()
{
    headersChkBox=new QCheckBox("Export Headers");
    headersChkBox->setChecked(settings->getExportHeaders());

    const QAbstractTableModel *model = ((ExportDialog*)(parent()))->model;
    captionTable = new QTableWidget(model->columnCount(),2,this);
    QStringList tableHeaders = {"Fields","Captions"};
    captionTable->setHorizontalHeaderLabels(tableHeaders);
    /* This table is used for caption each header in the database for exporting operation
     * The first field shouldn't be editable because it holds the original header names from the
     * database selected, but the second field is editable so we can use it later in the main dialog
     * in the first moment both fields are populated with the same names */
    for (int i=0;i<model->columnCount();i++)
    {
        captionTable->setItem(i,0,new QTableWidgetItem(model->headerData(i,Qt::Horizontal).toString()));
        captionTable->item(i,0)->setFlags(Qt::ItemIsAutoTristate);
        captionTable->setItem(i,1,new QTableWidgetItem(captionTable->item(i,0)->text()));
    }

    //Final Design Stage
    QGridLayout *mainLayout=new QGridLayout;
    mainLayout->addWidget(headersChkBox);
    mainLayout->addWidget(captionTable);
    setLayout(mainLayout);
}
