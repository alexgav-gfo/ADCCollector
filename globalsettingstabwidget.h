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

#ifndef ADCCOLLECTOR_GLOBALSETTINGSTABWIDGET_H
#define ADCCOLLECTOR_GLOBALSETTINGSTABWIDGET_H
#include <QWidget>
#include <QPushButton>
#include <QBoxLayout>
#include <QComboBox>
#include <QCheckBox>
#include "directoryselector.h"
#include "settings.h"

/**
 * Вкладка диалога настроек с глобальными настройками программы.
 */
class GlobalSettingsTabWidget : public QWidget {
Q_OBJECT
public:
    explicit  GlobalSettingsTabWidget(QWidget *parent = nullptr);

    GlobalView getSets();

private:
    DirectorySelector *dataRootSelector;
    DirectorySelector *loggingSelector;
    QComboBox *frequencies;
    QComboBox *meaningDataBuffer;
    QCheckBox *dataInOneFileCheckBox;
    QCheckBox *autoStart;
    QVBoxLayout *labels;
    QHBoxLayout *freq;
    QHBoxLayout *mean;
    QLabel *freqStr;
    QLabel *meanStr;
    GlobalView globalSets;
};


#endif //ADCCOLLECTOR_GLOBALSETTINGSTABWIDGET_H
