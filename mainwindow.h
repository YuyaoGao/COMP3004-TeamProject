#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QList>
#include <QGraphicsView>
#include <QTimer>

#include "menu.h"
#include "action.h"

/* Class Purpose: The main window of the application, acts as the control of the application
 *
 * Data Members:
 * -Ui::MainWindow *ui: A ui pointer for the UI in mainwindow
 * -QListWidget *activeQListWidget: A widget for displaying the menus
 *
 * Menu* sessionMenu: the menu for selsction of session times
 * Menu* modeMenu: the menu for selsction of modes
 * Menu* masterMenu: the menu that currently been displayed
 *
 * Action curAction:the action to record the current session
 * QList<Action> recordAcionsthe list of recorded sessions
 *
 * QTimer *m_time: timer for session time;
 * QTimer *m_batteryTimer: timer for calculating battery draining
 *
 * bool powerStatus: whether the oasis-pro is turned on or off
 * bool con2ear: whether the devices is on someones ears or not
 * int connectionLevel: storing the connection level
 * int batteryLvl: storing the battery level
 * int intensity: storing the intensity level
 * bool m_isRecord: boolean value represent if this session is been recorded
 * bool m_isStartOk: boolean value represent if connection test is passed, and ok to start session
 * int batteryCountDown: every 900 seconds countdown will reduce 1 battery level
 * int m_countDown: countdown number for session time countdown
 * bool m_isReplay: boolean value represent if the session is being replayed
 * bool m_isSessionStart: boolean value represent if the session is starting
 *
 * Class Functions:
 * void changePowerStatus(): Disable UI if the device is getting turned off, enable UI if the device is getting turned on
 * void updateMenu(const QString, const QStringList): update the menu items and menu name
 * void beginTherapy():when the connection test if passed, begin therapy, start timer for session and drain timer for battery
 * void drainBattery():function for draining battery, battery will lose 1 level every 900 seconds when doing session, the full battery supports 2 hours using
 * void connectionTest(): check connection level, if connection level is 7 or 8, the session is not starting, and if level is below 7, starts session
 * void updateDisplay(): displays the the connection status
 * QString convertSecToString(int):convert int to string for printing out session time count down
 *
 * void powerChange(): change the power status to on/off
 * void navigateUpMenu(): nevigate up on a menu
 * void navigateDownMenu(): nevigate down on a menu
 * void confirmSelection(): connected to checkButton, confirm the selsction for session time and session mode
 * void changeConnectionLevel(int): set the connection level in admin section
 * void changeBatteryLevel(int): reduce battery level when running the therapy session by time goes and set the battery level in admin section
 * void applyToEar(): start and stop the session and change connection level, when the value been setted to true, a random connection level value will be assigned between 1 to 6
 * void updateTimer(): update the timer display on the screen
 * void intensityDownClick(): button to increase the intensity level when doing session
 * void intensityUpClick(): button to decrease the intensity level when doing session
 * void handleTimeout(): actions when session is over or battery is dead
 * void onClickReplay(): button to replay the same the setting for last previous session
 */


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void changePowerStatus();
    void updateMenu(const QString, const QStringList);
    void beginTherapy();
    void drainBattery();
    void connectionTest();
    void updateDisplay();
    QString convertSecToString(int);

private:
    Ui::MainWindow *ui;
    QListWidget *activeQListWidget;

    Menu* sessionMenu;
    Menu* modeMenu;
    Menu* masterMenu;

    bool powerStatus;
    bool con2ear;
    int connectionLevel;
    int intensity;
    int batteryLvl;
    Action curAction;
    QList<Action> recordAcions;
    bool m_isRecord;
    bool m_isStartOk;
    QTimer *m_timer;
    QTimer *m_batteryTimer;
    int batteryCountDown;
    int m_countDown;
    bool m_isReplay;
    bool m_isSessionStart;

private slots:
    void powerChange();
    void navigateUpMenu();
    void navigateDownMenu();
    void comfirmSelection();
    void changeConnectionLevel(int);
    void changeBatteryLevel(int);
    void applyToEar();
    void updateTimer();
    void intensityDownClick();
    void intensityUpClick();
    void handleTimeout();
    void onClickReplay();
};
#endif // MAINWINDOW_H
