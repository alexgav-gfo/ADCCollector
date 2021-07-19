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

#include "adc.h"

/**
 * Конструктор потока работы с АЦП.
 * @param globalView - глобальные настройки.
 * @param channelsSets - настройки каналов.
 */
ADC::ADC(GlobalView globalView, std::vector<ChannelView> channelsSets) {
    glView = globalView;
    chSets = channelsSets;
    loggingError = false;
}

/**
 * Метод для основной работы потока.
 */
void ADC::run() {
    interrupt = false;
    uint32_t attempt = 0;
    int8_t result = SUCCESS;
    while(attempt++ < MAX_ATTEMPTS) {
        result = mainLoop();
        if(result == ADC_OPEN_ERROR) {
            QString msg = "ADC open error, check ADC connection and status";
            logging(ERROR, msg.toStdString().c_str());
            emit error(msg);
            break;
        } else if (result == ADC_FAILURE) {
            logging(ERROR, "ADC error, try recovering");
            emit error("ADC error, check ADC connection and status");
            sleep(10);
        } else if(result == IO_FAILURE) {
            logging(ERROR, "Cannot write data");
            emit error("Cannot write data on disk, disk error");
            break;
        } else if(result == ALL_CHANNELS_DISABLED) {
            emit error("All channels are disabled, please enable at least one.");
            break;
        } else {
            logging(INFO, "ADC reading success.");
            break;
        }
    }
    if(result != SUCCESS) {
        logging(ERROR, "Finished with errors");
    } else {
        logging(INFO, "Finished successfully.");
    }
}

/**
 * Логирование сообщений.
 * @param level - уровень сообщения.
 * @param message - сообщение.
 */
void ADC::logging(logLevel level, const char* message) {
    const char *lvl;
    switch(level) {
        case DEBUG:
            lvl = "DEBUG";
            break;
        case INFO:
            lvl = "INFO";
            break;
        case WARN:
            lvl = "WARNING";
            break;
        case ERROR:
            lvl = "ERROR";
            break;
        case FATAL:
            lvl = "FATAL";
            break;
        default:
            lvl = "INFO";
    }
    if(!loggingError) {
        time_t now = time(NULL);
        struct tm *t = gmtime(&now);
        char msgbuf[1024];
        snprintf(msgbuf, 1024, "%04d-%02d-%02d %02d:%02d:%02d %s: %s",
                 t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec, lvl, message);

        char log_filename[1024];
        snprintf(log_filename, 1024, "%s/datacollect.log", glView.loggingRoot.toStdString().c_str());
        FILE *lf = fopen(log_filename, "a");
        if (lf == NULL) {
            loggingError = true;
            emit error(QString("Cannot open a log file for writing"));
            return;
        }
        fprintf(lf, "%s\n", msgbuf);
        fflush(lf);
        fclose(lf);
    }
}

/**
 * Инициализация подсистемы USB.
 * @return - код ошибки.
 */
int8_t ADC::usbInit() {
    int res = libusb_init(&usbContext);
    if (res < 0) {
        logging(FATAL, "USB init error");
        usbContext = NULL;
        return ADC_FAILURE;
    }
    return SUCCESS;
}

/**
 * Закрытие подсистемы USB.
 */
void ADC::usbClose() {
    if(usbContext) {
        libusb_exit(usbContext);
    }
}

/**
 * Открытие устройства.
 * @param idVendor - Vendor ID.
 * @param idProduct - Product ID.
 * @return - дескриптор устройства.
 */
libusb_device_handle *ADC::openAdc(uint16_t idVendor, uint16_t idProduct) {
    logging(INFO, "Open ADC...");
    libusb_device_handle *handle = NULL;
    if (usbContext == NULL) {
        logging(FATAL, "Attempt to call open_adc() without init USB subsystem");
        return NULL;
    }
    libusb_device **devices;
    ssize_t cnt = libusb_get_device_list(usbContext, &devices);
    if (cnt < 0) {
        logging(FATAL, "Get devices list error");
        return NULL;
    }
    for(int32_t i = 0; i < cnt; i++) {
        struct libusb_device_descriptor desc;
        int res = libusb_get_device_descriptor(devices[i], &desc);
        if (res < 0) {
            logging(FATAL, "Failed to get device descriptor");
            return NULL;
        }
        if (desc.idVendor == idVendor && desc.idProduct == idProduct) {
            res = libusb_open(devices[i], &handle);
            if (res < 0) {
                logging(FATAL, "Failed to open device");
                return NULL;
            }
            break;
        }
    }
    libusb_free_device_list(devices, 1);
    logging(INFO, "ADC opened successfully.");
    return handle;
}

/**
 * Отправка команды на устройство.
 * @param handle - дескриптор.
 * @param b1 - первый байт команды.
 * @param b2 - второй байт команды.
 * @param b3 - третий байт команды.
 * @param b4 - четвертый байт команды.
 * @param delay - задержка (msec).
 * @return - число байт, переданных на устройство.
 */
uint8_t ADC::sendCmd(libusb_device_handle *handle, uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4, uint32_t delay) {
    uint8_t cmd[4];
    cmd[0] = b1;
    cmd[1] = b2;
    cmd[2] = b3;
    cmd[3] = b4;
    int len;
    libusb_bulk_transfer(handle, EPOUT1, cmd, 4, &len, BULK_TRANSFER_TIMEOUT);
    usleep(delay * 1000);
    return len;
}

/**
 * Запуск АЦП.
 * @param handle - дескриптор.
 * @return - код ошибки.
 */
int8_t ADC::startAdc(libusb_device_handle *handle) {
    int status;
    status = sendCmd(handle, C_START_STOP, 0, 0, 0, 1);
    if (status != 4) {
        logging(FATAL, "Stop ADC: cannot send command");
        return ADC_FAILURE;
    }
    status = sendCmd(handle, C_RESET, 0, 0, 0, 1);
    if (status != 4) {
        logging(FATAL, "Reset ADC: cannot send command");
        return ADC_FAILURE;
    }
    status = sendCmd(handle, C_FREQ_SET, 0, 0, FREQ_800, 1);
    if (status != 4) {
        logging(FATAL, "Set frequency: cannot send command");
        return ADC_FAILURE;
    }
    status = sendCmd(handle, C_GAIN_SET, 0, 0, 0, 100);
    if (status != 4) {
        logging(FATAL, "Set gain on channel 1: cannot send command");
        return ADC_FAILURE;
    }
    status = sendCmd(handle, C_GAIN_SET, 1, 0, 0, 100);
    if (status != 4) {
        logging(FATAL, "Set gain on channel 2: cannot send command");
        return ADC_FAILURE;
    }
    status = sendCmd(handle, C_GAIN_SET, 2, 0, 0, 100);
    if (status != 4) {
        logging(FATAL, "Set gain on channel 3: cannot send command");
        return ADC_FAILURE;
    }
    status = sendCmd(handle, C_GAIN_SET, 3, 0, 0, 100);
    if (status != 4) {
        logging(FATAL, "Set gain on channel 4: cannot send command");
        return ADC_FAILURE;
    }
    status = sendCmd(handle, C_FREQ_SET, 0, 0, getAdcFreq(glView.frequency), 1);
    if (status != 4) {
        logging(FATAL, "Set frequency: cannot send command");
        return ADC_FAILURE;
    }
    status = sendCmd(handle, C_START_STOP, 1, 0, 0, 1);
    if (status != 4) {
        logging(FATAL, "Start ADC: cannot send command");
        return ADC_FAILURE;
    }
    return SUCCESS;
}

/**
 * Остановка АЦП.
 * @param handle - дескриптор.
 * @return - код ошибки.
 */
int8_t ADC::stopAdc(libusb_device_handle *handle) {
    int status;
    status = sendCmd(handle, C_START_STOP, 0, 0, 0, 1);
    if (status != 4) {
        logging(FATAL, "Stop ADC: cannot send command");
        return ADC_FAILURE;
    }
    status = sendCmd(handle, C_RESET, 0, 0, 0, 1);
    if (status != 4) {
        logging(FATAL, "Reset ADC: cannot send command");
        return ADC_FAILURE;
    }
    return SUCCESS;
}

/**
 * Чтение данных с АЦП.
 * @param handle - дескриптор.
 * @return - код ошибки.
 */
int8_t ADC::readData(libusb_device_handle *handle) {
    if (handle == NULL) {
        logging(FATAL, "Attempt to read data from null handler");
        return ADC_FAILURE;
    }

    uint8_t buf[DATABUF_LEN];
    int32_t ch[NUM_CHANNELS][CHANBUF_LEN];
    uint8_t ch_counter[NUM_CHANNELS] = {0, 0, 0, 0};
    int len;

    libusb_bulk_transfer(handle, EPIN1, buf, DATABUF_LEN, &len, BULK_TRANSFER_TIMEOUT);

    struct timeval tv;
    int res = gettimeofday(&tv, NULL);
    if (res < 0) {
        logging(ERROR, "Cannot get current time in milliseconds");
        return ADC_FAILURE;
    }

    if (len != DATABUF_LEN) {
        logging(FATAL, "Read data: wrong received data length (should be 512)");
        return ADC_FAILURE;
    }

    for (uint16_t i = 0; i < DATABUF_LEN; i += 4) {
        int32_t data = (buf[i + 1] << 8) + (buf[i + 2] << 16) + (buf[i + 3] << 24);
        uint8_t chan = (buf[i] & 0xf0) >> 4;
        if (chan <= 3) {
            ch[chan][ch_counter[chan]++] = data;
        }
    }

    // Получение данных для отображения графиков:
    const int MEAN_COEFF = 32;
    for(int i = 0; i < NUM_CHANNELS; i++) {
        int32_t meanBuf[1];
        meanChanData(ch[i], CHANBUF_LEN, meanBuf, MEAN_COEFF);
        double value = (double)meanBuf[0] / 0x7fffff00 * 2.500;
        channelsData[i] = value;
    }

    // Write data
    for (uint8_t i = 0; i < NUM_CHANNELS; i++) {
        if (chSets.at(i).enabled) {
            int8_t writeRes;
            if(chSets.at(i).saveTextData) {
                writeRes = writeText(ch[i], CHANBUF_LEN, i, &tv);
                if(writeRes < 0) {
                    return IO_FAILURE;
                }
            }
            if(chSets.at(i).saveBinaryData) {
                writeRes = writeData(ch[i], CHANBUF_LEN, i, &tv);
                if (writeRes < 0) {
                    return IO_FAILURE;
                }
            }
        }
    }
    return SUCCESS;
}

/**
 * Запись данных.
 * @param chan_data - данные каналов.
 * @param len - длина данных каналов.
 * @param chan_num - номер канала.
 * @param tv - время.
 * @return - код ошибки.
 */
int8_t ADC::writeData(int32_t *chan_data, uint16_t len, uint8_t chan_num, struct timeval *tv) {
    time_t time_sec = tv->tv_sec;
    struct tm *gt = gmtime(&time_sec);
    const uint16_t FULL_NAME_LEN = PATH_LEN + FILE_LEN + 1;
    char file_path[PATH_LEN];
    char file_name[FILE_LEN];
    char full_name[FULL_NAME_LEN];
    int year = gt->tm_year + 1900;
    int mon  = gt->tm_mon + 1;
    int day  = gt->tm_mday;
    int hour = gt->tm_hour;
    if(!glView.dataInOneFile) {
        snprintf(file_path, PATH_LEN, "%s/%04d/%02d/%02d", glView.dataRoot.toStdString().c_str(), year, mon, day);
        snprintf(file_name, FILE_LEN, "%02d%02d%02d_%02d.%02d", year, mon, day, hour, chan_num);
        int8_t res = mkdirs(file_path, PATH_LEN, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
        if (res < 0) {
            return IO_FAILURE;
        }
        snprintf(full_name, FULL_NAME_LEN, "%s/%s", file_path, file_name);
    } else {
        snprintf(full_name, FULL_NAME_LEN, "%s/data_ch%d.dat", glView.dataRoot.toStdString().c_str(), chan_num);
    }

    // Open file to write (append)
    FILE *f = fopen(full_name, "a");
    if (!f) {
        logging(ERROR, "Cannot open a file for writing");
        return IO_FAILURE;
    }

    int write_result;

    // Write header
    uint8_t h[4] = {0xff, 0xff, 0xff, 0xff};
    write_result = fwrite(&h, 4, 1, f);
    if(write_result < 0) {
        logging(ERROR, "Cannot write header to file");
        fclose(f);
        return IO_FAILURE;
    }

    // Write timestamp
    uint64_t msec = (uint64_t)tv->tv_sec * 1000 + ((uint32_t)tv->tv_usec / 1000);
    write_result = fwrite(&msec, sizeof(uint64_t), 1, f);
    if (write_result != 1) {
        logging(ERROR, "Cannot write timestamp to file");
        fclose(f);
        return IO_FAILURE;
    }

    // Write data
    int32_t mean_buf[CHANBUF_LEN];
    uint16_t writeLen;
    if (glView.meaningDataBuffer == 0) {
        writeLen = CHANBUF_LEN;
        write_result = fwrite(chan_data, sizeof(int32_t), writeLen, f);
    } else {
        writeLen = CHANBUF_LEN / glView.meaningDataBuffer;
        meanChanData(chan_data, len, mean_buf, glView.meaningDataBuffer);
        write_result = fwrite(mean_buf, sizeof(int32_t), writeLen, f);
    }
    if (write_result != len) {
        logging(ERROR, "Cannot write current data buffer to file");
        fclose(f);
        return IO_FAILURE;
    }

    fflush(f);
    fclose(f);

    return SUCCESS;
}

/**
 * Запись данных в виде текста.
 * @param chan_data - данные каналов.
 * @param len - длина данных каналов.
 * @param chan_num - номер канала.
 * @param tv - время.
 * @return - код ошибки.
 */
int8_t ADC::writeText(int32_t *chan_data, uint16_t len, uint8_t chan_num, struct timeval *tv) {
    time_t time_sec = tv->tv_sec;
    struct tm *gt = gmtime(&time_sec);
    const uint16_t FULL_NAME_LEN = PATH_LEN + FILE_LEN + 1;
    char file_path[PATH_LEN];
    char file_name[FILE_LEN];
    int year = gt->tm_year + 1900;
    int mon  = gt->tm_mon + 1;
    int day  = gt->tm_mday;
    int hour = gt->tm_hour;

    char full_name[FULL_NAME_LEN];
    if(!glView.dataInOneFile) {
        snprintf(file_path, PATH_LEN, "%s/%04d/%02d/%02d", glView.dataRoot.toStdString().c_str(), year, mon, day);
        snprintf(file_name, FILE_LEN, "%02d%02d%02d_%02d_%02d.txt", year, mon, day, hour, chan_num);
        int8_t res = mkdirs(file_path, PATH_LEN, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
        if (res < 0) {
            return IO_FAILURE;
        }
        snprintf(full_name, FULL_NAME_LEN, "%s/%s", file_path, file_name);
    } else {
        snprintf(full_name, FULL_NAME_LEN, "%s/data_ch%d.txt", glView.dataRoot.toStdString().c_str(), chan_num);
    }
    FILE *f = fopen(full_name, "a");
    if (!f) {
        logging(ERROR, "Cannot open a file for writing");
        return IO_FAILURE;
    }

    int res;
    if(glView.meaningDataBuffer == 0) {
        res = writeTextData(f, chan_data, len, gt);
    } else {
        int32_t meanBuf[CHANBUF_LEN];
        meanChanData(chan_data, len, meanBuf, glView.meaningDataBuffer);
        res = writeTextData(f, meanBuf, len / glView.meaningDataBuffer, gt);
    }
    fclose(f);
    return res;
}

/**
 * Запись данных в текстовый файл.
 * @param f - дескриптор файла.
 * @param data - буфер данных.
 * @param len - длина буфера.
 * @param tm - время.
 * @return - код ошибки.
 */
int8_t ADC::writeTextData(FILE *f, int32_t *data, size_t len, struct tm *tm) {
    int year = tm->tm_year + 1900;
    int mon  = tm->tm_mon + 1;
    int day  = tm->tm_mday;
    int hour = tm->tm_hour;
    int min = tm->tm_min;
    int sec = tm->tm_sec;
    for(size_t i = 0; i < len; i++) {
        float val = (float)data[i] / 0x7fffff00 * 2.500;
        int res = fprintf(f, "%04d-%02d-%02d %02d:%02d:%02d  %f\n", year, mon, day, hour, min, sec, val);
        if(res < 0) {
            logging(ERROR, "Cannot write text data");
            return IO_FAILURE;
        }
    }
    return SUCCESS;
}

/**
 * Получение кода установки частоты АЦП.
 * @param freq - частота в Гц.
 * @return - код частоты.
 */
uint8_t ADC::getAdcFreq(int freq) {
    uint8_t result;
    switch(freq) {
        case 25:
            result = FREQ_25;
            break;
        case 50:
            result = FREQ_50;
            break;
        case 100:
            result = FREQ_100;
            break;
        case 200:
            result = FREQ_200;
            break;
        case 400:
            result = FREQ_400;
            break;
        case 800:
            result = FREQ_800;
            break;
        default:
            result = FREQ_100;
    }
    return result;
}

/**
 * Создание каталогов для сохранения данных.
 * @param path - корневой путь.
 * @param path_len - длина пути.
 * @param mode - режим создания.
 * @return - код ошибки.
 */
int8_t ADC::mkdirs(const char *path, const u_int16_t path_len, mode_t mode) {
    const uint8_t BUF_LEN = 128;
    const u_int8_t DELIM_POS_LEN = 128;

    char buf[BUF_LEN];
    uint8_t delim_pos[DELIM_POS_LEN];
    uint8_t delim_cnt = 0;
    uint16_t path_cnt = 0;

    // Find delimiters ('/') in path and save it's positions to delim_pos array
    while(path_cnt < path_len) {
        if (path[path_cnt] == '/' || path[path_cnt] == '\0') {
            delim_pos[delim_cnt++] = path_cnt;
            if(delim_cnt >= DELIM_POS_LEN) {
                break;
            }
        }
        if (path[path_cnt] == '\0') {
            break;
        }
        path_cnt++;
    }

    // Split path to pieces and make directories
    for (uint8_t i = (path[0] == '/' ? 1 : 0); i < delim_cnt && i < BUF_LEN; i++) {
        uint8_t p = delim_pos[i];
        for (uint8_t j = 0; j < p; j++) {
            buf[j] = path[j];
        }
        buf[p] = '\0';
        int res = mkdir(buf, mode);
        if (res < 0 && errno != EEXIST) {
            logging(ERROR, "Cannot make a directory for writing data");
            return IO_FAILURE;
        }
    }

    return SUCCESS;
}

/**
 * Усреднение данных.
 * @param chan_data - данные каналов.
 * @param chan_size - размер данных каналов.
 * @param mean_buf - выходной буфер.
 * @param aver - коэффициент усреднения.
 */
void ADC::meanChanData(const int32_t *chan_data, uint8_t chan_size, int32_t *mean_buf, uint8_t aver) {
    uint8_t buf_cnt = 0;
    int64_t sum = 0;
    u_int8_t c = 0;
    do {
        if (c != 0 && c % aver == 0) {
            mean_buf[buf_cnt++] = (int32_t)(sum / aver);
            sum = 0;
        }
        sum += chan_data[c];
        c++;
    } while (c <= chan_size);
}

/**
 * Основной цикл сбора данных.
 * @return - код ошибки.
 */
int8_t ADC::mainLoop() {
    int8_t res;

    // Init USB subsystem
    res = usbInit();
    if (res < 0) {
        return ADC_OPEN_ERROR;
    }

    // Check channels enable. If all channels are disabled, return an error.
    uint8_t ch_enable = 0;
    for (uint8_t i = 0; i < NUM_CHANNELS; i++) {
        if (chSets.at(i).enabled == 1) { //
            ch_enable = 1;
            break;
        }
    }
    if (ch_enable == 0) {
        logging(ERROR, "All channels are disabled.");
        return ALL_CHANNELS_DISABLED;
    }

    // Open ADC
    libusb_device_handle *dev_handle = openAdc(VENDOR_ID, PRODUCT_ID);
    if (dev_handle == NULL) {
        return ADC_OPEN_ERROR;
    }

    // Start ADC
    res = startAdc(dev_handle);
    if (res == SUCCESS) {
        // Data read loop
        while(true) {
            // Check free space
            fs::space_info info = fs::space(glView.dataRoot.toStdString());
            if(info.available == 0) {
                logging(ERROR, "No space left on device");
                res = IO_FAILURE;
                break;
            }
            res = readData(dev_handle);
            if (res == ADC_FAILURE) {
                logging(ERROR, "Error reading data from ADC, stop main loop");
                break;
            } else if(res == IO_FAILURE) {
                logging(ERROR, "Cannot write data on disk");
                break;
            }
            if(interrupt) {
                break;
            }
        }
    }
    // Stop ADC
    if(!stopAdc(dev_handle)) {
        logging(ERROR, "Failed to stop ADC, ADC error");
    }
    logging(INFO, "Closing ADC");
    // Close ADC device
    libusb_close(dev_handle);
    return res;
}

/**
 * Остановка потока.
 */
void ADC::stop() {
    interrupt = true;
}

/**
 * Установка настроек.
 * @param globalView - глобальные настройки.
 * @param channelsSets - настройки каналов.
 */
void ADC::setSettings(GlobalView globalView, std::vector<ChannelView> channelsSets) {
    glView = globalView;
    chSets = channelsSets;
}

/**
 * Получение данных для отображения.
 * @return - данные для отображения.
 */
std::vector<double> ADC::getChannelData() {
    return channelsData;
}
