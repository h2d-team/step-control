#include <QGuiApplication>
#include <QApplication>
#include <QFont>
#include <QQuickStyle>
#include "flaticon/flaticon.h"
#include "engine.h"
#include "views.h"
#include <manager.h>
#include "serial/controller.h"
#include "QFont"

int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);
    QQuickStyle::setStyle("Material");
    QFont font("Cantarell", 11);
    app.setFont(font);
    auto engine = core::Engine::instance();
    QObject::connect(&app, &QGuiApplication::aboutToQuit, engine, &core::Engine::stop);

    engine->init();
    COMPONENT_REG(icon, Flaticon);
    COMPONENT_REG(serial, Controller);
    COMPONENT_REG(console, Manager);

    engine->start();
    return app.exec();
}
