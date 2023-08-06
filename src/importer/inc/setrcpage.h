#ifndef SETRCPAGE_H
#define SETRCPAGE_H


#include <QtWidgets>
#include <QtSql>
#include "settings.h"

//! in the setRCPage the user will choose which field in the file data is related to which field in database
class SetRCPage: public QWizardPage
{
    Q_OBJECT

public:
    /*! SetRC Page is abbreviation of (Set Row Column Page) and it's duty is to map the data extracted from
     * import file and their fields to the columns of the database. it other word it's mapping each column
     * of the file data to the columns of the database.
     * \param settings settings of the import from Importer to gather user - input setting though the pages
     * \param model QSqlQuery Model to retrive each column captions and show them to the user */
    SetRCPage(ImportSettings *settings, QSqlQueryModel *model, QWidget *parent = 0);

private slots:
    /*! This slot will replace the name of the database fields in the header of the file data table widget
     * \param index integer the new index of combobox that has been called the slot and user interacted with */
    void colSelect();
    /*! This slot will set the values of  header items of fileTable based on the sequence of the names of database fields, usually it's correct*/
    void autofill();
    /*! Clears all mapped source-destination selections to default (empty) comboboxes */
    void clear();
    /*! This procedure will select the cells in the parse file data that are going to be imported based on the model field that
     * user has selected by clicking and skip number of rows */
    void fieldSelected(int,int);

private:
    /*! initialize Page procedure will deploy variables and prepares some widgets in this page based
     * on the parameters and settings from previous pages */
    void initializePage();
    /*! When user presses Next button this procedure will scan the input data by the user and if they were valid inputs
     * stores them in the ImportSettings class and will show the next page.
     * \note that some of the pages need previous pages parameters to be analyzed and perform some tasks to go the next ones
     * it also will be done here.
     * \param results will be true if all things done right accordignly or will return false and the user will stay on the page
     * to correct the mistakes */
    bool validatePage();
    /*! Designs the UI of the page and creates widgets and elements to interact with user */
    void setupUI();
    /*! initFieldsTable will initialize the table that shows the columns of the database to the user and another field to
     * show the respected column number of the file data that has been selected. this way the user knows how file datas
     * are going to be mapped on the database */
    void initFieldsTable();
    /*! initFileData will initialize the table that shows the data that is parsed and extracted from the input source file
     * and also shows the mapped field from database destination model. */
    void initFileData();

    //! pointer to the fileData as a table widget which stores all of the parsed data in a table as separated information cells
    QTableWidget *fileData;
    //! pointer to fields as a table widget which shows the user all of the available fields and selective comboboxes in front of them to map data
    QTableWidget *fields;
    //! this list will map the source fields to the destination fields by index numbers, so each column will be related to it's correspondent field
    QMap<int,int> keys;
    QSpinBox *skipEdit;
    //! Import settings pointer traverse through wizard pages to gather user settings and stores all setting parameters
    ImportSettings *settings;
    //! model pointer to access to the database connection and it's table content (data items)
    QSqlQueryModel *model;

    QGridLayout *mainLayout;
    QGridLayout *rcSelLayout;

    //! this list will store all the address of all of the QComboBoxes related to the field numbers to access to them each time
    QList<QComboBox*> *combos = new QList<QComboBox*>;
    //! this list will store the names of the fields by index number
    QStringList *comboNames = new QStringList;
};
#endif // SETRCPAGE_H
