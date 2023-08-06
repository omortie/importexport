#include "intropage.h"

IntroPage::IntroPage(ImportSettings *isettings,QWidget *parent)
    : QWizardPage(parent)
    , settings(isettings)
{
    setupUi();
}

void IntroPage::browse()
{
    // The first 4 characters of type radio button is the filter for set the file type in open file dialog
    pathEdit->setText(QFileDialog::getOpenFileName(this,"Select a file","","*"+radioGroup->checkedButton()->text().left(4)));
}

void IntroPage::loadCodecs()
{
    codecEdit->clear();
    /* Loads all the codecs available on the system
     * it will take a little time and may seems hanging for 1 or 2 seconds , so it depends on user's request */
    for (int i = 0 ; i < QTextCodec::availableCodecs().count() ; i++)
    {
        codecEdit->addItem(QTextCodec::availableCodecs().at(i).data());
    }
}

void IntroPage::initializePage()
{
    // Initializing Values injected from ImportSettings class
    pathEdit->setText(settings->getFilePath());
    csvRadio->setChecked((settings->getFileType() == "csv"));
    txtRadio->setChecked((settings->getFileType() == "txt"));
    sepEdit->setText(settings->getSeparator());
    quoteEdit->setText(settings->getQuote());
}

void IntroPage::setupUi()
{
    // Header Label
    setTitle(" : Select File");
    setSubTitle("Select a type for file to be imported and then specify the file path");

    // Creating a group box for show the user available file types for import and take the user's choice
    QGroupBox *typeGB=new QGroupBox("Import from...");
    QVBoxLayout *typeLayout=new QVBoxLayout;
    csvRadio  = new QRadioButton(".csv - CSV File");
    QHBoxLayout *csvParsHBox=new QHBoxLayout;
    sepEdit=new QLineEdit;
    sepEdit->setEnabled(false);
    sepEdit->setText(settings->getSeparator());
    quoteEdit=new QLineEdit;
    quoteEdit->setEnabled(false);
    quoteEdit->setText(settings->getQuote());
    csvParsHBox->addWidget(new QLabel("Separator: "));
    csvParsHBox->addWidget(sepEdit);
    csvParsHBox->addWidget(new QLabel("Quote:"));
    csvParsHBox->addWidget(quoteEdit);
    txtRadio = new QRadioButton(".txt - Text File");
    radioGroup = new QButtonGroup;
    radioGroup->addButton(csvRadio);
    connect(csvRadio,SIGNAL(toggled(bool)),sepEdit,SLOT(setEnabled(bool)));
    connect(csvRadio,SIGNAL(toggled(bool)),quoteEdit,SLOT(setEnabled(bool)));
    radioGroup->addButton(txtRadio);
    typeLayout->addWidget(csvRadio);
    csvRadio->setChecked((settings->getFileType() == "csv"));
    txtRadio->setChecked((settings->getFileType() == "txt"));
    typeLayout->addLayout(csvParsHBox);
    typeLayout->addWidget(txtRadio);
    typeGB->setLayout(typeLayout);

    //Create a form for browsing between files and take the file path address by opening an open file dialog
    QGroupBox *browseGB=new QGroupBox("Browse");
    QHBoxLayout *browseHBox=new QHBoxLayout;
    pathEdit=new QLineEdit;
    pathEdit->setText(settings->getFilePath());
    pathEdit->setReadOnly(true);
    QPushButton *browseButton=new QPushButton("Browse");
    connect(browseButton,SIGNAL(clicked(bool)),this,SLOT(browse()));
    browseHBox->addWidget(new QLabel("Select File"));
    browseHBox->addWidget(pathEdit);
    browseHBox->addWidget(browseButton);
    browseGB->setLayout(browseHBox);

    //Designs codec group box
    QGroupBox *codecGB = new QGroupBox("Codec:");
    QHBoxLayout *codecHBox = new QHBoxLayout;
    codecEdit = new QComboBox;
    /* Just populates the combobox by the first 5 codecs. if user choose to load more codecs
     * by pushing the Load Codecs button it will load all of the codecs available on user system */
    // maximum number of first initialized list of codecs will be determined by host system not just explicit 5
    // this will be for consistency between Qt ICU and Qt Locale migration, will be changed later
    int codecMaxNumber = QTextCodec::availableCodecs().size();
    codecMaxNumber = (codecMaxNumber >= 5) ? 5 : codecMaxNumber;
    for (int i = 0 ; i < codecMaxNumber ; i++)
    {
        codecEdit->addItem(QTextCodec::availableCodecs().at(i).data());
    }
    QPushButton *loadCodecsBtn = new QPushButton("Load Codecs");
    loadCodecsBtn->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Preferred);
    connect(loadCodecsBtn,SIGNAL(clicked(bool)),this,SLOT(loadCodecs()));
    codecHBox->addWidget(new QLabel("Encoding System: "));
    codecHBox->addWidget(codecEdit);
    codecHBox->addWidget(loadCodecsBtn);
    codecGB->setLayout(codecHBox);

    //Final Designing Part
    QGridLayout *mainLayout=new QGridLayout;
    mainLayout->addWidget(typeGB);
    mainLayout->addWidget(browseGB);
    mainLayout->addWidget(codecGB);
    setLayout(mainLayout);
}

bool IntroPage::validatePage()
{
    // Shows a message to the user if the user didn't choose any specific path for the import file because that's essential
    if(pathEdit->text().isEmpty()){
        QMessageBox::critical(this,"Error","File path cannot be empty , please select a file to import");
        return false;
    }
    settings->setFilePath(pathEdit->text());

    // Determines the type of the file to use the appropriate reader to read the data from the file
    switch (radioGroup->checkedId()) {
    case -2:
        settings->setFileType("csv");
        settings->setSeparator(sepEdit->text());
        settings->setQuote(quoteEdit->text());
        settings->setCodec(codecEdit->currentText());
        // using appropriate reader to read the data from csv file and populate a table widget by it's data
        reader = new csvReader(settings->getFilePath(),settings->getSeparator(),settings->getQuote(),settings->getCodec());
        break;
    case -3:
        settings->setFileType("txt");
        settings->setSeparator(sepEdit->text());
        settings->setQuote(quoteEdit->text());
        settings->setCodec(codecEdit->currentText());
        // using appropriate reader to read the data from txt file and populate a table widget by it's data
        reader = new csvReader(settings->getFilePath(),settings->getSeparator(),settings->getQuote(),settings->getCodec());
        break;
    default:
        break;
    }

    /* If the reading process didn't went well, shows an error message that parsing the file is impossible and user should check the
     * validity of the file */
    if (!reader->getSuccess()) {
        QMessageBox::critical(this,"Error","Error while parsing source file, not a valid " + settings->getFileType() + " file!");
        return false;
    }
    settings->setDataTable(reader->getTable());
    settings->setConverted(true); // This flag is set to know if our file has been parsed successfuly or not and do we have a table instead?
    return true;
}
