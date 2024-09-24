/******************************************************************************
 **  Copyright (c) Raoul Hecky. All Rights Reserved.
 **
 **  Moolticute is free software; you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation; either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Moolticute is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Foobar; if not, write to the Free Software
 **  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 **
 ******************************************************************************/
#ifndef APPGUI_H
#define APPGUI_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QSystemTrayIcon>
#include <QLocalServer>
#include <QLocalSocket>

#include "Common.h"
#include "MainWindow.h"
#include "WSClient.h"
#include "DaemonMenuAction.h"
#include <QtAwesome.h>

class DbMasterController;
class AppGui : public QApplication
{
    Q_OBJECT
public:
    AppGui(int &argc, char **argv);
    virtual ~AppGui();

    bool initialize();

    void mainWindowShow(bool autoLaunched = false);
    void mainWindowHide();
    void enableDaemon();
    void disableDaemon();
    void setupLanguage();

    static QtAwesome *qtAwesome();

    void checkUpdate(bool displayMessage);

    static QString getDataDirPath();

    static QString getFileName(QWidget* parent, const QString &title, const QString &dir, const QString &filter = QString{}, bool acceptSave = false);
    static QString getSaveFileName(QWidget* parent, const QString &title, const QString &dir, const QString &filter = QString{});

private slots:
    void restartDaemon();
    void connectedChanged();
    void updateSystrayTooltip();
    void searchDaemonTick();
    void slotConnectionEstablished();
    void daemonLogRead();
    void updateAvailableReceived(QString version, QString changesetURL);
    void displayStatusWarningNotification();
    void resetLastNotificationStatus();

private:
     MainWindow *win = nullptr;
     QSystemTrayIcon *systray = nullptr;
     WSClient *wsClient = nullptr;
     QAction *showConfigApp = nullptr;
     QAction *getScreenshotOTP = nullptr;
     DaemonMenuAction *daemonAction = nullptr;
#ifndef Q_OS_WIN
     QAction *restartDaemonAction = nullptr;
#endif

     QProcess *daemonProcess = nullptr;
     QProcess *sshAgentProcess = nullptr;
     bool dRunning = false;
     bool needRestart = false;
     bool aboutToQuit = false;
     bool foundDaemon = false;

     quint64 lastStateChange = 0;

     //This is for communication between app/daemon
     QSharedMemory sharedMem;
     QTimer *timerDaemon = nullptr;

     //local server for single instance of the app
     QLocalServer *localServer = nullptr;

     //This socket gives us access to the daemon log
     QLocalSocket *logSocket = nullptr;

     QTranslator *translator = nullptr;

     //Buffer for storing log from daemon when mainwindow is not created
     QByteArray logBuffer;

     Common::MPStatus m_lastNotificationStatus;

     bool createSingleApplication();
     void startSSHAgent();
     void createMainWindow();

     DbMasterController *dbMasterController;
};

#endif // APPGUI_H
