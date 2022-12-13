#ifndef MENU_H
#define MENU_H

#include <QString>
#include <QStringList>
#include <QVector>

/* Class Purpose: Menu class serves as an objet that will be displayed and interacted with selecting sessions and modes
 *
 * Data Members:
 * -QString name: a QString representation of the menu title
 * -QStringList menuItems: a list of sub menu's titles a menu has
 * -int position: the position of the menu in a menu list
 *
 * Class Functions:
 * -Getters and Setters
 */

class Menu
{
public:
    explicit Menu(QString, QStringList);
    ~Menu();

    QString getName();
    QStringList getMenuItems();
    int getPos();

private:
    QString name;
    QStringList menuItems;
    int position;
};

#endif // MENU_H
