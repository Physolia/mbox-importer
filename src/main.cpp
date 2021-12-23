/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "mboxmainwindow.h"
#include <KAboutData>
#include <QApplication>

#include "mboximporter-version.h"
#include <KCrash>
#include <KLocalizedString>
#include <QCommandLineParser>
#include <QFileDialog>
#include <QIcon>

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps, true);
#endif
    QApplication app(argc, argv);
    app.setDesktopFileName(QStringLiteral("org.kde.mboximporter"));
    KCrash::initialize();
    KLocalizedString::setApplicationDomain("mboximporter");

    KAboutData aboutData(QStringLiteral("mboximporter"),
                         i18n("MBox importer tool"),
                         QStringLiteral(MBOXIMPORTER_VERSION),
                         i18n("MBox Import Tool"),
                         KAboutLicense::GPL_V2,
                         i18n("Copyright © 2013-2021 MBoxImporter authors"));
    aboutData.addAuthor(i18n("Laurent Montel"), i18n("Maintainer"), QStringLiteral("montel@kde.org"));

    QApplication::setWindowIcon(QIcon::fromTheme(QStringLiteral("kmail")));
    aboutData.setOrganizationDomain(QByteArray("kde.org"));
    aboutData.setProductName(QByteArray("mboximporter"));

    KAboutData::setApplicationData(aboutData);

    QString fileName;
    QCommandLineParser parser;
    aboutData.setupCommandLine(&parser);

    parser.addPositionalArgument(QStringLiteral("url"), i18n("URL of mbox to be imported"), QStringLiteral("[url]"));

    parser.process(app);
    aboutData.processCommandLine(&parser);

    const QStringList &args = parser.positionalArguments();
    if (!args.isEmpty()) {
        fileName = args.at(0);
    } else {
        fileName = QFileDialog::getOpenFileName();
    }
    if (fileName.isEmpty()) {
        return 0;
    }
    auto w = new MBoxMainWindow(fileName);
    w->show();
    QObject::connect(w, &MBoxMainWindow::rejected, &app, &QApplication::quit);
    const int ret = app.exec();
    delete w;
    return ret;
}
