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

#ifndef ADCCOLLECTOR_INFOWIDGET_H
#define ADCCOLLECTOR_INFOWIDGET_H
#include <QWidget>
#include <QStorageInfo>
#include <QDateTime>
#include <QLabel>
#include <QBoxLayout>
#include <QTimer>
#include "settings.h"

/**
 * Виджет, отображающий текущее время и свободное место в каталоге,
 * в который записываются данные с АЦП.
 */
class InfoWidget : public QWidget {
Q_OBJECT

public:
    explicit InfoWidget(QWidget *parent = nullptr);

public slots:
    void slotTimerSpace();

private:
    const QString criticalColor = "color: rgb(127, 0, 0)";
    const QString warningColor = "color: rgb(230, 120, 0)";
    const QString okColor = "color: rgb(0, 140, 30)";

    QStorageInfo *info;
    QDate *date;
    QTime *time;
    QLabel *freeSpaceLabel;
    QLabel *dateTimeStr;
    QLabel *infoFreeSpaceLabel;
    QLabel *dateTimeLabel;
    QTimer *timerDate;
    QTimer *timerSpace;

    QHBoxLayout *mainLayout;

private slots:
    void slotTimerDate();
};


#endif //ADCCOLLECTOR_INFOWIDGET_H
