#include "exportdialog.h"

ExportDialog::ExportDialog(const QAbstractTableModel *model, const int index, QWidget *parent)
    : QDialog(parent)
    , model(model)
    , index(index)
{
    // Removing the hint button from the dialog box
    this->setWindowModality(Qt::ApplicationModal);
    this->setWindowFlags(this->windowFlags() & (~Qt::WindowContextHelpButtonHint));

    // use a settings class for storing parameters
    settings = new ExportSettings;

    // Creating Input Forms
    setupUi();
}

void ExportDialog::setupUi()
{
    //loading all icons resource files
    Q_INIT_RESOURCE(export);

    // Designing ExportDialog Header form
    QHBoxLayout *browseHBox = new QHBoxLayout;
    QLabel *destinationLabel = new QLabel("Destination File:");
    pathEdit = new QLineEdit;
    pathEdit->setText(settings->getFilePath());
    pathEdit->setReadOnly(true);
    QPushButton *browseButton = new QPushButton("Select...");
    connect(browseButton,SIGNAL(clicked(bool)),this,SLOT(browse()));
    browseHBox->addWidget(destinationLabel);
    browseHBox->addWidget(pathEdit);
    browseHBox->addWidget(browseButton);
    QHBoxLayout *afterHBox = new QHBoxLayout;
    openafterChkbox = new QCheckBox("Open file after export");
    openafterChkbox->setChecked(true);
    afterHBox->addWidget(openafterChkbox);

    // Designing Tab Forms
    typeTab = new TypeTab(settings,this);
    selectTab = new SelectTab(this);
    hfTab = new HFTab(settings,this);
    captionTab = new CaptionTab(settings,this);
    asciiTab = new ASCIITab(settings,this);
    tabWidget = new QTabWidget;
    tabWidget->addTab(typeTab, tr("Export Type"));
    tabWidget->addTab(selectTab, tr("Select Fields"));
    tabWidget->addTab(captionTab, tr("Caption Fields"));
    tabWidget->addTab(hfTab, tr("Header Footer"));
    tabWidget->addTab(asciiTab, tr("ASCII Options"));

    // Designing Operator Button Form
    QHBoxLayout *buttonBox = new QHBoxLayout;
    startButton = new QPushButton("Start Export");
    QPushButton *cancelButton = new QPushButton("Cancel");
    buttonBox->addWidget(startButton);
    buttonBox->addWidget(cancelButton);
    startButton->setEnabled(false);
    connect(typeTab->typeGroup,SIGNAL(buttonClicked(int)),this,SLOT(typeChanged()));
    connect(startButton,SIGNAL(clicked(bool)) , this, SLOT(gatherSettings()));
    connect(cancelButton, SIGNAL(clicked(bool)), this, SLOT(reject()));

    // Final Design Stage
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addLayout(browseHBox,0,0);
    mainLayout->addLayout(afterHBox,1,0);
    mainLayout->addWidget(tabWidget);
    mainLayout->addLayout(buttonBox,3,0);
    setLayout(mainLayout);
    setWindowTitle(tr("Export Dialog"));
}

void ExportDialog::gatherSettings()
{
    settings->setOpenAfter(openafterChkbox->isChecked());
    settings->setFilePath(pathEdit->text());
    // Checking the selected file type and will set the settings
    if(typeTab->csvRadio->isChecked())
        settings->setFileType("csv");
    if(typeTab->textRadio->isChecked())
        settings->setFileType("txt");
    settings->setSeparator(asciiTab->sepEdit->text());
    settings->setQuote(asciiTab->quoteEdit->text());
    settings->setSkipRow(typeTab->skipEdit->value());
    settings->setGotoFirstRecord(typeTab->gotofirstChkbox->isChecked());
    settings->setExportOnly(typeTab->exportonlyEdit->value());
    settings->setExportAll(typeTab->exportallRadio->isChecked());
    settings->setExportHeaders(captionTab->headersChkBox->isChecked());
    settings->setCurrentOnly(typeTab->selectcurrentChkbox->isChecked());
    settings->setHeader(hfTab->headerEdit->toPlainText());
    settings->setFooter(hfTab->footerEdit->toPlainText());

    // after gathering all settings from GUI elements will call xport to follow procedure
    xport();
}

void ExportDialog::typeChanged()
{
    startButton->setEnabled(false);
}

//! Opens a save dialog box if the user clicks on Select... button and the user have to choose a path for the file to be saved
void ExportDialog::browse()
{
    /*! open a save-file dialog with the selected file type based on the type option for export radio box, determining the file type by the
     * first four characters of the selected radiobutton! */
    QString filePath = QFileDialog::getSaveFileName(this,"Select a file","","*"+typeTab->typeGroup->checkedButton()->text().left(4));
    pathEdit->setText(filePath);
    /*! Checks if the user really selected a file to save exported data or choosed to cancel and then decides
     * to enable or disable start export button */
    startButton->setEnabled((filePath != ""));
}

bool ExportDialog::checkType()
{
    // Deciding which export type has been selected by the user
    if (settings->getFileType() == "csv")
    {
        QString sep=settings->getSeparator();
        QString quote=settings->getQuote();
        xporter=new csvExport(settings->getFilePath(),sep,quote);
    }
    if (settings->getFileType() == "txt") {QMessageBox::warning(this,"Error","Text File Export is not yet implemented");return false;}
    return true;
}

void ExportDialog::xport()
{
    // calling checkType() will determine the appropriate sub-class for the pointer *xporter based on the export types that user selected
    if(!checkType()) return;


    /* extracting the fields selected to be exported from selectTab lists to extract the correspondance data
     * the sequence of field names on this list is the base for the export operation */
    QStringList xportFields;
    if (selectTab->xportFieldList->selectedItems().isEmpty()) xportFields = selectTab->fields.values();
    else
    {
        for (int i=0;i<selectTab->xportFieldList->count();i++)
        {
            xportFields.append(selectTab->xportFieldList->item(i)->text());
        }
    }

    QStringList dataList;
    int startPoint = 0;
    startPoint += settings->getSkipRow();
    int endPoint = model->rowCount()-startPoint;

    // the program will examine the user-input options to constraint the output data and to set limitations such as startPoint and endPoint
    if (settings->getGotoFirstRecord()) startPoint=settings->getSkipRow();

    if (!settings->getExportAll()) endPoint=settings->getExportOnly();

    // Header Export
    if (!settings->getHeader().isEmpty()) xporter->addRecord(((QStringList)(settings->getHeader())));

    // Writing Headers in the string list
    if (settings->getExportHeaders())
    {
        for (int i = 0; i < xportFields.count(); i++) {
            /* this command is a little bit confusing becuase it will use the QMap fields in the selectTab as the base
             * sequence of the database and with that it will find the location (row index) of the current field on the captionTable TableWidget
             * and then add it's related user specified caption as a data to the records that are going to be exported
            */
            dataList.append(captionTab->captionTable->item(selectTab->fields.key(xportFields.at(i)),1)->text());
        }
        xporter->addRecord(dataList);
        dataList.clear();
    }

    //if the user just want the current record to be extracted
    if (settings->getCurrentOnly())
    {
        for(int j=0;j<xportFields.count();j++)
        {
            int n;
            for(n=0;n<model->columnCount();n++)
            {
                if(xportFields.at(j) == model->headerData(n,Qt::Horizontal).toString()) break;
            }
            dataList.append(model->data(model->index(index,n)).toString());
        }
        xporter->addRecord(dataList);
        dataList.clear();
    }
    // or if the user wants all-records or some of them to be extracted
    else
    {
        for(int i=startPoint;i<startPoint+endPoint;i++)
        {
            for(int j=0;j<xportFields.count();j++)
            {
                /* in this part the procedure will search for the header titles which are selected by the user to export
                 * in the selectTab and will just send the correspondance data to the dataList and send it to the exporter as
                 * a record. I used this because there is no other functions in the QAbstractTable model for searching in the headers
                 * like Record.value() in QSqlQuery */
                int n;
                for(n=0;n<model->columnCount();n++)
                {
                    if(xportFields.at(j) == model->headerData(n,Qt::Horizontal).toString()) break;
                }
                dataList.append(model->data(model->index(i,n)).toString());
            }
            xporter->addRecord(dataList);
            dataList.clear();
        }
    }

    // Footer Export
    if (!settings->getFooter().isEmpty()) xporter->addRecord(((QStringList)(settings->getFooter())));

    // Finally the xport() command will act as exporting all the records in the Exporter class and if it doesn't trigger
    // the exporter class has a buffer limitation so it will act automatically based on the records added
    xporter->xport();
    // Closes the exported - written file
    xporter->closeFile();
    QMessageBox::information(this,"Information","Database has been exported successfully");
    // Opening the exported file after export
    if (settings->getOpenAfter()) QDesktopServices::openUrl(QUrl::fromLocalFile(settings->getFilePath()));
}
