#ifndef CAPTIONTAB_H
#define CAPTIONTAB_H

#include <QtWidgets>
#include "settings.h"
class ExportDialog;

//! Caption Tab is the tab which user can modify the headers names of the columns of database when exporting them the customized names would be exported instead of the original names.
class CaptionTab : public QWidget
{
    Q_OBJECT
    friend class ExportDialog;

public:
    /*! Caption Tab is the tab which user can modify the headers names of the
     * columns of database when exporting them the customized names would be
     * exported instead of the original names. as default the original names
     * are meant to be exported
     * \param settings an instance of ExportSettings which travel thorugh pages and
     * gathers settings that are necessary for exporting */
     explicit CaptionTab(ExportSettings *settings,QWidget *parent = 0);
private:
    //! Designs the UI elements that user can interact with and input some parameters and select options
    void setupUi();

    // User interface elements
     QTableWidget *captionTable;
     QCheckBox *headersChkBox;

     //! export settings class to store and access settings entered by the user whenever possible
     ExportSettings *settings;
};

#endif // CAPTIONTAB_H
