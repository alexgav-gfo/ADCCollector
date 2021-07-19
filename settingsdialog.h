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

#ifndef ADCCOLLECTOR_SETTINGSDIALOG_H
#define ADCCOLLECTOR_SETTINGSDIALOG_H
#include <QDialog>
#include <QPushButton>
#include <QDebug>
#include <QBoxLayout>
#include <QTabWidget>
#include <QCloseEvent>
#include <QMessageBox>
#include "globalsettingstabwidget.h"
#include "channelssettingstabwidget.h"
#include "infowidget.h"

/**
 * Диалог настроек программы.
 */
class SettingsDialog : public QDialog {
Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);

    void saveSettings();

private:
    QPushButton *ok;
    QPushButton *cancel;

    QHBoxLayout *btns;
    QVBoxLayout *elements;
    QTabWidget *settingsTab;
    GlobalSettingsTabWidget *globalSettings;
    ChannelsSettingsTabWidget *channelsSettings;
    std::vector<ChannelView> *chSets;
    GlobalView *glSets;


private slots:
    void slotOk();
    void slotCancel();
};


#endif //ADCCOLLECTOR_SETTINGSDIALOG_H
