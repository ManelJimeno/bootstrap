/**
 * Part of https://github.com/ManelJimeno/bootstrap (C) 2022
 * Authors: Manel Jimeno <manel.jimeno@gmail.com>
 * License: http://www.opensource.org/licenses/mit-license.php MIT
 */

#include "application_settings.h"
#include <QtWidgets>
#include <chrono>
#include <thread>

using namespace std::chrono_literals;

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    QMainWindow mainWindow;
    mainWindow.resize(320, 240);

    QPixmap pixmap(":images/splash.png");
    QSplashScreen splash(pixmap, Qt::WindowStaysOnTopHint);
    splash.show();
    splash.showMessage("Loading settings", Qt::AlignHCenter | Qt::AlignBottom);
    ApplicationSettings settings;

    std::thread loadSettings([&mainWindow, &splash, &settings]() {
        std::this_thread::sleep_for(1000ms);
        settings.load();
        QMetaObject::invokeMethod(&splash, "showMessage", Qt::QueuedConnection, Q_ARG(QString, "Loaded settings"),
                                  Q_ARG(int, (Qt::AlignHCenter | Qt::AlignBottom)));
        std::this_thread::sleep_for(1000ms);
        QMetaObject::invokeMethod(&mainWindow, "show", Qt::QueuedConnection);
        QMetaObject::invokeMethod(&splash, "hide", Qt::QueuedConnection);
    });

    auto ret = app.exec();

    loadSettings.join();

    return ret;
}
