#include "msdscriptgui.h"
#include "parse.hpp"
#include "pointer.h"
#include "Val.h"

MSDScriptGUI::MSDScriptGUI(QWidget *parent)
    : QWidget{parent}, expression_edit(new QTextEdit) ,submit(new QPushButton("submit")), reset(new QPushButton("reset")), summary(new QTextEdit)
{
    QGridLayout *gridLayout = new QGridLayout;

    setGridLayout(gridLayout);

    setLayout(gridLayout);


    connect(submit, &QPushButton::clicked, this , &MSDScriptGUI::fillSummary);
    connect(reset, &QPushButton::clicked, this, &MSDScriptGUI::clearWidgets);
}


void MSDScriptGUI::fillSummary() {

    QString input_str = expression_edit.toPlainText();
    std::string str_to_be_parsed = input_str.toUtf8().constData();
    QString output;


    if(interp_button.isChecked()){
        PTR(Val) s = parse_str(str_to_be_parsed)->interp(Env::empty);
        output = QString::fromStdString(s->to_string());
    }
    if(pp_button.isChecked()){
        std::string final = parse_str(str_to_be_parsed)->to_pp_string();
        output = QString::fromStdString(final);

    }

    //summary update
    summary->setText(output);
}

void MSDScriptGUI::clearWidgets() {
    expression_edit.clear();
    pp_button.setChecked(false);
    interp_button.setChecked(false);
    summary->clear();
}

void MSDScriptGUI::setGridLayout(QGridLayout *layout){
    //enter name bit
    QGroupBox *nameBox = new QGroupBox("expression: ");
    QHBoxLayout *nameLayout = new QHBoxLayout;
    nameLayout->addWidget(&expression_edit);
    nameBox->setLayout(nameLayout);

    //radio bit
    QHBoxLayout *radioLayout = new QHBoxLayout;
    QHBoxLayout *summaryLayout = new QHBoxLayout;
    QGroupBox *radioBox = new QGroupBox("display as: ");
    QGroupBox *sumBox = new QGroupBox("summary: ");

    pp_button.setText("pretty print");
    interp_button.setText("interp");
    radioLayout->addWidget(&pp_button);
    radioLayout->addWidget(&interp_button);
    radioBox->setLayout(radioLayout);

    summaryLayout->addWidget(summary);

    sumBox->setLayout(summaryLayout);

    //first row
    layout->addWidget(nameBox, 0, 0);
    //second row
    layout->addWidget(radioBox, 1 , 0);
    //third row
    layout->addWidget(submit,2, 0);
    //fourth row
    layout->addWidget(sumBox);
    layout->addWidget(reset, 6, 0);

}



