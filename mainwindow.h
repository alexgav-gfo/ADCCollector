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

#ifndef ADCCOLLECTOR_MAINWINDOW_H
#define ADCCOLLECTOR_MAINWINDOW_H
#include "centralwidget.h"
#include "settingsdialog.h"
#include "settings.h"
#include "channelssettingstabwidget.h"
#include "globalsettingstabwidget.h"
#include "adc.h"
#include "infowidget.h"
#include <iostream>
#include <QMainWindow>
#include <QCloseEvent>
#include <QAction>
#include <QMessageBox>
#include <QMenuBar>
#include <QToolBar>
#include <QComboBox>
#include <QTimer>
#include <vector>
#include <random>

/**
 * Окно программы.
 */
class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override = default;


protected:
    void closeEvent(QCloseEvent *event) override;

private:
    ADC *adcCollector;
    CentralWidget *centralWidget;
    InfoWidget *infoWidget;
    QMenuBar *menu;
    QToolBar *toolBar;
    QAction *exitAction;
    QAction *startAction;
    QAction *stopAction;
    QAction *allPanelsAction;
    QAction *firstPanelAction;
    QAction *secondPanelAction;
    QAction *thirdPanelAction;
    QAction *forthPanelAction;
    QAction *preferencesAction;
    QAction *aboutAction;
    QComboBox *chartUpdateSpeed;
    QLabel *chartUpdateSpeedStr;
    QTimer *timer;
    bool errMsgExist;

    void initActions();
    void initMenu();
    void initToolBar();
    void stopAll();

private slots:
    void slotStart();
    void slotStop();
    void slotExit();
    void slotAbout();
    void slotSettings();
    void slotFirstPanel();
    void slotSecondPanel();
    void slotThirdPanel();
    void slotForthPanel();
    void slotAllPanels();
    void slotADCError(QString msg);
    void slotUpdateTimerSpeed(const QString &s);
    void slotUpdateTimer();
};


#endif //ADCCOLLECTOR_MAINWINDOW_H
