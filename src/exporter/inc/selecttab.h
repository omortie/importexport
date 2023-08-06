#ifndef SELECTTAB_H
#define SELECTTAB_H

#include <QtWidgets>

class ExportDialog;

//! Select Tab offers the user the ability to select columns for export or leave the page and all columns will be exported
class SelectTab : public QWidget
{
    Q_OBJECT

friend class ExportDialog;

public:

    explicit SelectTab(QWidget *parent = 0);

private slots:
    //! Adds selected column from available fields to exporting fields
    void add();
    //! Adds all of the available columns to the exporting columns
    void addall();
    //! Delete one of the records from exporting frields and adds it to available fields
    void del();
    //! Delete all of the exporting fields and adds them to available fields
    void delall();

private:
    //! Designs the UI elements that user can interact with and input some parameters and select options
    void setupUi();

    //! a list of available field names
    QListWidget *availableFieldList;
    //! a list of fields that are going to be exported
    QListWidget *xportFieldList;
    //! a dictionary that stores the name of available fields in the database and their correspondent index to populate lists
    QMap<int,QString> fields;
};
#endif // SELECTTAB_H
