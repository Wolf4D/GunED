#include "mainwindow.h"

#include <QApplication>
#include <QCommandLineParser>
#include "airmsg.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setApplicationName("GunED");
    QApplication::setApplicationVersion(QString(VER));
    QApplication::setOrganizationName("Madness Studio");

    QCommandLineParser parser;
    parser.setApplicationDescription("GunED by Navy LiK (aka Ivan Klenov)");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("source", QApplication::translate("main", "Source file to open"));
    parser.process(a);

    const QStringList args = parser.positionalArguments();

    MainWindow w;
    if (args.size()>0)
        w.loadFile(args.at(0));
    else
    {
        airMsg("Starting GunED\nPlease wait...", QColor(Qt::green));
        w.makeTabs(false);
    }

    w.show();
    return a.exec();
}
