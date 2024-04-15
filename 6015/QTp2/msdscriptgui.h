#ifndef MSDSCRIPTGUI_H
#define MSDSCRIPTGUI_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QRadioButton>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QSpinBox>

class MSDScriptGUI : public QWidget
{
    Q_OBJECT
public:
    explicit MSDScriptGUI(QWidget *parent = nullptr);
    void setGridLayout(QGridLayout *layout);
    void fillSummary();
    void clearWidgets();
    void handleInterp();

    QLabel expression_label;
    QTextEdit expression_edit;
    QRadioButton pp_button;
    QRadioButton interp_button;
    QTextEdit q_text_edit_;

private:
    QPushButton *submit;
    QPushButton *reset;
    QTextEdit *summary;



signals:
};

#endif // MSDSCRIPTGUI_H
