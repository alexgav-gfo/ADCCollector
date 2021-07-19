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

#include "settingsdialog.h"

/**
 * Конструктор диалога настроек.
 * @param parent - указатель на дочерний виджет.
 */
SettingsDialog::SettingsDialog(QWidget *parent) : QDialog(parent) {
    ok = new QPushButton(tr("Ok"), this);
    connect(ok, &QPushButton::clicked, this, &SettingsDialog::slotOk);
    cancel = new QPushButton(tr("Cancel"), this);
    connect(cancel, &QPushButton::clicked, this, &SettingsDialog::slotCancel);

    setWindowTitle(tr("Settings"));

    elements = new QVBoxLayout(this);
    settingsTab = new QTabWidget(this);

    btns = new QHBoxLayout;
    btns->addStretch();
    btns->addWidget(ok);
    btns->addWidget(cancel);

    elements->addWidget(settingsTab);
    elements->addLayout(btns);

    globalSettings = new GlobalSettingsTabWidget(this);
    channelsSettings = new ChannelsSettingsTabWidget(this);

    settingsTab->addTab(globalSettings, tr("Global settings"));
    settingsTab->addTab(channelsSettings, tr("Channels settings"));
}

/**
 * Слот для кнопки OK. При нажатии кнопки сохраняет полученные
 * настройки, если не было ошибок.
 */
void SettingsDialog::slotOk() {
    if(settingsTab->currentIndex() == 0) {
        GlobalView globalSets = globalSettings->getSets();
        if (globalSets.dataRoot.isEmpty()) {
            QMessageBox::critical(this, tr("Error"), tr("Wrong data root path!"));
        } else if (globalSets.loggingRoot.isEmpty()) {
            QMessageBox::critical(this, tr("Error"), tr("Wrong logging root path!"));
        } else {
            this->accept();
        }
    } else {
        this->accept();
    }
}

/**
 * Слот для кнопки Cancel.
 */
void SettingsDialog::slotCancel() {
    this->reject();
}

/**
 * Сохраняет настройки каналов и глобальные настройки программы.
 */
void SettingsDialog::saveSettings() {
    chSets = channelsSettings->getSets();
    for(int i = 0; i < chSets->size(); i++) {
        ChannelView *chView = new ChannelView;
        chView->name = chSets->at(i).name;
        chView->enabled = chSets->at(i).enabled;
        chView->colorOfGrid = chSets->at(i).colorOfGrid;
        chView->colorOfGraph = chSets->at(i).colorOfGraph;
        chView->colorOfText = chSets->at(i).colorOfText;
        chView->saveBinaryData = chSets->at(i).saveBinaryData;
        chView->saveTextData = chSets->at(i).saveTextData;
        Settings::instance().saveChannelSettings(chView, i);
    }

    GlobalView glView = globalSettings->getSets();
    Settings::instance().saveGlobalSettings(&glView);
}

