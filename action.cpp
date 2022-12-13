#include "action.h"

Action::Action()
{
    timeSec = 0;
    connectionLevel = 0;
    intensity = 0;
    batteryLvl = 0;
    mode = "";
}

Action::~Action(){

}

int Action::getTimeSec(){return timeSec;}
int Action::getConnectionLevel(){return connectionLevel;}
int Action::getIntensity(){return intensity;}
int Action::getBatteryLvl(){return batteryLvl;}
QString Action::getMode(){return mode;}
void Action::setTimeSec(int t){timeSec = t;}
void Action::setConnectionLevel(int c){connectionLevel = c;}
void Action::setIntensity(int i){intensity = i;}
void Action::setBatteryLvl(int b){batteryLvl = b;}
void Action::setMode(QString m){mode = m;}

QString Action::GetActionString(){
    QString res = QString("SESSION:%1min; MODE:%2; Connection Level:%3; Intensity level:%4")
            .arg(timeSec / 60).arg(mode).arg(connectionLevel).arg(intensity);
    return res;
}



