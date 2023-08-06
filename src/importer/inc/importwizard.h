/* this component has been written to import the file datas into a given database table
 * the component is using QWizard precedures to get the user input constraints and after finishing the
 * wizard will show a preview panel to verify the changes that has been made to the database table
 * if the user approve to submit the changes will be applied to the database but if user select to rollback
 * all changes to the model will be discarded.
 * the main duty in this component will be done in the Importer class, refer to that class for more information */

//Written by Mortie : contact sherafati.morteza@gmail.com if you had any questions.

#ifndef IMPORTWIZARD_H
#define IMPORTWIZARD_H

#include <QtWidgets>
#include <QtSql>
#ifdef SCRIPT_SUPPORT
    #include "PythonQt.h"
    #include "PythonQt_QtAll.h"
#endif
#include "importer.h"
#include "wizardpages.h"



//! A wizard that manages the process of taking user preferences and options and other important parameters
class ImportWizard : public QWizard
{
    Q_OBJECT

    friend class ImportWizardWrapper;

    enum {
        INTRO_PAGE
        ,SETRC_PAGE
        ,HOW_PAGE
#ifdef SCRIPT_SUPPORT
        ,SCRIPTOR_PAGE
#endif
        ,PREVIEW_PAGE
    };

public:
    /*! A wizard that manages the process of taking user preferences and options and other important parameters
     * then it will populate the user-preferred settings in the ImportSettings class of the Importer class
     * so the Importer will use those settings when the user order the command of import.
     * the 1st constructor will recieve a tableName as QString and a database as QSqlDatabase or it will use the default database and
     * will create a model as QSqlTableModel from these parameters, it's also plays the duty of container for other widgets and wizardpages
     * \param tableName the name of the table in database that we want to import data into that
     * \param db the databse connection */
    explicit ImportWizard(QString tableName,const QSqlDatabase db = QSqlDatabase::database(), QWidget *parent = 0);
    ImportWizard(QAbstractTableModel *_model, QWidget *parent = nullptr);

private slots:
    /*! The submitter slot will commit all changes that has been made to the model to the database and will show the result in a messagebox
     *  submiter Slot will be run when the wizard pages had been completed and will submit the changes done on the model
     *  to the database by calling submitAll() procedure. also manages to write the log file and show it. */
    void submiter();

private:
#ifdef SCRIPT_SUPPORT
    /*! Initializing PythonQt module in the import wizard component and create a mainContext pointer to the PythonQt
     * singleton engine and send it to the ScriptorPage to take control of when user is scripting */
    void initPython();
#endif
    /*! Will Design the UI of the wizard and inserts the wizard pages by sequence and sends the settings class of Importer to them to be populated */
    void setupUi();
    /*! initModel will create a QSqlTableModel based on the database and tableName that caller sent so all operations on the database
     * will be performed by the model later on.
     * \param tableName the name of the table exists in the database
     * \param db the database connection sent by the caller with the specified table */
    void initModel(QString tableName,const QSqlDatabase db);
    /*! nextId function will return the next page id number when user clicks on next button it is an overridden function of QWizard
     * \return the next page number as an integer calculated based on the current page */
    int nextId() const override;

    //! the pointer to the importer class and all of it's subclasses
    Importer *importer;
    //! the pointer to the sql model to access to the data of database tabel , to be compared to imported data
    QSqlTableModel *model;
    //! introduction page pointer to be added to the wizard, will take the file format and basic parameters to parse file
    IntroPage *introPage;
    //! set row column SetRC (stupid first name that got to my mind) for mapping column data imported from source to destionation
    SetRCPage *setRCPage;
    //! how page (what was I thinking when I was naming them? :D) determines the mode to import source data into database and also other commit options
    HowPage *howPage;
#ifdef SCRIPT_SUPPORT
    //! this page will be added to the last page of wizard that enables scripting in python to manipulate all settings , gives access from c++ to python scripts
    ScriptorPage *scriptorPage;
    //! main context of python engine pointer that will do the job of passing python scripts to it's true parent (python API) and returns result
    PythonQtObjectPtr *mainContext;
#endif
    //! the last page of this wizard to show the user what they have done in all other pages and takes an accept or apology :))
    PreviewPage *previewPage;
};



#endif // IMPORTWIZARD_H
