#include <QApplication>
#include <QPushButton>

#include "Toast.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    QPushButton button(QStringLiteral("Show Toast"));
    button.resize(320, 120);

    QObject::connect(&button, &QPushButton::clicked, [&button]() {
        Toast::ShowTip(QStringLiteral("Hello Toast"), QMessageBox::Information, &button);
    });

    button.show();
    Toast::ShowTip(QStringLiteral("Toast ready"), QMessageBox::Information, &button);

    return app.exec();
}
