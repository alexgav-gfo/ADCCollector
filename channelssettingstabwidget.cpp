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

#include "channelssettingstabwidget.h"

/**
 * Конструктор вкладки с настройками каналов.
 * @param parent - указатель на дочерний виджет.
 */
ChannelsSettingsTabWidget::ChannelsSettingsTabWidget(QWidget *parent) : QWidget(parent) {
    channelsSettings = new QVBoxLayout(this);
    channelsSets = new std::vector<ChannelSettingsWidget*>;
    for(int i = 0; i < NUM_OF_CHANNELS; i++) {
        ChannelView chView{};
        sets.push_back(chView);
        sets.at(i) = Settings::instance().loadChannelSettings(i);
    }
    for(int i = 0; i < NUM_OF_CHANNELS; i++) {
        ChannelSettingsWidget *channelSettingsWidget = new ChannelSettingsWidget(this);
        channelsSets->push_back(channelSettingsWidget);
        channelsSets->at(i)->setEnabled(sets.at(i).enabled);
        channelsSets->at(i)->setName(sets.at(i).name);
        channelsSets->at(i)->setColorOfGraph(sets.at(i).colorOfGraph);
        channelsSets->at(i)->setColorOfGrid(sets.at(i).colorOfGrid);
        channelsSets->at(i)->setColorOfText(sets.at(i).colorOfText);
        channelsSets->at(i)->setSaveBinaryDataFlag(sets.at(i).saveBinaryData);
        channelsSets->at(i)->setSaveTextDataFlag(sets.at(i).saveTextData);
        channelsSettings->addWidget(channelSettingsWidget);
    }
}

/**
 * @return - массив настроек всех каналов.
 */
std::vector<ChannelView>* ChannelsSettingsTabWidget::getSets() {
    for(int i = 0; i < channelsSets->size(); i++) {
        sets.at(i).enabled = channelsSets->at(i)->getEnabled();
        sets.at(i).name = channelsSets->at(i)->getName();
        sets.at(i).colorOfText = channelsSets->at(i)->getColorOfText();
        sets.at(i).colorOfGraph = channelsSets->at(i)->getColorOfGraph();
        sets.at(i).colorOfGrid = channelsSets->at(i)->getColorOfGrid();
        sets.at(i).saveBinaryData = channelsSets->at(i)->getSaveBinaryDataFlag();
        sets.at(i).saveTextData = channelsSets->at(i)->getSaveTextDataFlag();
    }

    return &sets;
}
