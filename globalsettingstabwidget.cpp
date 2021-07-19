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

#include "globalsettingstabwidget.h"

/**
 * Конструктор вкладки с глобальными настройками.
 * @param parent - указатель на дочерний виджет.
 */
GlobalSettingsTabWidget::GlobalSettingsTabWidget(QWidget *parent) : QWidget(parent) {
    globalSets = Settings::instance().loadGlobalSettings();
    dataRootSelector = new DirectorySelector(tr("Data root"), globalSets.dataRoot, this);
    loggingSelector = new DirectorySelector(tr("Logging root"), globalSets.loggingRoot, this);

    freqStr = new QLabel(tr("Frequency: "), this);
    frequencies = new QComboBox(this);
    frequencies->addItem("25");
    frequencies->addItem("50");
    frequencies->addItem("100");
    frequencies->addItem("200");
    frequencies->addItem("400");
    frequencies->addItem("800");
    for(int i = 0; i < frequencies->count(); i++) {
        if(frequencies->itemText(i).toInt() == globalSets.frequency) {
            frequencies->setCurrentIndex(i);
            break;
        }
    }
    freq = new QHBoxLayout;
    freq->addWidget(freqStr);
    freq->addWidget(frequencies);

    meanStr = new QLabel(tr("Meaning data buffer (0 - no mean): "), this);
    meaningDataBuffer = new QComboBox(this);
    meaningDataBuffer->addItem("0");
    meaningDataBuffer->addItem("2");
    meaningDataBuffer->addItem("4");
    meaningDataBuffer->addItem("8");
    meaningDataBuffer->addItem("16");
    meaningDataBuffer->addItem("32");
    for(int i = 0; i < meaningDataBuffer->count(); i++) {
        if(meaningDataBuffer->itemText(i).toInt() == globalSets.meaningDataBuffer) {
            meaningDataBuffer->setCurrentIndex(i);
            break;
        }
    }
    mean = new QHBoxLayout;
    mean->addWidget(meanStr);
    mean->addWidget(meaningDataBuffer);

    dataInOneFileCheckBox = new QCheckBox(tr("Data in one file"), this);
    dataInOneFileCheckBox->setCheckable(true);
    dataInOneFileCheckBox->setChecked(globalSets.dataInOneFile);

    labels = new QVBoxLayout(this);
    labels->addWidget(dataRootSelector);
    labels->addWidget(loggingSelector);
    labels->addLayout(freq);
    labels->addLayout(mean);
    labels->addWidget(dataInOneFileCheckBox);
    labels->addStretch();
}

/**
 * Собирает полученные глобальные настройки.
 * @return - глобальные настройки программы.
 */
GlobalView GlobalSettingsTabWidget::getSets() {
    globalSets.dataRoot = dataRootSelector->getDirectoryPath();
    globalSets.loggingRoot = loggingSelector->getDirectoryPath();
    QString f = frequencies->currentText();
    globalSets.frequency = f.toInt();
    globalSets.dataInOneFile = dataInOneFileCheckBox->isChecked();
    QString m = meaningDataBuffer->currentText();
    globalSets.meaningDataBuffer = m.toInt();
    return globalSets;
}
