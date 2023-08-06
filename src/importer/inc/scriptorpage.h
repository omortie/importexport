#ifndef SCRIPTORPAGE_H
#define SCRIPTORPAGE_H

#include <QtWidgets>
#include <QtSql>
#include "PythonQt.h"
#include "PythonQtScriptingConsole.h"
#include "scriptdebugger.h"
#include "importer.h"
#include "scriptgenerator.h"

//! in the ScriptorPage the user can run it's own python scripts to manipulate data and set parameters by just running a single script
class ScriptorPage : public QWizardPage
{
    Q_OBJECT

public:
    /*! Scriptor page will show the user a panel to eneter , load , save and run the python scripts
     * and also the ability to choose some options and add extra commands to the script
     * \param settings Import Settings class that saves many parameters essential for import procedure
     * \param importer Importer class so we can use it's internal settings class to induce our own script customizations
     * \param model a QSqlTableModel for the scriptor page to to show the user the initial model so he/she can change it
     * by his/her scripts
     * \param mainContext the main context of python qt module that runs any wrapped or internal commands in python console */
    ScriptorPage(ImportSettings *settings
                 , QSqlTableModel *model
                 , PythonQtObjectPtr *mainContext
                 , QWidget *parent = 0);

private slots:
    //! a slot for managing to enable or disable the script debugger component and show the user proper warning messages
    void scriptTurn();

private:
    /*! Designs the UI of the scriptor page , the table models and scriptor panels */
    void setupUi();
    /*! Initializing PythonQt Console and scripteditor window for user to run his/her python commands in shell */
    void initPython();
    /*! Initializing the page is for those settings that are dependant to previous pages. */
    void initializePage();
    /* Testing just to retrive variables */
    bool validatePage();

    //! pointer to settings class that gathers and stores user input parameters and settings
    ImportSettings *settings;
    //! pointer to the sql model for database connection and accessing database data items
    QSqlTableModel *model;
    //! script debugger component as a basic IDE for running and debugging python scripts wrapped in this application
    ScriptDebugger *scriptor;
    //! a button for enabling and disabling the python debugger IDE
    QPushButton *scriptKey;
    //! pointer to the python engine console for running script commands
    PythonQtObjectPtr *mainContext;
    //! using script generator class for setting a pre-generated default script based on user inputs
    ScriptGenerator *generator;
    QString lastFilePath;
};

#endif // SCRIPTORPAGE_H
