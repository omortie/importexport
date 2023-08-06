#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

class QTableView;
class QComboBox;
class QSqlTableModel;
class ExportDialog;
class ImportWizard;

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget(){}

private slots:
    void xport();
    void import();
    void refresh();

private:
    void createForm();
    bool setupModel();

    ExportDialog *xporter;
    ImportWizard *importer;
    QTableView *table;
    QSqlTableModel *model;
    QComboBox *tableEdit;
};

#endif // WIDGET_H
