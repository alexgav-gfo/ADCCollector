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

#ifndef ADCCOLLECTOR_CENTRALWIDGET_H
#define ADCCOLLECTOR_CENTRALWIDGET_H
#include <QWidget>
#include <QTimer>
#include <random>
#include <vector>
#include <QGridLayout>
#include <QBoxLayout>
#include "chartwidget.h"
#include "settings.h"

/**
 * Виджет, в котором находятся все каналы.
 */
class CentralWidget : public QWidget {
Q_OBJECT
public:
    explicit CentralWidget(QWidget *parent = nullptr);
    void clear();
    void reload();
    void changeNumberOfChannels(int indexOfChannel);
    void setDataForChannels(std::vector<double> dataForChans);

private:
    std::vector<ChartWidget*> *channels;
    std::vector<double> *dataForChannels;
    QGridLayout *l;

};


#endif //ADCCOLLECTOR_CENTRALWIDGET_H
