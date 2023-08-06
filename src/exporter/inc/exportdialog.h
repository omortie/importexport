#ifndef ExportDialog_H
#define ExportDialog_H

#include <QtWidgets>
#include <QtSql>
#include "tabs.h"
#include "exporter.h"
#include "csvexport.h"
#include "settings.h"


//! Export Dialog shows a Dialog Window that contains options and GUI elements to take the user preferences and input parameters and perform the export operation based on those options and settings
class ExportDialog : public QDialog
{
    Q_OBJECT

    friend class ExportDialogWrapper;

public:
    /*! ExportDialog constructor that will recieve a sql model that has a database connection in it, it also has database data items
     * ExportDialog will need to access those data to export them into different types.
     * \param model a Model that holds the connection to the database and also has a specific table values in itself
     * all the values will be retrived by this model from the datbase
     * \param index as an integer for specifing currently selected index row in the table widget before export dialog creates */
    explicit ExportDialog(const QAbstractTableModel *model, const int index = 0, QWidget *parent = 0);
    ~ExportDialog() {}
    //! a Model that holds the connection to the database and also has a specific table values in itself
    //! all the values will be retrived by this model from the datbase
    const QAbstractTableModel *model;

private slots:
    //! Opens a save dialog box if the user clicks on Select... button and the user have to choose a path for the file to be saved
    void browse();
    /*! This Procedure will do the main duty in ExportDialog and it will extract data from Model
     * and will send the extracted data to the Class Exporter record-by-record so it will produce an output file
     * in the selected address Path and the appropriate type
     * Important parameters are:
     * - dataList QStringList as data strings that are sequentially created from a record based on the selected fields
     * - xportFields QStringList as a list of field names that they are gonna be exported in an appropriate sequence extracted from xportFielList from selectTab for simplicity
     * - startPoint , endPoint these two integers are limiting the number of exporting records by making limitation to exporting loops
     * - xporter Exporter this is a pointer to the Exporter class which is gonna be the master class of the appropriate sub-class (for example csvExport)
     * all of the extracted datas will send to this class by QStringList and the final call functin xport() will do the export job in the correspondanse sub-class
    */
    void xport();
    /*! enables or disables the Start Export button if the type of the export file has been changed by the user
     * by clicking another radio button related to the type of the export file */
    void typeChanged();
    /*! this procedure will gather all of the necessary data from tabs and will set their respective
     * parameters in the ExportSettings object (settings) to use them during the exporting operation */
    void gatherSettings();

private:
    /*! Designs the UI elements of the export dialog window:
    * a text box and a browse button to show and choose the path of the export file
    * a checkbox to open the exported file after operation
    * Tabs Widget to manage options in the separated tabs
    * includes: TypeTab , SelectTab , CaptionTab , HFTab , ASCIITab */
    void setupUi();
    /*! checkType will check the user preferred type of file to be exported before export operation starts
     * it will check the type of the export file and calls appropriate sub-class of Exporter to export the datas
     * based on that type of the file
     * \return true if everything went well accordignly to the plan or returns false otherwise */
    bool checkType();

    // User interface elements (Widgets that are nesting tab widget)
    QTabWidget *tabWidget;
    TypeTab *typeTab;
    SelectTab *selectTab;
    ASCIITab *asciiTab;
    CaptionTab *captionTab;
    HFTab *hfTab;
    QPushButton *startButton;
    QCheckBox *openafterChkbox;
    QLineEdit *pathEdit;

    //! Exporter class to do the export command and sort recieved records in a desired type for exporting
    Exporter *xporter;
    //! Export Settings class to store the user preferred settings through tab widgets to use them when exporting operation is on going
    ExportSettings *settings;
    //! index as the currently selected index before the export dialog creates
    int index;
};

//! Wrapping Export Dialog class to PythonQt So advanced users can modify the variables from inside the shell of python console
class ExportDialogWrapper : public QObject
{
    Q_OBJECT

public Q_SLOTS:
    ExportDialog *new_ExportDialog(const QAbstractTableModel *model, const int index, QWidget *parent = 0)
    {return new ExportDialog(model,index,parent);}
    void delete_ExportDialog(ExportDialog *o) {delete o;}
    void showWindow(ExportDialog *o) {o->show();}
    void xport(ExportDialog *o) {o->xport();}
    void setFilePath(ExportDialog *o,QString filePath) {o->settings->setFilePath(filePath);}
    void setSkipRows(ExportDialog *o,int skipRows) {o->settings->setSkipRow(skipRows);}
    void setOpenAfter(ExportDialog *o,bool openAfer) {o->settings->setOpenAfter(openAfer);}
    void setFileType(ExportDialog *o,QString fileType) {o->settings->setFileType(fileType);}
    void setSeparator(ExportDialog *o,QString separator) {o->settings->setSeparator(separator);}
    void setQuote(ExportDialog *o,QString quote) {o->settings->setQuote(quote);}
    void setGotoFirstRecord(ExportDialog *o,bool gotofirst) {o->settings->setGotoFirstRecord(gotofirst);}
    void setExportOnly(ExportDialog *o,int exportOnly) {o->settings->setExportOnly(exportOnly);}
    void setExportAll(ExportDialog *o,bool exportAll) {o->settings->setExportAll(exportAll);}
    void setExportHeaders(ExportDialog *o,bool exportHeaders) {o->settings->setExportHeaders(exportHeaders);}
    void setCurrentOnly(ExportDialog *o,bool currentOnly) {o->settings->setCurrentOnly(currentOnly);}
    void setHeader(ExportDialog *o,QString header) {o->settings->setHeader(header);}
    void setFooter(ExportDialog *o,QString footer) {o->settings->setFooter(footer);}
};

#endif // ExportDialog_H
