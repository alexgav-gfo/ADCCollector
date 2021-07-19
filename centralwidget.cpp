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

#include "centralwidget.h"

/**
 * Конструктор виджета с каналами.
 * @param parent - указатель на дочерний виджет.
 */
CentralWidget::CentralWidget(QWidget *parent) : QWidget(parent) {
    channels = new std::vector<ChartWidget*>;
    for(int i = 0; i < Settings::instance().getNumOfChannels(); i++) {
        ChartWidget *channel = new ChartWidget(this);
        channels->push_back(channel);
    }
    reload();
    l = new QGridLayout(this);
    l->addWidget(channels->at(0), 0, 0);
    l->addWidget(channels->at(1), 0, 1);
    l->addWidget(channels->at(2), 1, 0);
    l->addWidget(channels->at(3), 1, 1);
}

/**
 * Изменяет количество отображаемых на экране каналов.
 * @param indexOfChannel - индекс канала, который нужно отобразить (если
 * индекс равен 0, значит нужно отобразить все каналы).
 */
void CentralWidget::changeNumberOfChannels(int indexOfChannel) {
    if(indexOfChannel != 0) {
        for (int i = 0; i < l->count(); i++) {
            l->itemAt(i)->widget()->setVisible(false);
        }
        l->itemAt(indexOfChannel - 1)->widget()->setVisible(true);
    } else {
        for (int i = 0; i < l->count(); i++) {
            l->itemAt(i)->widget()->setVisible(true);
        }
    }
}

/**
 * Подгружает и устанавливает новые настройки для каналов.
 */
void CentralWidget::reload() {
    for(int i = 0; i < Settings::instance().getNumOfChannels(); i++) {
        channels->at(i)->setSettings(Settings::instance().loadChannelSettings(i));
    }
}

/**
 * Очищает массив каналов.
 */
void CentralWidget::clear() {
    for (int i = 0; i < Settings::instance().getNumOfChannels(); i++) {
        if (channels->at(i) != nullptr) {
            channels->at(i)->clear();
        }
    }
}

/**
 * Записывает данные полученные с АЦП во все каналы.
 * @param dataForChans - данные с АЦП.
 */
void CentralWidget::setDataForChannels(std::vector<double> dataForChans) {
    for(int i = 0; i < dataForChans.size(); i++) {
        channels->at(i)->addChartValue(dataForChans[i]);
    }
}
