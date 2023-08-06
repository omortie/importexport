#ifndef ASCIITAB_H
#define ASCIITAB_H


#include <QtWidgets>
#include "settings.h"

class ExportDialog;

//! ASCII Tab is a special tab for csv exporting operation and in this tab the user can specify the separator and quote parameters
class ASCIITab : public QWidget
{
    Q_OBJECT
friend class ExportDialog;
public:

/*! ASCII Tab is a special tab for csv exporting operation and in this tab the user can specify the
 * separator and quote parameters so the exporting operation will use these parameters to do the export
 * task.
 * \param settings an instance of ExportSettings that will travel through pages and gather settings */
    explicit ASCIITab(ExportSettings *settings,QWidget *parent = 0);

private:
    // User interface elements
    QLineEdit *sepEdit;
    QLineEdit *quoteEdit;

    //! export settings class to store and access settings entered by the user whenever possible
    ExportSettings *settings;

private:
    //! Designs the UI elements that user can interact with and input some parameters and select options
    void setupUi();
};


#endif // ASCIITAB_H
