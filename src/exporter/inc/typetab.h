#ifndef TYPETAB_H
#define TYPETAB_H

#include <QtWidgets>
#include "settings.h"

class ExportDialog;

//! Type Tab is the first tab in the Tabs widget that offers the user the ability to choose the file type for export
class TypeTab : public QWidget
{
public:
    /*! Type Tab is the first tab in the Tabs widget that offers the user the ability to choose
     * the file type for export and also the ability to select customized number of rows to export
     * \param settings an instance of ExportSettings that travel through the pages and gathers settings for export */
    explicit TypeTab(ExportSettings *settings,QWidget *parent = 0);

    friend class ExportDialog;

private:
    //! Designs the UI elements that user can interact with and input some parameters and select options
    void setupUi();

    //! Export settings class that will gather all of the user input settings and stores them in a unified place to be accessed everytime required
    ExportSettings *settings;

    // GUI elements
    QButtonGroup *typeGroup;
    QRadioButton *csvRadio;
    QRadioButton *textRadio;
    QCheckBox *gotofirstChkbox;
    QCheckBox *selectcurrentChkbox;
    QSpinBox *skipEdit;
    QRadioButton *exportallRadio;
    QRadioButton *exportonlyRadio;
    QSpinBox *exportonlyEdit;
};

#endif // TYPETAB_H
