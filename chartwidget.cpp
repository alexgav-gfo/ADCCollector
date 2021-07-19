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

#include "chartwidget.h"

/**
 * Конструктор канала.
 * @param parent - указатель на дочерний виджет.
 */
ChartWidget::ChartWidget(QWidget *parent) : QWidget(parent) {
    data = new QQueue<double>();
    interval = tr("Automatic");
    enabled = true;
    autoMinMax = true;
    name = tr("Unnamed");
    colorOfGraph = Settings::instance().getColorOfGraph();
    colorOfGrid = Settings::instance().getColorOfGrid();
    colorOfText = Settings::instance().getColorOfText();
    disabledColorOfGrid = QColor(110, 110, 110);
    disabledColorOfText = Qt::white;
    initActions();
    initPopupMenu();
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setColor(QPalette::Background, Qt::black);
    this->setPalette(palette);
}

/**
 * Рисует сетку графика данных.
 * @param gridColor - цвет сетки.
 * @param textColor - цвет значений на сетке.
 */
void ChartWidget::drawGrid(QColor gridColor, QColor textColor) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, false);

    //YAxes:
    double middleCoord = heightOfChartWidget / 2;
    int numberOfHorizontalLines = 0;
    QFont font = painter.font();
    font.setPixelSize(12);
    painter.setFont(font);
    painter.setPen(gridColor);
    for(int i = middleCoord - Y_STEP_IN_PIXELS; i > 0; i -= Y_STEP_IN_PIXELS) {
        painter.drawLine(0, i, widthOfChartWidget, i);
        numberOfHorizontalLines++;
    }
    numberOfHorizontalLines = numberOfHorizontalLines * 2;
    yStepInNumber = (max - min) / numberOfHorizontalLines;
    double numberNearLines = min + ((max - min) / 2) + yStepInNumber;

    painter.setPen(textColor);
    for(int i = middleCoord - Y_STEP_IN_PIXELS; i > 0; i -= Y_STEP_IN_PIXELS) {
        painter.drawText(2, i + 5, QString("%1").arg(numberNearLines, 0, 'f', 2));
        numberNearLines += yStepInNumber;
    }

    painter.setPen(gridColor);
    for(int i = middleCoord + Y_STEP_IN_PIXELS; i < heightOfChartWidget; i += Y_STEP_IN_PIXELS) {
        painter.drawLine(0, i, widthOfChartWidget, i);
    }
    numberNearLines = (min + ((max - min) / 2)) - yStepInNumber;

    painter.setPen(textColor);
    for(int i = middleCoord + Y_STEP_IN_PIXELS; i < heightOfChartWidget; i += Y_STEP_IN_PIXELS) {
        painter.drawText(2, i + 5, QString("%1").arg(numberNearLines, 0, 'f', 2));
        numberNearLines -= yStepInNumber;
    }

    //MiddleLine:
    painter.setPen(QPen(gridColor, 3));
    painter.drawLine(0, middleCoord, widthOfChartWidget, middleCoord);

    painter.setPen(textColor);
    painter.drawText(2, middleCoord + 5, QString("%1").arg(min + ((max - min) / 2), 0, 'f', 2));

    //XAxes:
    painter.setPen(gridColor);
    for(int i = X_STEP_IN_PIXELS; i < widthOfChartWidget; i += X_STEP_IN_PIXELS) {
        painter.drawLine(i, 0, i, heightOfChartWidget);
    }
}

/**
 * Рисует график данных.
 */
void ChartWidget::drawGraph() {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    numberOfDrawingData = this->width() / stepOfGraphInPixels + 1;
    //Graph:
    if(data->size() > 1) {
        painter.setPen(QPen(colorOfGraph, 1.5));
        int j;
        if(data->size() < numberOfDrawingData) {
            j = 1;
        } else {
            j = data->size() - numberOfDrawingData + 1;
        }
        int k = 0;
        for(int i = j; i < data->size(); i++) {
            double distanceAmongMaxAndMinInPixels = ((max - min) / yStepInNumber) * Y_STEP_IN_PIXELS;
            double x1 = stepOfGraphInPixels * k;
            double x2 = stepOfGraphInPixels * (k + 1);
            double y1 = distanceAmongMaxAndMinInPixels + ((heightOfChartWidget - distanceAmongMaxAndMinInPixels) / 2) -
                        (data->at(i - 1) - min) / yStepInNumber * Y_STEP_IN_PIXELS;
            double y2 = distanceAmongMaxAndMinInPixels + ((heightOfChartWidget - distanceAmongMaxAndMinInPixels) / 2) -
                        (data->at(i) - min) / yStepInNumber * Y_STEP_IN_PIXELS;
            painter.drawLine(x1, y1, x2, y2);
            k++;
        }
    }

}

/**
 * Рисует имя канала, текущее значение данных и интервал в котором рисуется график.
 */
void ChartWidget::drawLabels() {
    QPainter painter(this);

    //Name of channel:
    QFont nameFont(painter.font());
    nameFont.setPixelSize(18);
    painter.setFont(nameFont);
    QFontMetrics nameFontFM(nameFont);

    painter.setPen(colorOfText);
    int nameWidthInPixels = nameFontFM.width(name);
    painter.drawText(widthOfChartWidget / 2 - nameWidthInPixels / 2, 20, name);

    //Current data:
    if(!data->empty()) {
        QFont dataFont(painter.font());
        dataFont.setPixelSize(28);
        QFontMetrics dataFontFM(dataFont);
        painter.setFont(dataFont);
        QString dataString = QString("%1").arg(data->at(data->size() - 1), 0, 'f', 2);
        int dataWidthInPixels = dataFontFM.width(dataString);
        painter.drawText(widthOfChartWidget - dataWidthInPixels - 10, 25, dataString);
    }

    if(enabled && channelEnabled && data->size() > 1) {
        if(autoMinMax) {
            interval = tr("Automatic");
        } else {
            interval = QString::number(min) + "..." + QString::number(max) + " V";
        }
        QFont intervalFont(painter.font());
        intervalFont.setPixelSize(20);
        QFontMetrics intervalFontFM(intervalFont);
        painter.setFont(intervalFont);
        int intervalWidthInPixels = intervalFontFM.width(interval);
        painter.setPen(QPen(colorOfText, 20));
        painter.drawText(widthOfChartWidget - intervalWidthInPixels - 10, heightOfChartWidget - 5, interval);
    }
}

/**
 * Отрисовывает весь виджет.
 */
void ChartWidget::paintEvent(QPaintEvent *) {
    if(!this->isVisible()) {
        return;
    }
    QPainter painter(this);
    widthOfChartWidget = this->width();
    heightOfChartWidget = this->height();

    if (data->size() > 1 && enabled && channelEnabled) {
        if(autoMinMax) {
            min = findMin();
            max = findMax();
        } else {
            min = selectedMin;
            max = selectedMax;
        }
    } else {
        min = 0;
        max = 0;
    }

    if(channelEnabled) {
        drawGrid(colorOfGrid, colorOfText);
    } else {
        drawGrid(disabledColorOfGrid, disabledColorOfText);
    }
    if(enabled && channelEnabled) {
        drawGraph();
    }
    if(channelEnabled) {
        drawLabels();
    }
}

/**
 * Устанавливает настройки для канала.
 * @param channelView - настройки канала.
 */
void ChartWidget::setSettings(ChannelView channelView) {
    channelEnabled = channelView.enabled;
    name = channelView.name;
    colorOfText = channelView.colorOfText;
    colorOfGrid = channelView.colorOfGrid;
    colorOfGraph = channelView.colorOfGraph;
    this->update();
}

/**
 * Устанавливает максимальное значение данных для отображения.
 * @param maxValue - максимальное значение данных.
 */
void ChartWidget::setMax(double maxValue) {
    max = maxValue;
    this->update();
}

/**
 * Устанавливает минимальное значение данных для отображения.
 * @param minValue - минимальное значение данных.
 */
void ChartWidget::setMin(double minValue) {
    min = minValue;
    this->update();
}

/**
 * Очищает массив с данными.
 */
void ChartWidget::clear() {
    data->clear();
    this->repaint();
}

/**
 * Добавляет текущее полученное значение в очередь с данными, которые рисуются на экране.
 * @param value - полученное значение.
 */
void ChartWidget::addChartValue(double value) {
    data->enqueue(value);
    if(data->size() > this->width() / stepOfGraphInPixels + 1) {
        data->dequeue();
    }
    this->repaint();
}

/**
 * Поиск минимального значения в очереди данных.
 * @return - минимальное значение в очереди данных.
 */
double ChartWidget::findMin() {
    double minVal = data->at(0);
    for(int i = 1; i < data->size(); i++) {
        if(data->at(i) < minVal) {
            minVal = data->at(i);
        }
    }
    return minVal;
}

/**
 * Поиск максимального значения в очереди данных.
 * @return - максимальное значение в очереди данных.
 */
double ChartWidget::findMax() {
    double maxVal = data->at(0);
    for(int i = 1; i < data->size(); i++) {
        if(data->at(i) > maxVal) {
            maxVal = data->at(i);
        }
    }
    return maxVal;
}

/**
 * Устанавливает изначальные размеры канала.
 * @return - размер канала.
 */
QSize ChartWidget::sizeHint() const {
    return QSize(600, 400);
}

/**
 * Устанавливает минимально возможный размер канала.
 * @return - минимальный размер канала.
 */
QSize ChartWidget::minimumSizeHint() const {
    return QSize(300, 150);
}

/**
 * Контекстное меню для канала.
 * @param ev - событие контекстного меню.
 */
void ChartWidget::contextMenuEvent(QContextMenuEvent *ev) {
    if(channelEnabled) {
        popupMenu->exec(ev->globalPos());
    }
}

/**
 * Создает события контекстного меню.
 */
void ChartWidget::initActions() {
    enabledAction = new QAction(tr("Enabled"), this);
    enabledAction->setCheckable(true);
    enabledAction->setChecked(true);
    connect(enabledAction, &QAction::toggled, this,  &ChartWidget::slotEnabledAction);

    autoIntervalAction = new QAction(tr("Auto"), this);
    connect(autoIntervalAction, &QAction::triggered, this, &ChartWidget::slotAutoMinMax);

    oneHundredthIntervalAction = new QAction("-0.01 ... +0.01", this);
    connect(oneHundredthIntervalAction, &QAction::triggered, this, &ChartWidget::slotAct0_01);

    fiveHundredthIntervalAction = new QAction("-0.05 ... +0.05", this);
    connect(fiveHundredthIntervalAction, &QAction::triggered, this, &ChartWidget::slotAct0_05);

    oneTenthIntervalAction = new QAction("-0.1 ... +0.1", this);
    connect(oneTenthIntervalAction, &QAction::triggered, this, &ChartWidget::slotAct0_1);

    fiveTenthIntervalAction = new QAction("-0.5 ... +0.5", this);
    connect(fiveTenthIntervalAction, &QAction::triggered, this, &ChartWidget::slotAct0_5);

    oneIntervalAction = new QAction("-1.0 ... +1.0", this);
    connect(oneIntervalAction, &QAction::triggered, this, &ChartWidget::slotAct1_0);

    oneAndFiveTenthIntervalAction = new QAction("-1.5 ... +1.5", this);
    connect(oneAndFiveTenthIntervalAction, &QAction::triggered, this, &ChartWidget::slotAct1_5);

    twoAndFiveTenthIntervalAction = new QAction("-2.5 ... +2.5", this);
    connect(twoAndFiveTenthIntervalAction, &QAction::triggered, this, &ChartWidget::slotAct2_5);
}

/**
 * Инициализирует контекстное меню.
 */
void ChartWidget::initPopupMenu() {
    popupMenu = new QMenu(this);

    popupMenu->addAction(enabledAction);
    popupMenu->addSeparator();
    popupMenu->addAction(autoIntervalAction);
    popupMenu->addAction(oneHundredthIntervalAction);
    popupMenu->addAction(fiveHundredthIntervalAction);
    popupMenu->addAction(oneTenthIntervalAction);
    popupMenu->addAction(fiveTenthIntervalAction);
    popupMenu->addAction(oneIntervalAction);
    popupMenu->addAction(oneAndFiveTenthIntervalAction);
    popupMenu->addAction(twoAndFiveTenthIntervalAction);
}

/**
 * Слот для чекбокса разрешающего отрисовку графика.
 * @param en
 */
void ChartWidget::slotEnabledAction(bool en) {
    enabled = en;

    autoIntervalAction->setEnabled(enabled);
    oneHundredthIntervalAction->setEnabled(enabled);
    fiveHundredthIntervalAction->setEnabled(enabled);
    oneTenthIntervalAction->setEnabled(enabled);
    fiveTenthIntervalAction->setEnabled(enabled);
    oneIntervalAction->setEnabled(enabled);
    oneAndFiveTenthIntervalAction->setEnabled(enabled);
    twoAndFiveTenthIntervalAction->setEnabled(enabled);

    this->update();
}

/**
 * Слот устанавливает минимальное и максимальное значения данных для отображения из контекстного меню.
 */
void ChartWidget::slotAct0_01() {
    autoMinMax = false;
    setMinMax(-0.01, 0.01);
}

/**
 * Слот устанавливает минимальное и максимальное значения данных для отображения из контекстного меню.
 */
void ChartWidget::slotAct0_05() {
    autoMinMax = false;
    setMinMax(-0.05, 0.05);
}

/**
 * Слот устанавливает минимальное и максимальное значения данных для отображения из контекстного меню.
 */
void ChartWidget::slotAct0_1() {
    autoMinMax = false;
    setMinMax(-0.1, 0.1);
}

/**
 * Слот устанавливает минимальное и максимальное значения данных для отображения из контекстного меню.
 */
void ChartWidget::slotAct0_5() {
    autoMinMax = false;
    setMinMax(-0.5, 0.5);
}

/**
 * Слот устанавливает минимальное и максимальное значения данных для отображения из контекстного меню.
 */
void ChartWidget::slotAct1_0() {
    autoMinMax = false;
    setMinMax(-1, 1);
}

/**
 * Слот устанавливает минимальное и максимальное значения данных для отображения из контекстного меню.
 */
void ChartWidget::slotAct1_5() {
    autoMinMax = false;
    setMinMax(-1.5, 1.5);
}

/**
 * Слот устанавливает минимальное и максимальное значения данных для отображения из контекстного меню.
 */
void ChartWidget::slotAct2_5() {
    autoMinMax = false;
    setMinMax(-2.5, 2.5);
}

/**
 * Слот устанавливает автоопределение минимального и максимального значений данных для отображения.
 */
void ChartWidget::slotAutoMinMax() {
    autoMinMax = true;
    this->update();
}

/**
 * Устанавливает минимальное и максимальное значения данных для отображения.
 * @param min - минимальное значение данных.
 * @param max - максимальное значение данных.
 */
void ChartWidget::setMinMax(double minVal, double maxVal) {
    selectedMin = minVal;
    selectedMax = maxVal;
    setMin(selectedMin);
    setMax(selectedMax);
    this->update();
}
