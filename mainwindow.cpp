#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <time.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_isStartOk = false;
    m_isSessionStart = false;
    setGeometry(400, 200, 700, 550);
    sessionMenu = new Menu("SESSION", {"20min","45min","1hour"});
    modeMenu = new Menu("MODE", {"MET", "DELTA", "THETA", "ALPHA"});
    masterMenu = sessionMenu;
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(handleTimeout()));

    // Initialize the main menu view
    activeQListWidget = ui->menuScreen;
    activeQListWidget->addItems(masterMenu->getMenuItems());
    activeQListWidget->setCurrentRow(0);
    ui->menuNameLabel->setText(masterMenu->getName());

    //power status is off by default
    powerStatus = false;
    changePowerStatus();
    connect(ui->powerButton, &QPushButton::released, this, &MainWindow::powerChange);

    // device interface button connections
    connect(ui->upButton, &QPushButton::clicked, this, &MainWindow::navigateUpMenu);
    connect(ui->downButton, &QPushButton::clicked, this, &MainWindow::navigateDownMenu);
    connect(ui->intensityUpButton, &QPushButton::clicked, this, &MainWindow::intensityUpClick);
    connect(ui->intensityDownButton, &QPushButton::clicked, this, &MainWindow::intensityDownClick);

    connect(ui->checkButton, &QPushButton::clicked, this, &MainWindow::comfirmSelection);

    // Change connection level from admin
    connect(ui->connectionLevelSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::changeConnectionLevel);

    // Battery level spin box connections
    connect(ui->batteryLevelSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::changeBatteryLevel);

    // Connect to ear comboBox from admin
    connect(ui->ConnectToEarAdminBox,QOverload<int>::of(&QComboBox::currentIndexChanged),this,&MainWindow::applyToEar);

    connect(ui->pushButtonReplay, &QPushButton::clicked, this, &MainWindow::onClickReplay);
    ui->checkBoxRecord->setChecked(true);



    m_batteryTimer = new QTimer(this);
    connect(m_batteryTimer, SIGNAL(timeout()), this, SLOT(updateTimer()));
    //connect to ear is false by default
    con2ear = false;

    // Initialize battery and connection levels
    connectionLevel = 8;
    batteryLvl = 8;
    intensity = 5;

    ui->batteryLevelSpin->setValue(batteryLvl);
    ui->programView->setVisible(false);
    ui->sessionView->setVisible(false);

    //disable the intensity up down button
    ui->intensityUpButton->setEnabled(false);
    ui->intensityDownButton->setEnabled(false);
    ui->connectionLevelSpin->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::changePowerStatus(){
    activeQListWidget->setVisible(powerStatus);
    ui->menuNameLabel->setVisible(powerStatus);
    ui->batteryBar->setVisible(powerStatus);
    ui->batteryLabel->setVisible(powerStatus);

    if(powerStatus){
        ui->connectToEarText->setText("Disconnected");
        ui->connectionLevelSpin->setValue(connectionLevel);
        ui->intensityUpButton->setEnabled(false);
        ui->intensityDownButton->setEnabled(false);
    }

    if (!powerStatus) {
        ui->sessionText->clear();
        ui->modeText->clear();
        masterMenu = sessionMenu;
        updateMenu(masterMenu->getName(), masterMenu->getMenuItems());
        ui->programView->clearFocus();
        ui->programView->setVisible(false);
        ui->sessionView->clearFocus();
        ui->sessionView->setVisible(false);
        ui->intensityText->clear();
        ui->connectionText->clear();
        m_timer->stop();
        ui->intensityUpButton->setEnabled(false);
        ui->intensityDownButton->setEnabled(false);
        ui->ConnectToEarAdminBox->setCurrentIndex(0);
        ui->connectToEarText->clear();
        m_isSessionStart = false;
    }

    ui->upButton->setEnabled(powerStatus);
    ui->downButton->setEnabled(powerStatus);
    ui->checkButton->setEnabled(powerStatus);
    ui->ConnectToEarAdminBox->setEnabled(powerStatus);
}

void MainWindow::powerChange(){
    powerStatus = !powerStatus;
    changePowerStatus();
}

void MainWindow::navigateUpMenu(){
   int nextIndex = activeQListWidget->currentRow() - 1;

   if (nextIndex < 0){
       nextIndex = activeQListWidget->count() -1;
   }

   activeQListWidget->setCurrentRow(nextIndex);
}

void MainWindow::navigateDownMenu(){
    int nextIndex = activeQListWidget->currentRow() + 1;

    if (nextIndex > activeQListWidget->count() - 1){
        nextIndex = 0;
    }

    activeQListWidget->setCurrentRow(nextIndex);
}

void MainWindow::comfirmSelection(){
    int index = activeQListWidget->currentRow();
    if (index < 0) return;

    //selecting session
    if (masterMenu->getName() == "SESSION"){
        //selsct a 20mins session
        if (index == 0){
            ui->sessionText->setText("20 mins");
            curAction.setTimeSec(20* 60);
        }
        //select a 45mins session
        else if(index == 1){
            ui->sessionText->setText("45 mins");
            curAction.setTimeSec(45* 60);
        }
        else if(index == 2){
            ui->sessionText->setText("1 hour");
            curAction.setTimeSec(60* 60);
        }
        //go to modeMenu to selsct modes
        masterMenu = modeMenu;
        updateMenu(masterMenu->getName(), masterMenu->getMenuItems());
    }
    //selscting mode
    else if(masterMenu->getName() == "MODE"){
        //select MET mode
        if(index == 0){
            ui->modeText->setText("MET");
            curAction.setMode("MET");
        }
        //select DELTA mode
        else if(index == 1){
            ui->modeText->setText("DELTA");
            curAction.setMode("DELTA");
        }
        //select THETA mode
        else if(index == 2){
            ui->modeText->setText("THETA");
            curAction.setMode("THETA");
        }
        //selct ALPHA mode
        else if(index == 3){
            ui->modeText->setText("ALPHA");
            //m_curAction.mode = ALPHA;
            curAction.setMode("ALPHA");
        }
        //do a connection test before session start
        connectionTest();
    }

}

void MainWindow::beginTherapy(){
    ui->intensityDownButton->setEnabled(true);
    ui->intensityUpButton->setEnabled(true);
    ui->intensityText->setText("Intensity level: " + QString::number(intensity));

    curAction.setIntensity(intensity);
    m_countDown = 0;
    batteryCountDown = 0;
    m_timer->start(1000);
    m_batteryTimer->start(500);
    ui->pushButtonReplay->setEnabled(false);
    m_isReplay = false;
    m_isSessionStart = true;

}

void MainWindow::connectionTest(){
    ui->programView->setVisible(true);
    QGraphicsScene *scene = new QGraphicsScene(this);
    ui->programView->setScene(scene);
    ui->connectionText->setText("connection Level: " + QString::number(connectionLevel));
    if (connectionLevel >= 7){
        scene->clear();
        scene->addText("No connection");
        ui->sessionView->setVisible(true);
        QGraphicsScene *s = new QGraphicsScene(this);
        ui->sessionView->setScene(s);
        s->addText("Session is not starting due to poor connection");
        m_isStartOk = false;
    }
    else if(connectionLevel >= 4 && connectionLevel <= 6){
        scene->clear();
        scene->addText("Okay connection");
        beginTherapy();
        m_isStartOk = true;
    }
    else if(connectionLevel <= 3){
        scene->clear();
        scene->addText("Excellent connection");
        beginTherapy();
        m_isStartOk = true;
    }
    curAction.setConnectionLevel(connectionLevel);
    ui->upButton->setEnabled(false);
    ui->downButton->setEnabled(false);
    ui->checkButton->setEnabled(false);
    if(m_isStartOk)
    {
        beginTherapy();
        ui->checkButton->setEnabled(false);
    }
}

void MainWindow::updateMenu(const QString menuName, const QStringList menuItems) {

    activeQListWidget->clear();
    activeQListWidget->addItems(menuItems);
    activeQListWidget->setCurrentRow(0);

    ui->menuNameLabel->setText(menuName);
}

void MainWindow::changeConnectionLevel(int conlvl){
    connectionLevel = conlvl;
    ui->connectionLevelSpin->setValue(connectionLevel);

}

void MainWindow::changeBatteryLevel(int blvl){
    batteryLvl = blvl;
    if (batteryLvl >= 1 && batteryLvl <= 8) {
        if (batteryLvl == 1 && powerStatus == true) {
            powerChange();
        }

        ui->batteryLevelSpin->setValue(batteryLvl);
        ui->batteryBar->setValue(batteryLvl);
    }
}

void MainWindow::applyToEar(){
    if(ui->ConnectToEarAdminBox->currentText()=="True"){
        ui->connectToEarText->setText("Connected to Ear");
        srand(time(0));
        connectionLevel = (rand() % 6 + 1);
        ui->connectionLevelSpin->setValue(connectionLevel);
        if(m_isSessionStart)
            m_timer->start(1000);
        ui->connectionLevelSpin->setEnabled(true);
    }
    else if(ui->ConnectToEarAdminBox->currentText()=="False"){
        ui->connectToEarText->setText("Disconnected");
        connectionLevel = 8;
        ui->connectionLevelSpin->setValue(connectionLevel);
        if(m_isSessionStart)
            m_timer->stop();
        ui->connectionLevelSpin->setEnabled(false);
    }
}

void MainWindow::updateTimer()
{
    drainBattery();
}

void MainWindow::drainBattery(){
    if (batteryCountDown >= 900){
        m_batteryTimer->start(500);
        batteryCountDown = 0;
        changeBatteryLevel(batteryLvl-1);
    }

    batteryCountDown++;
}

void MainWindow::updateDisplay(){
    if (con2ear){
        ui->connectToEarText->setText("Connected to Ear");
    }
    else{
        ui->connectToEarText->setText("Disconnect");
    }
    ui->intensityText->setText("Intensity Level: " + QString::number(intensity));
    ui->connectionText->setText("connection Level: " + QString::number(connectionLevel));
}


void MainWindow::intensityDownClick()
{
    if(intensity > 1) intensity--;
    ui->intensityText->setText("Intensity level: " + QString::number(intensity));
    curAction.setIntensity(intensity);
}

void MainWindow::intensityUpClick()
{
    if(intensity < 8) intensity++;
    ui->intensityText->setText("Intensity level: " + QString::number(intensity));
    curAction.setIntensity(intensity);
}

void MainWindow::handleTimeout()
{
    if(m_countDown >= curAction.getTimeSec())
    {
        changePowerStatus();
        m_timer->stop();
        m_batteryTimer->stop();
        ui->pushButtonReplay->setEnabled(true);
        ui->checkButton->setEnabled(true);
        if(ui->checkBoxRecord->isChecked() && !m_isReplay)
        {
            recordAcions.push_back(curAction);
        }
        m_isSessionStart = false;
        if(m_isReplay)
            ui->textBrowser->append("Record:");
        ui->textBrowser->append(curAction.GetActionString());
    }

    ui->sessionView->setVisible(true);
    QGraphicsScene *scene = new QGraphicsScene(this);
    ui->sessionView->setScene(scene);
    scene->addText(convertSecToString(curAction.getTimeSec() - m_countDown));
    m_countDown++;
}

void MainWindow::onClickReplay()
{
    if(recordAcions.empty())
    {
        ui->textBrowser->append("No Record!");
        return;
    }
    curAction = recordAcions.last();
    recordAcions.pop_back();
    m_timer->start(1000);
    m_countDown = 0;
    ui->pushButtonReplay->setEnabled(false);
    ui->checkButton->setEnabled(false);
    m_isReplay = true;
}

QString MainWindow::convertSecToString(int sec)
{
    int m = sec / 60;
    int s = sec % 60;
    return QString("%1::%2").arg(m).arg(s);
}
