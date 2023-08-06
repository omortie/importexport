#ifndef HOWPAGE_H
#define HOWPAGE_H

#include <QtWidgets>
#include <QtSql>
#include "settings.h"

//! in the HowPage the user will choose the mode of the importing operation based on the INSERT,UPDATE or DELETE functions from SQL or a combination of them
class HowPage : public QWizardPage
{
    Q_OBJECT

public:
    /*! in How Page user is supposed to select the mode of the import operation and also to choose
     * the key field which when importing and searching thorough database it will be done based on key field
     * \param settings : settings of the import from Importer to gather user - input setting though the pages
     * \param model : QSqlQuery Model to retrive each column captions and show them to the user */
    HowPage(ImportSettings *isettings,QSqlQueryModel *imodel,QWidget *parent = 0);

private:
    // Radio button options for various import types like insert, delete or update or even a combination of them
    QRadioButton *insRadio;
    QRadioButton *insnewRadio;
    QRadioButton *upRadio;
    QRadioButton *upinsRadio;
    QRadioButton *delRadio;
    QRadioButton *delinsRadio;
    QButtonGroup *modeGroup;

    // Graphical user interface widgets used to take user inputs
    QListWidget *keyField;
    QSpinBox *commitafterEdit;
    QSpinBox *importCountEdit;
    QLineEdit *logFilePath;
    QPushButton *logPathBtn;
    QCheckBox *importallChkbox;
    QCheckBox *commitDoneChkBox;
    QCheckBox *logChkBox;
    QCheckBox *rewriteChkBox;
    QCheckBox *showAfterChkBox;
    QTabWidget *mainTab = new QTabWidget;
    QWidget *advancedWidget;
    QWidget *commitWidget;

private slots:
    /*! enabling and disabling the input widgets based on the related checkboxes to prevent
     * user from modifying and entering extra parameters */
    void allOrNot(bool);
    void afterOrDuring(bool);
    void logOrNot(bool);
    void logpathBrowse();

private:
    //! Designs the UI of the page and creates widgets and elements to interact with user
    void setupUi();
    //! procedure that will create user interface for the advanced tab options
    void CreateAdvancedTab(QSqlQueryModel *);
    //! procedure that will create user Commit tab options
    void CreateCommitTab();
    /*! When user presses Next button this procedure will scan the input data by the user and if they were valid inputs
     * stores them in the ImportSettings class and will show the next page.
     * \note that some of the pages need previous pages parameters to be analyzed and perform some tasks to go the next ones
     * it also will be done here.
     * \return results will be true if all things done right accordignly or will return false and the user will stay on the page
     * to correct the mistakes */
    bool validatePage();
    /*! Takes the settings parameters from ImportSettings class and apply them to the GUI elements in the page */
    void initializePage();

    //! pointer to sql model to access to the database connection and it's data
    QSqlQueryModel *model;
    //! pointer of import settings that traverse through wizard pages and gather and store setting parameters
    ImportSettings *settings;
};
#endif // HOWPAGE_H
