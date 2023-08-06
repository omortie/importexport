#include "asciitab.h"

ASCIITab::ASCIITab(ExportSettings *settings, QWidget *parent)
    : QWidget(parent)
    , settings(settings)
{
    // Designs the UI elements that user can interact with and input some parameters and select options
    setupUi();
}


void ASCIITab::setupUi()
{
    QGroupBox *csvGB=new QGroupBox("CSV Options");
    QGridLayout *csvLayout=new QGridLayout;
    QLabel *sepLabel=new QLabel("Separator:");
    sepEdit=new QLineEdit;
    sepEdit->setText(settings->getSeparator());
    sepLabel->setBuddy(sepEdit);
    QLabel *quoteLabel=new QLabel("Quote:");
    quoteEdit=new QLineEdit;
    quoteEdit->setText(settings->getQuote());
    quoteLabel->setBuddy(quoteEdit);

    csvLayout->addWidget(sepLabel,0,0);
    csvLayout->addWidget(sepEdit,0,1);
    csvLayout->addWidget(quoteLabel,1,0);
    csvLayout->addWidget(quoteEdit,1,1);
    csvGB->setLayout(csvLayout);

    QGridLayout *mainLayout=new QGridLayout;
    mainLayout->addWidget(csvGB);
    setLayout(mainLayout);
}
