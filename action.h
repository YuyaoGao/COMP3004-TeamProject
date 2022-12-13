#ifndef ACTION_H
#define ACTION_H

#include <QString>

/* Class Purpose: Action class serves as an objet that stores the information for a therapy session
 *
 * Data Members:
 * -QString name: a QString representation of the menu title
 * -QStringList menuItems: a list of sub menu's titles a menu has
 * -int position: the position of the menu in a menu list
 *
 * -int timeSec: the session time selected
 * -int connectionLevel: the connection level selected
 * -int intensity: the connection level selected
 * -int batteryLvl: the battery level left
 * -QString mode: the session mode selected
 *
 * Class Functions:
 * -Getters and Setters
 */

class Action
{
public:
    Action();
    ~Action();

    int getTimeSec();
    int getConnectionLevel();
    int getIntensity();
    int getBatteryLvl();
    QString getMode();
    void setTimeSec(int);
    void setConnectionLevel(int);
    void setIntensity(int);
    void setBatteryLvl(int);
    void setMode(QString);

    QString GetActionString();

private:
    int timeSec;
    int connectionLevel;
    int intensity;
    int batteryLvl;
    QString mode;
};

#endif // ACTION_H
