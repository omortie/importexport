#include "importwizard.h"

ImportWizard::ImportWizard(QString tableName,const QSqlDatabase db, QWidget *parent)
    :QWizard(parent)
{
    initModel(tableName,db);
#ifdef SCRIPT_SUPPORT
    initPython();
#endif
    setupUi();
}

ImportWizard::ImportWizard(QAbstractTableModel *_model, QWidget *parent)
    :QWizard(parent)
{
    model = reinterpret_cast<QSqlTableModel*>(_model);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
//    model->select();
    // Creating an object from the Importer class and send the created model to it
    importer = new Importer(model);
#ifdef SCRIPT_SUPPORT
    initPython();
#endif
    setupUi();
}

void ImportWizard::setupUi()
{
    //loading icons resources
    Q_INIT_RESOURCE(import);

    // Set the wizard page always on top and disables other widgets behind it so the user cannot interact with them
    this->setWindowModality(Qt::ApplicationModal);

    // Disable the help button of the wizard
    this->setWindowFlags(this->windowFlags() & (~Qt::WindowContextHelpButtonHint));

    /* Sets the signal of the pages when completed by the user and pressed finish it will call the submiter slot or close wizard if user rejected
     * and pressed cancel button */
    connect(this,SIGNAL(accepted()),this,SLOT(submiter()));
    connect(this,SIGNAL(rejected()),this,SLOT(close()));

    /* Creating and populating the pages of the wizard
     * also sends the settings of importer to each page so they can set user-input settings from GUI to importer class */
    introPage = new IntroPage(importer->settings,this);
    setRCPage = new SetRCPage(importer->settings,model,this);
    howPage = new HowPage(importer->settings,model,this);
    previewPage = new PreviewPage(importer,model,this);
#ifdef SCRIPT_SUPPORT
    scriptorPage = new ScriptorPage(importer->settings,model,mainContext,this);
#endif
    setPage(INTRO_PAGE,introPage);
    setPage(SETRC_PAGE,setRCPage);
    setPage(HOW_PAGE,howPage);
    setPage(PREVIEW_PAGE,previewPage);
#ifdef SCRIPT_SUPPORT
    setPage(SCRIPTOR_PAGE,scriptorPage);
#endif
    // Setting title for each page in the wizard and show the number of page in the pages
    foreach(int pageId,pageIds()){
        page(pageId)->setTitle("Page " + QString::number(pageId + 1) + " of " +
                               QString::number(pageIds().count()) + page(pageId)->title());
    }
}

void ImportWizard::initModel(QString tableName,const QSqlDatabase db)
{
    // Initializing the table model created from the name of table and database sent from caller
    model = new QSqlTableModel(this,db);
    model->setTable(tableName);

    // This model will use OnManualSubmit Strategy to control the transactions to the database by itself
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();

    // Creating an object from the Importer class and send the created model to it
    importer = new Importer(model);
}

int ImportWizard::nextId() const
{
    // Determines which page should be displayed when user clicks on next button in the current page based on the currentId of the page
    switch(currentId())
    {
    case INTRO_PAGE:
        return SETRC_PAGE;
        break;
    case SETRC_PAGE:
        return HOW_PAGE;
        break;
#ifdef SCRIPT_SUPPORT
    case HOW_PAGE:
        return SCRIPTOR_PAGE;
        break;
    case SCRIPTOR_PAGE:
        return PREVIEW_PAGE;
        break;
#else
    case HOW_PAGE:
        return PREVIEW_PAGE;
        break;
#endif
    case PREVIEW_PAGE:
    default:
        return -1; // will finish the wizard in this point
    }
}

void ImportWizard::submiter()
{
    QString error;
    // using buit-in function of importer class to submit (commit) changes that has been made to the model
    if(importer->submit(error))  // if any error occures , error string will be populated and the submit function will return false
    {
        QMessageBox::information(this,tr("Information"),tr("Submit Succeeded"));
    }
    else
    {
        //showing returned error in the case of failure
        QMessageBox::critical(this,"Error","Last Error[s] : " + model->lastError().text() + "\n" + error);
    }
    //shows the log file if the user selects to show log file after imports
    if (importer->settings->getShowLog()) QDesktopServices::openUrl(QUrl::fromLocalFile(importer->settings->getLogFilePath()));
}

#ifdef SCRIPT_SUPPORT
void ImportWizard::initPython()
{
    PythonQt::init(PythonQt::IgnoreSiteModule | PythonQt::RedirectStdOut); // for wrapping the Qt Core
    PythonQt_QtAll::init(); // for wrapping all of the Qt classes

    /* Wrapping ImportSettings Class to the singleton of PythonQt */
    PythonQt::self()->registerCPPClass("ImportSettings","","Wrapped",PythonQtCreateObject<ImportSettingsWrapper>);
    // Creating the main context pointer to the python engine so we can communicate with the API of python and run our scripts through it
    // Importing wrapped classes in the script
    mainContext = new PythonQtObjectPtr(PythonQt::self()->getMainModule());
    mainContext->evalScript("from PythonQt.Wrapped import *");

    // adding support for wrapped settings class to the context
    mainContext->addObject("settings",importer->settings);
}
#endif
