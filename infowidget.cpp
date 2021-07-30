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

#include "infowidget.h"

/**
 * Конструктор виджета.
 * @param parent - указатель на дочерний виджет.
 */
InfoWidget::InfoWidget(QWidget *parent) : QWidget(parent) {
    info = new QStorageInfo();
    date = new QDate;
    time = new QTime;

    QFont font;
    font.setPixelSize(24);

    freeSpaceLabel = new QLabel(tr("Free space: "),this);
    dateTimeStr = new QLabel(tr("Current time: "),this);
    infoFreeSpaceLabel = new QLabel(this);
    dateTimeLabel = new QLabel(this);
    infoFreeSpaceLabel->setFont(font);
    dateTimeLabel->setFont(font);

    timerDate = new QTimer(this);
    connect(timerDate, &QTimer::timeout, this, &InfoWidget::slotTimerDate);
    timerDate->start(1000);

    timerSpace = new QTimer(this);
    connect(timerSpace, &QTimer::timeout, this, &InfoWidget::slotTimerSpace);
    slotTimerSpace();
    timerSpace->start(60000);

    mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(freeSpaceLabel);
    mainLayout->addWidget(infoFreeSpaceLabel);
    mainLayout->addWidget(dateTimeStr);
    mainLayout->addWidget(dateTimeLabel);
}

/**
 * Слот для таймера, который спрашивает текущее время.
 */
void InfoWidget::slotTimerDate() {
    int y = date->currentDate().year();
    int m = date->currentDate().month();
    int d = date->currentDate().day();

    QString currYear = QString::number(y);
    QString currMonth{};
    QString currDay{};
    if(m < 10) {
        currMonth = "0" + QString::number(m);
    } else {
        currMonth = QString::number(m);
    }
    if(d < 10) {
        currDay = "0" + QString::number(d);
    } else {
        currDay = QString::number(d);
    }

    int h = time->currentTime().hour();
    int min = time->currentTime().minute();
    int sec = time->currentTime().second();

    QString currHour{};
    QString currMinute{};
    QString currSecond{};
    if(h < 10) {
        currHour = "0" + QString::number(h);
    } else {
        currHour = QString::number(h);
    }
    if(min < 10) {
        currMinute = "0" + QString::number(min);
    } else {
        currMinute = QString::number(min);
    }
    if(sec < 10) {
        currSecond = "0" + QString::number(sec);
    } else {
        currSecond = QString::number(sec);
    }

    QString currentDataTime = currYear + "-" + currMonth + "-" + currDay + " " + currHour + ":" + currMinute + ":" + currSecond;
    dateTimeLabel->setText(currentDataTime);
}

/**
 * Слот для таймера, который спрашивает количество оставшегося места в каталоге с данными.
 */
void InfoWidget::slotTimerSpace() {
    GlobalView globalView = Settings::instance().loadGlobalSettings();
    QString dataRoot = globalView.dataRoot;
    if(dataRoot.isEmpty()) {
        infoFreeSpaceLabel->setStyleSheet(criticalColor);
        infoFreeSpaceLabel->setText("Unknown");
    } else {
        info->setPath(dataRoot);
        qint64 bytes = info->bytesAvailable();
        double mb = static_cast<double>(bytes) / (1024 * 1024);
        QString mbStr = QString("%1 Mb").arg(mb, 0, 'f', 1);
        infoFreeSpaceLabel->setText(mbStr);

        qint64 allSpace = info->bytesTotal();
        if(bytes > allSpace / 3) {
            infoFreeSpaceLabel->setStyleSheet(okColor);
        } else {
            if(bytes < allSpace / 10) {
                infoFreeSpaceLabel->setStyleSheet(criticalColor);
            } else {
                infoFreeSpaceLabel->setStyleSheet(warningColor);
            }
        }
    }
}
