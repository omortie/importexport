#include "hftab.h"

HFTab::HFTab(ExportSettings *settings,QWidget *parent)
    : QWidget(parent)
    , settings(settings)
{
    setupUi();
}

void HFTab::setupUi()
{
    QVBoxLayout *mainLayout=new QVBoxLayout;
    mainLayout->addWidget(new QLabel("Header:"));
    headerEdit=new QPlainTextEdit;
    headerEdit->setPlainText(settings->getHeader());
    mainLayout->addWidget(headerEdit);
    mainLayout->addWidget(new QLabel("Footer:"));
    footerEdit=new QPlainTextEdit;
    footerEdit->setPlainText(settings->getFooter());
    mainLayout->addWidget(footerEdit);
    setLayout(mainLayout);
}
