# 轮腿平衡步兵技术验证

## 项目简介

轮腿平衡步兵技术验证代码仓库，用于探索轮腿平衡步兵的技术路线，并尝试将常用的技术模块化，以便于后续的开发。

代码使用 CubeMX + Cmake + VSCode + Ozone 进行开发，使用 FreeRTOS 作为中间件，使用 CMSIS-DSP 库做快速计算。

开发环境： Ubuntu 24.04 LTS, gcc-arm-none-eabi, C++11

> 环境搭建可以参考[这篇博客](https://blog.null-qwerty.work/2024/10/03/ubuntu-%E4%B8%8B-CubeMX-cmake-gcc-arm-none-eabi-ozone-%E5%BC%80%E5%8F%91%E7%8E%AF%E5%A2%83%E6%90%AD%E5%BB%BA/)，感谢湖南大学跃鹿战队开源的环境搭建方案。

## 项目结构

```bash
.
├── Application     # 应用层代码
│   ├── Chassis     # 底盘控制
│   ├── CMakeLists.txt
│   ├── Gimbal      # 云台控制
│   ├── include
│   ├── Leg     # 腿部解算
│   └── tasks   # 任务管理
├── build   # build 文件夹
├── cmake   # CubeMX 生成的 CMakeLists.txt 文件
│   ├── gcc-arm-none-eabi.cmake
│   └── stm32cubemx
├── CMakeLists.txt  # 项目根目录 CMakeLists.txt
├── CMakePresets.json
├── Core    # CubeMX 生成的代码
│   ├── Inc
│   └── Src
├── Drivers    # 驱动层代码，包括 HAL 库和 CMSIS 库，其中 DSP 库在 CMSIS 中，以子模块的形式引入
│   ├── CMSIS
│   └── STM32F4xx_HAL_Driver
├── LICENSE    # Apache 2.0
├── Middlewares   # 中间件，由 CubeMX 生成
│   └── Third_Party
│       └── FreeRTOS
├── newlib_lock_glue.c
├── PHOENIX    # 基础库, 是对 HAL 库的封装，包括通信、控制器、电机驱动
│   ├── BaseControl   # 基础控制库
│   ├── CMakeLists.txt
│   ├── Filter  # 观测器/滤波器
│   ├── include
│   ├── Math    # 数学库，是对 CMSIS-DSP 库的封装
│   └── Utils   # 工具库
├── README.md
├── startup_stm32f407xx.s
├── STM32F407IGHx_FLASH.ld
├── stm32_lock.h
└── WheelLeg-test.ioc   # CubeMX 工程文件
```

## 项目进度

- 通信模块
    - [x] CAN
    - [x] UART
        - [x] DBUS
    - [x] SPI
    - [x] I2C
- 电机驱动
    - [x] RM3508
    - [x] DM4310
    - [ ] UniTree A1
    - [ ] MF9025
- 传感器
    - [x] BMI088
    - [ ] IST8310
- 控制器
    - [x] PID
    - [ ] ADRC
    - [ ] LQR
- 观测器/滤波器
    - [ ] 卡尔曼滤波器
    - [ ] 扩展卡尔曼滤波器
    - [ ] 无迹卡尔曼滤波器
    - [x] Mahony
- 数学库 (**未测试**)
    - [x] CMSIS-DSP 移植
    - [x] 三角函数
    - [x] 矩阵
    - [x] 向量
    - [x] 四元数
    - [ ] 龙格库塔积分
- 应用层
    - [ ] 腿部解算
    - [ ] 姿态解算
    - [ ] 底盘控制
    - [ ] 云台控制
    - [ ] 决策及任务调度

## 使用方法

编译：

```bash
mkdir build
cd build
cmake ..
make -j
```

烧录：

```bash
make Flash
```