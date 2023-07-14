#include <QApplication>
#include "MainWindow.h"

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    MainWindow fen;
    fen.show();

    return app.exec();
}
