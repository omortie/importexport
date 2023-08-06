#ifndef HFTAB_H
#define HFTAB_H

#include <QtWidgets>
#include "settings.h"

class ExportDialog;

//! HFTab is to show two big plain text boxes  so the user can enter simple texts for the header and for footer of the exported file
class HFTab : public QWidget
{
    Q_OBJECT
friend class ExportDialog;

public:
    /*! HFTab is to show two big plain text boxes  so the user can enter simple texts for the header
    * and for footer of the exported file , if the text boxes remain empty no header and footer
    * would be exported and vise versa
    * \param settings an ExportSettings that travel through the tabs and will gather all necessary settings */
    explicit HFTab(ExportSettings *settings,QWidget *parent = 0);

private:
    /*! Designs UI elements so the user can enter texts as header or footer texts
     * Default header and footer texts will be retrived first from ExportSettings class */
    void setupUi();

    //! Export settings class that is used to gather user input settings and also load default values for ui elements
    ExportSettings *settings;

    // User interface elements for taking user inputs for header and footer texts of exported file
    QPlainTextEdit *headerEdit;
    QPlainTextEdit *footerEdit;
};

#endif // HFTAB_H
