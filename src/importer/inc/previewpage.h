#ifndef PREVIEWPAGE_H
#define PREVIEWPAGE_H

#include <QtWidgets>
#include <QtSql>
#include "importer.h"

//! in the HowPage the user will choose the mode of the importing operation based on the INSERT,UPDATE or DELETE function from SQL or a combination of them
class PreviewPage : public QWizardPage
{
    Q_OBJECT

public:
    /*! Preview page will take all the settings populated from other pages and sends the command of import
     * to the model. changes the model based on the data of the file (table widget) and the mode of import
     * \param importer Importer class that will perform the import operation , fetches the model and table widget datas
     * based on the mode of import (insert , update or delete)
     * \param model a QSqlTableModel for the preview panel to show the changed model on a tableview for the user
     * to see and approve the changes */
    PreviewPage(Importer *importer, QSqlTableModel *model, QWidget *parent = 0);

private:
    /*! Designs the UI of the preview page to show the user a table view based on the changed model
     * and the ability to approve or decline the changes and submit model to database or rollback */
    void setupUi();
    /*! Initializing the page is for those settings that are dependant to previous pages
     * in this page it will determine what mode should be sent to Importer based on the mode
     * of import that user selected in the previous pages. it also commands the Importer
     * to perform the import operation. */
    void initializePage();

    //! pointer to the sql model as a database connection and for accessing to database data items
    QSqlTableModel *model;
    //! pointer to the importer class for using it's commands like execy, submit and logging methods
    Importer *importer;
};
#endif // PREVIEWPAGE_H
