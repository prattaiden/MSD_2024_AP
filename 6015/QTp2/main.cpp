#include <QApplication>
#include <QPushButton>
#include "msdscriptgui.h"

int main(int argc, char **argv) {
    QApplication app (argc, argv);
    MSDScriptGUI widget;
    widget.show();
    return app.exec();
}
