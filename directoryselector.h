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

#ifndef ADCCOLLECTOR_DIRECTORYSELECTOR_H
#define ADCCOLLECTOR_DIRECTORYSELECTOR_H
#include <QWidget>
#include <QLabel>
#include <QToolButton>
#include <QLineEdit>
#include <QBoxLayout>
#include <QDebug>
#include <QDir>
#include <QFileDialog>

/**
 * Селектор каталогов.
 * Виджет, позволяющий выбрать каталог для записи данных.
 */
class DirectorySelector : public QWidget {
Q_OBJECT
public:
    explicit DirectorySelector(QString name, QString path, QWidget *parent = nullptr);
    QString getDirectoryPath();

private:
    QLabel *name;
    QToolButton *selectDirectoryBtn;
    QLineEdit *directoryPath;
    QHBoxLayout *labels;
    QString path;

private slots:
    void slotCheckDirectoryPath(const QString &str);
    void slotSelectDirectory();

};


#endif //ADCCOLLECTOR_DIRECTORYSELECTOR_H
