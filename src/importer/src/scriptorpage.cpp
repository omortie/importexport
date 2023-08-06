#include "scriptorpage.h"
#include "importwizard.h"

ScriptorPage::ScriptorPage(ImportSettings *settings, QSqlTableModel *model, PythonQtObjectPtr *mainContext, QWidget *parent)
    :QWizardPage(parent)
    ,settings(settings)
    ,model(model)
    ,mainContext(mainContext)
{
    // initializing Python engine with the pointer sent by the wizard itself to execute the commands in python console
    initPython();
    // designing the user interface of the scripting wizard page
    setupUi();
}

void ScriptorPage::initializePage()
{
    scriptor->setText(generator->getFullScript());
    lastFilePath = settings->getFilePath();
}

bool ScriptorPage::validatePage()
{
    if (lastFilePath != settings->getFilePath())
    {
        settings->convertFileToTable();
        lastFilePath = settings->getFilePath();
    }
    return true;
}

void ScriptorPage::setupUi()
{
    // Header Label
    setTitle(" : Script");
    setSubTitle("Run the script commands in (Python) to modify and manipulate the parameters and data tables");

    // Layout designs and implement options and widgets to take user inputs
    QGridLayout *mainLayout = new QGridLayout;
    scriptor->setLanguage(Language::Python);
    generator = new ScriptGenerator(settings);
    scriptor->setEnabled(false);
    mainLayout->addWidget(scriptor,0,0);
    QGroupBox *optionsGB = new QGroupBox("Scripting");
    QVBoxLayout *optionsVBox = new QVBoxLayout;
    scriptKey = new QPushButton("Enable");
    optionsVBox->addWidget(scriptKey);
    connect(scriptKey,SIGNAL(clicked(bool)),this,SLOT(scriptTurn()));
    optionsGB->setLayout(optionsVBox);
    optionsVBox->addStretch();
    mainLayout->addWidget(optionsGB,0,1);
    setLayout(mainLayout);
}

void ScriptorPage::initPython()
{
    // Creating a script debugger component (external library) and implementing it as the main IDE for running scripts and debugging them
    scriptor = new ScriptDebugger(mainContext);
}

void ScriptorPage::scriptTurn(){
    if (!scriptor->isEnabled()){
        QMessageBox *msgBox = new QMessageBox();
        msgBox->setText("WARNING : \nEdit scripts will override ALL of the previous settings that you have "
                        "defined ,\nbe careful what are you doing!!!\n"
                        "If you are not advanced in python scripting \nyou can follow manual \nOR\n"
                        "just disable scripting.");
        msgBox->setInformativeText("Do you really want to enable scripting?");
        msgBox->setWindowTitle("CAREFUL!!!");
        msgBox->setIcon(QMessageBox::Warning);
        msgBox->setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        int ret = msgBox->exec();
        if (ret == QMessageBox::No) return;
        scriptor->setEnabled(true);
        scriptKey->setText("Disable");
    }
    else{
        scriptor->setEnabled(false);
        scriptKey->setText("Enable");
    }
}
