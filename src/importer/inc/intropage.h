#ifndef INTROPAGE_H
#define INTROPAGE_H

#include <QtWidgets>
#include "settings.h"
#include "csvreader.h"

//! IntroPage is one of the pages of the ImportWizard to show the user the options and file types available for import
class IntroPage : public QWizardPage
{
    Q_OBJECT

public:
    /*! IntroPage is one of the pages of the ImportWizard to show the user the options and file types available for import
     * and also takes the special necessary parameters for each file type that user selects. it has another part for user to choose
     * the path of the file to import and also choose the encoding system to read the file by that
     * \param settings the ImportSettings class that will gather user inputs and saves them through the pages of wizard */
    IntroPage(ImportSettings *isettings,QWidget *parent=0);

private slots:
    /*! this slot shows an open file dialog for user to choose the file path for import and sets the result in the pathEdit QLineEdit element
     * in the UI form */
    void browse();
    /*! This procedure will shows the installed available codecs on the machine of the user to read the import file */
    void loadCodecs();

private:
    //! Initialize intro page values saved as default initial values in import settings class
    void initializePage();
    /*! When user presses Next button this procedure will scan the input data by the user and if they were valid inputs
     * stores them in the ImportSettings class and will show the next page.
     * Note that : some of the pages need previous pages parameters to be analyzed and perform some tasks to go the next ones
     * it also will be done here.
     * \return results will be true if all things done right accordignly or will return false and the user will stay on the page
     * to correct the mistakes */
    bool validatePage();
    /*! Designs the UI of the IntroPage and sets the default values to some of the elements
     * Those values are default values provided by the ImportSettings class */
    void setupUi();

    //! import settings pointer that will traverse through the pages of wizard and gathers and stores user input parameters
    ImportSettings *settings;
    //! a QLineEdit widget that will take the user input : source file path to be imported
    QLineEdit *pathEdit;
    //! a QLineEdit widget which will take the user input : separator string for separating the columns in the source file while parsing it
    QLineEdit *sepEdit;
    //! a QLineEdit widget which will take the user input : quote string for encapsulating words in columns of the source file while parsing it
    QLineEdit *quoteEdit;
    //! a QComboBox widget that will shows the user codecs available to parse the input file
    QComboBox *codecEdit;

    //! Radiobox group , file type options (supported by importer)
    QButtonGroup *radioGroup;
    QRadioButton *csvRadio;
    QRadioButton *txtRadio;

    //! .csv file reader to parse csv file and convert them into a table widget , separated data
    csvReader *reader;
};
#endif // INTROPAGE_H
