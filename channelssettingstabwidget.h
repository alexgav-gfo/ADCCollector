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

#ifndef ADCCOLLECTOR_CHANNELSSETTINGSTABWIDGET_H
#define ADCCOLLECTOR_CHANNELSSETTINGSTABWIDGET_H
#include <QWidget>
#include <QPushButton>
#include <QBoxLayout>
#include <vector>
#include "ChannelSettingsWidget.h"
#include "settings.h"

/**
 * Вкладка диалога настроек с настройками для каналов.
 */
class ChannelsSettingsTabWidget : public QWidget {
Q_OBJECT
public:
    explicit ChannelsSettingsTabWidget(QWidget *parent = nullptr);

    std::vector<ChannelView>* getSets();

private:
    const int NUM_OF_CHANNELS = Settings::instance().getNumOfChannels();
    QVBoxLayout *channelsSettings;
    std::vector<ChannelSettingsWidget*> *channelsSets;
    std::vector<ChannelView> sets;
};


#endif //ADCCOLLECTOR_CHANNELSSETTINGSTABWIDGET_H
