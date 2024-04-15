#include "mainwidget.h"



mainWidget::mainWidget(QWidget *parent)
    : QWidget{parent}, ref (new QPushButton("update summary")), summary(new QTextEdit), fin(new QPushButton("clear"))
{
    QGridLayout *gridLayout = new QGridLayout;

    setGridLayout(gridLayout);

    setLayout(gridLayout);


    connect(ref, &QPushButton::clicked, this , &mainWidget::fillSummary);
    connect(fin, &QPushButton::clicked, this, &mainWidget::clearWidgets);

}

void mainWidget::fillSummary() {
    QString summaryS = QString("name: %1\nage: %2\ngender: %3")
                          .arg(name_edit.text())
                          .arg(age.value())
                          .arg(male_button_.isChecked() ? "male" : female_button_.isChecked() ? "female" : "other");
    summary->setText(summaryS);
}

void mainWidget::clearWidgets() {
    name_edit.clear();
    age.setValue(0);
    male_button_.setChecked(false);
    female_button_.setChecked(false);
    summary->clear();
}

void mainWidget::setGridLayout(QGridLayout *layout){
    //enter name bit
    QGroupBox *nameBox = new QGroupBox("name: ");
    QHBoxLayout *nameLayout = new QHBoxLayout;
    name_label.setText("name: ");
    name_edit.setPlaceholderText("");
    age_label.setText("age: ");
    age.setRange(0,100);
    nameLayout->addWidget(&name_label);
    nameLayout->addWidget(&name_edit);
    nameLayout->addWidget(&age_label);
    nameLayout->addWidget(&age);
    nameBox->setLayout(nameLayout);
    //radio bit
    QGroupBox *genderBox = new QGroupBox("select gender: ");
    QHBoxLayout *radioLayout = new QHBoxLayout;
    male_button_.setText("male");
    female_button_.setText("female");
    other_button_.setText("other");
    radioLayout->addWidget(&male_button_);
    radioLayout->addWidget(&female_button_);
    radioLayout->addWidget(&other_button_);
    genderBox->setLayout(radioLayout);

    //first row
    layout->addWidget(nameBox, 0, 0);
    //second row
    layout->addWidget(genderBox, 1 , 0);
    // //third row
    layout->addWidget(ref,2, 0);
    layout->addWidget(fin, 3, 0);
    //summ
    layout->addWidget(summary);


}



