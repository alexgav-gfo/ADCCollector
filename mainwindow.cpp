/*Copyright (C) 2021 Daria Gurieva
 This file is part of ADCCollector <https://github.com/dGurieva/ADCCollector>.

 ADCCollector is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 ADCCollector is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with ADCCollector. If not, see <http://www.gnu.org/licenses/>.
 */

#include "MainWindow.h"

/**
 * Конструктор главного окна.
 * @param parent - указатель для дочернего виджета.
 */
MainWindow::MainWindow(QWidget *parent) {
    centralWidget = new CentralWidget(this);
    adcCollector = new ADC(Settings::instance().loadGlobalSettings(), Settings::instance().loadAllChannelSettings());
    connect(adcCollector, &ADC::error, this, &MainWindow::slotADCError);

    initActions();
    initMenu();
    initToolBar();

    this->setCentralWidget(centralWidget);
}

/**
 * Инициализирует события кнопок.
 */
void MainWindow::initActions() {
    startAction = new QAction(QIcon(":/icons/control_play.png"), tr("Start"), this);
    connect(startAction, &QAction::triggered, this, &MainWindow::slotStart);
    stopAction = new QAction(QIcon(":/icons/control_stop.png"), tr("Stop"), this);
    connect(stopAction, &QAction::triggered, this, &MainWindow::slotStop);
    stopAction->setEnabled(false);
    exitAction = new QAction(QIcon(":/icons/cancel.png"), tr("Exit"), this);
    connect(exitAction, &QAction::triggered, this, &MainWindow::slotExit);

    allPanelsAction = new QAction(QIcon(":/icons/chan.png"), tr("All channels"), this);
    connect(allPanelsAction, &QAction::triggered, this, &MainWindow::slotAllPanels);
    firstPanelAction = new QAction(QIcon(":/icons/chan.png"), tr("Channel №1"), this);
    connect(firstPanelAction, &QAction::triggered, this, &MainWindow::slotFirstPanel);
    secondPanelAction = new QAction(QIcon(":/icons/chan.png"), tr("Channel №2"), this);
    connect(secondPanelAction, &QAction::triggered, this, &MainWindow::slotSecondPanel);
    thirdPanelAction = new QAction(QIcon(":/icons/chan.png"), tr("Channel №3"), this);
    connect(thirdPanelAction, &QAction::triggered, this, &MainWindow::slotThirdPanel);
    forthPanelAction = new QAction(QIcon(":/icons/chan.png"), tr("Channel №4"), this);
    connect(forthPanelAction, &QAction::triggered, this, &MainWindow::slotForthPanel);

    preferencesAction = new QAction(QIcon(":/icons/setting_tools.png"), tr("Preferences"), this);
    connect(preferencesAction, &QAction::triggered, this, &MainWindow::slotSettings);

    aboutAction = new QAction(QIcon(":/icons/information.png"), tr("About"), this);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::slotAbout);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::slotUpdateTimer);
}

/**
 * Инициализирует главное меню окна.
 */
void MainWindow::initMenu() {
    menu = new QMenuBar(this);
    QMenu *actionMenu = menu->addMenu(tr("&Action"));
    actionMenu->addAction(startAction);
    actionMenu->addAction(stopAction);
    actionMenu->addSeparator();
    actionMenu->addAction(exitAction);
    QMenu *viewMenu = menu->addMenu(tr("&View"));
    viewMenu->addAction(allPanelsAction);
    viewMenu->addAction(firstPanelAction);
    viewMenu->addAction(secondPanelAction);
    viewMenu->addAction(thirdPanelAction);
    viewMenu->addAction(forthPanelAction);
    QMenu *settingsMenu = menu->addMenu(tr("&Settings"));
    settingsMenu->addAction(preferencesAction);
    QMenu *helpMenu = menu->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAction);
    this->setMenuBar(menu);
}

/**
 * Инициализирует тулбар окна.
 */
void MainWindow::initToolBar() {
    toolBar = new QToolBar(this);
    chartUpdateSpeedStr = new QLabel(tr("Chart update speed (seconds): "), this);
    chartUpdateSpeed = new QComboBox(this);
    infoWidget = new InfoWidget(this);

    connect(chartUpdateSpeed, &QComboBox::currentTextChanged, this, &MainWindow::slotUpdateTimerSpeed);
    chartUpdateSpeed->addItem("0.2");
    chartUpdateSpeed->addItem("0.5");
    chartUpdateSpeed->addItem("1");
    chartUpdateSpeed->addItem("2");
    chartUpdateSpeed->addItem("5");
    chartUpdateSpeed->addItem("10");
    chartUpdateSpeed->addItem("30");
    chartUpdateSpeed->addItem("60");
    chartUpdateSpeed->setCurrentIndex(2);
    slotUpdateTimerSpeed(chartUpdateSpeed->currentText());

    toolBar->setIconSize(QSize(40, 40));
    toolBar->addAction(startAction);
    toolBar->addAction(stopAction);
    toolBar->addAction(preferencesAction);
    toolBar->addAction(aboutAction);
    toolBar->addSeparator();
    toolBar->addWidget(chartUpdateSpeedStr);
    toolBar->addWidget(chartUpdateSpeed);
    toolBar->addSeparator();
    toolBar->addWidget(infoWidget);
    this->addToolBar(toolBar);
}

/**
 * Обрабатывает событие закрытия окна.
 * @param event - событие закрытия окна.
 */
void MainWindow::closeEvent(QCloseEvent *event) {
    QMessageBox::StandardButton mBox = QMessageBox::question(this, tr("Exit"), tr("Do you really want to exit?"), QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel);
    if(mBox == QMessageBox::Ok) {
        adcCollector->stop();
        adcCollector->wait(5000);
        event->accept();
    } else {
        event->ignore();
    }
}

/**
 * Слот для кнопки закрытия программы.
 */
void MainWindow::slotExit() {
    this->close();
}

/**
 * Слот для кнопки "о программе".
 */
void MainWindow::slotAbout() {
    QMessageBox::about(this, tr("Collect system"), tr("Collect system using L-Card LA-I24USB ADC"));
}

/**
 * Слот для кнопки старт.
 */
void MainWindow::slotStart() {
    errMsgExist = false;
    timer->start(chartUpdateSpeed->currentText().toDouble() * 1000);
    preferencesAction->setEnabled(false);
    stopAction->setEnabled(true);
    adcCollector->start();
}

/**
 * Слот для кнопки стоп.
 */
void MainWindow::slotStop() {
    QMessageBox::StandardButton m = QMessageBox::question(this, tr("Stop"), tr("Do you really want to stop collecting data?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if(m == QMessageBox::Yes) {
        stopAll();
    }
}

/**
 * Слот для кнопки настроек.
 */
void MainWindow::slotSettings() {
    SettingsDialog *setDialog = new SettingsDialog(this);
    int res = setDialog->exec();
    if(res == QDialog::Accepted) {
        setDialog->saveSettings();
        infoWidget->slotTimerSpace();
        adcCollector->setSettings(Settings::instance().loadGlobalSettings(), Settings::instance().loadAllChannelSettings());
        centralWidget->reload();
    }
}

/**
 * Слот для кнопки отображения первого канала.
 */
void MainWindow::slotFirstPanel() {
    centralWidget->changeNumberOfChannels(1);
}

/**
 * Слот для кнопки отображения второго канала.
 */
void MainWindow::slotSecondPanel() {
    centralWidget->changeNumberOfChannels(2);
}

/**
 * Слот для кнопки отображения третьего канала.
 */
void MainWindow::slotThirdPanel() {
    centralWidget->changeNumberOfChannels(3);
}

/**
 * Слот для кнопки отображения четвертого канала.
 */
void MainWindow::slotForthPanel() {
    centralWidget->changeNumberOfChannels(4);
}

/**
 * Слот для кнопки отображения всех каналов.
 */
void MainWindow::slotAllPanels() {
    centralWidget->changeNumberOfChannels(0);
}

/**
 * Слот для вызова окна сообщения об ошибках.
 * @param msg - сообщение об ошибке.
 */
void MainWindow::slotADCError(QString msg) {
    stopAll();
    if(!errMsgExist) {
        errMsgExist = true;
        QMessageBox::critical(this, tr("ADC error"), msg, QMessageBox::Ok);
    }
}

/**
 * Слот, останавливающий работу программы.
 */
void MainWindow::stopAll() {
    timer->stop();
    preferencesAction->setEnabled(true);
    centralWidget->clear();
    stopAction->setEnabled(false);
    adcCollector->stop();
}

/**
 * Слот, который изменяет скорость таймера.
 * @param s - скорость таймера.
 */
void MainWindow::slotUpdateTimerSpeed(const QString &s) {
    if(timer->isActive()) {
        double updateFreq = s.toDouble();
        updateFreq *= 1000; // time in msec
        timer->stop();
        timer->start(updateFreq);
    }
}

/**
 * Слот, связанный с таймером.
 * Подгружает данные для каналов с каждым тиком таймера.
 */
void MainWindow::slotUpdateTimer() {
    std::vector<double> chansData = adcCollector->getChannelData();
    centralWidget->setDataForChannels(chansData);
}
