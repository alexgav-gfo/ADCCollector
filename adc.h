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

#ifndef ADCCOLLECTOR_ADC_H
#define ADCCOLLECTOR_ADC_H
#include <QThread>
#include <vector>
#include "settings.h"
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <signal.h>
#include <ctime>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <libusb-1.0/libusb.h>
#include <filesystem>

namespace fs = std::filesystem;

// Vendor & product IDs
#define VENDOR_ID    0x534B
#define PRODUCT_ID   0xC372
// ADC endpoint addresses
#define EPOUT1       0x02
#define EPOUT2       0x04
#define EPIN1        0x86
#define EPIN2        0x88
// ADC command set
#define C_START_STOP 0x01
#define C_POWER      0x02
#define C_GET_STATUS 0x03
#define C_FREQ_SET   0x04
#define C_CHAN_USE   0x0A
#define C_SYNC       0x12
#define C_GAIN_SET   0x18
#define C_PORT_OUT   0x20
#define C_PORT_IN    0x21
#define C_DAC_OUT    0x23
#define C_RESET      0x32
#define C_REG_WRITE  0xB0
#define C_REG_READ   0x70
// Frequency
#define FREQ_25      0x82
#define FREQ_50      0x83
#define FREQ_100     0x84
#define FREQ_200     0x85
#define FREQ_400     0x86
#define FREQ_800     0x87
// Buffers length
#define NUM_CHANNELS 4
#define DATABUF_LEN  512
#define CHANBUF_LEN  32
// Misc
#define MAX_ATTEMPTS 100
#define BULK_TRANSFER_TIMEOUT 2000
#define FILE_LEN 256
#define PATH_LEN 256

// Logging levels
enum logLevel {
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL
};

enum errcodes {
    SUCCESS = 0,
    ADC_OPEN_ERROR = -1,
    ADC_FAILURE = -2,
    IO_FAILURE = -3,
    ALL_CHANNELS_DISABLED = -4
};

/**
 * Поток работы с АЦП ЛА-и24USB.
 */
class ADC : public QThread {
Q_OBJECT
public:
    ADC(GlobalView globalView, std::vector<ChannelView> channelsSets);

    void stop();
    void setSettings(GlobalView globalView, std::vector<ChannelView> channelsSets);
    std::vector<double> getChannelData();

protected:
    void run() override;

private:
    GlobalView glView;
    std::vector<ChannelView> chSets;
    std::vector<double> channelsData {0, 0, 0, 0};

    libusb_context *usbContext = NULL;
    int32_t monitoring_data[NUM_CHANNELS];
    time_t monitoring_time;
    bool interrupt;
    bool loggingError;

    int8_t mainLoop();
    void logging(logLevel level, const char* message);
    int8_t usbInit();
    void usbClose();
    uint8_t sendCmd(libusb_device_handle *handle, uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4, uint32_t delay);
    libusb_device_handle *openAdc(uint16_t idVendor, uint16_t idProduct);
    int8_t startAdc(libusb_device_handle *handle);
    int8_t stopAdc(libusb_device_handle *handle);
    int8_t readData(libusb_device_handle *handle);
    int8_t writeData(int32_t *chan_data, uint16_t len, uint8_t chan_num, struct timeval *tv);
    int8_t writeText(int32_t *chan_data, uint16_t len, uint8_t chan_num, struct timeval *tv);
    int8_t writeTextData(FILE *f, int32_t *data, size_t len, struct tm *tm);
    uint8_t getAdcFreq(int freq);
    int8_t mkdirs(const char *path, const u_int16_t path_len, mode_t mode);
    void meanChanData(const int32_t *chan_data, uint8_t chan_size, int32_t *mean_buf, uint8_t aver);

signals:
    void error(QString message);
};


#endif //ADCCOLLECTOR_ADC_H
