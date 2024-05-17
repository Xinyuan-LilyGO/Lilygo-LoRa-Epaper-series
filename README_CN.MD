<h1 align = "center">🌟LilyGo LoRa Epaper Series🌟</h1>

## **[English](./README.MD) | 中文**

## 1️⃣快速开始:

### 🍇 PlatformIO Quick Start (Recommended)

- 安装`Visual Studio Code`和`Python`
- 在`Visual Studio Code`扩展中搜索`PlatformIO.IDE`插件并安装它，安装完成后，需要重新启动`Visual Studio Code`
- 重新启动`Visual Studio Code`后，选择`Visual Studio Code`左上角的`File`->`Open Folder`->`LILYGO-LORA-EPAPER-SERIES`目录
- 打开`platformio.ini`文件，在platformio列中单击取消其中一行`default_envs` = xxxx的注释，以确保只有一行有效
- 点击左下角的(✔)符号进行编译
- 将电路板连接到计算机的USB接口
- 点击(→)上传固件
- 点击(插头符号)监控串口输出


### 🍈Arduino IDE Quick Start
- 安装 [Arduino IDE](https://www.arduino.cc/en/software)
- 安装 [Arduino ESP32 V 2.0.5 or above and below V3.0](https://docs.espressif.com/projects/arduino-esp32/en/latest/)
- 下载`Lilygo-LoRa-Epaper-series`，移动到Arduino库文件夹(例如:C:\Users\YourName\Documents\Arduino\libraries)
- 将[lib文件夹](./lib/)中的所有文件夹复制到Arduino库文件夹(例如C:\Users\YourName\Documents\Arduino\libraries)
- 进入下载的`T-Display-S3/examples`目录
- `Lilygo-LoRa-Epaper-series`选择`ESP32S3 Dev Module`
- 在端口中选择板子的端口
- 请在每个示例中的 `utilities.h` 文件的上方根据你的板子型号取消注释,否则编译将报错误
- 上传


## 2️⃣示例描述

```
├── Display        
│   ├── GxEPD_U8G2_Fonts_Demo
│   └── GxEPD_picture_examples
├─ RadioLibExamples              # RadioLib examples,Support SX1276/78/62/80
│  ├─SX1262
│  │  ├─SX1262_Receive_Interrupt
│  │  └─SX1262_Transmit_Interrupt
│  ├─SX1268
│  │  ├─SX1268_Receive_Interrupt
│  │  └─SX1268_Transmit_Interrupt
│  ├─SX1276
│  │  ├─SX1276_Receive_Interrupt
│  │  └─SX1276_Transmit_Interrupt
│  ├─SX1278
│  │  ├─SX1278_Receive
│  │  ├─SX1278_Receive_Interrupt
│  │  ├─SX1278_Transmit
│  │  └─SX1278_Transmit_Interrupt
│  ├─SX1280
│  │  ├─SX128x_Receive_Interrupt
│  │  └─SX128x_Transmit_Interrupt
├──SD        
│   ├── SD_Test
│   └── SD_Epaper_Test
└─SleepTest
```


## 3️⃣引脚映射

- 查看每个示例文件夹中的 `utilities.h`


## 4️⃣数据手册
- [SX1262](https://www.semtech.com/products/wireless-rf/lora-transceivers/sx1262)
- [SX1278](https://www.semtech.com/products/wireless-rf/lora-transceivers/sx1278)
- [SX1276](https://www.semtech.com/products/wireless-rf/lora-transceivers/sx1276)
- [SX1280](https://www.semtech.cn/products/wireless-rf/lora-connect/sx1280)


## 5️⃣注意事项

1. LoRa设置输出功率后需要设置最低电流。设置sx1262如下 :

```
    // digitalWrite(LoRa_Busy, LOW);
    // set output power to 10 dBm (accepted range is -17 - 22 dBm)
    if (radio.setOutputPower(22) == RADIOLIB_ERR_INVALID_OUTPUT_POWER) {
        Serial.println(F("Selected output power is invalid for this module!"));
        while (true);
    }


    // set over current protection limit to 80 mA (accepted range is 45 - 240 mA)
    // NOTE: set value to 0 to disable overcurrent protection
    if (radio.setCurrentLimit(80) == RADIOLIB_ERR_INVALID_CURRENT_LIMIT) {
        Serial.println(F("Selected current limit is invalid for this module!"));
        while (true);
    }
```
2. 墨水屏取模:

注意:显示图片的大小要与墨水屏的宽高一致 否则可能无法达到显示的效果

下图为参考图片: ⚠️图片的大小为250*128

![product](src\image2lcd.jpg)

使用取模软件`image2lcd`

选择`输出数据类型`为`C语言数组`

选择`扫描模式`为`垂直扫描`

选择`输出灰度`为`单色`

根据墨水屏的大小选择`最大宽度和高度` 

可以参考图片

保存文件即可,打开文件后需要将数组内的前六位删除 并加入引用即可使用



## 6️⃣FAQ:
