#include "menu.h"

Menu::Menu(QString n, QStringList list)
{
    name = n;
    menuItems = list;
}

Menu::~Menu() {

}

// getters
QString Menu::getName() { return name; }
QStringList Menu::getMenuItems() { return menuItems; }
int Menu::getPos() { return position; }
