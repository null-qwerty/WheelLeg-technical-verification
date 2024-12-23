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
│   ├── include
│   └── src
├── build    # build 文件夹
├── cmake    # CubeMX 生成的 CMakeLists.txt 文件
│   ├── gcc-arm-none-eabi.cmake
│   └── stm32cubemx
├── CMakeLists.txt  # 项目根目录 CMakeLists.txt
├── CMakePresets.json
├── Core            # CubeMX 生成的代码
│   ├── Inc
│   └── Src
├── Drivers         # 驱动层代码，包括 HAL 库和 CMSIS 库，其中 DSP 库在 CMSIS 中，以子模块的形式引入
│   ├── CMSIS
│   └── STM32F4xx_HAL_Driver
├── Middlewares     # 中间件，由 CubeMX 生成
│   └── Third_Party
│       └── FreeRTOS
├── newlib_lock_glue.c
├── README.md
├── startup_stm32f407xx.s
├── STM32F407IGHx_FLASH.ld
├── stm32_lock.h
└── WheelLeg-test.ioc   # CubeMX 工程文件
```

其中 `Application` 文件夹内容如下：

+ PHOENIX: 基础库
  + BaseControl: 基础控制库，包括通信、控制器、电机驱动
    + Connectivity: 通信库，将不同的通信方式封装成统一的接口，对 HAL 库的封装
    + Controller: 控制器库
    + Motor: 电机驱动库，将不同的电机驱动方式封装成统一的接口
    + Sensor: 传感器库，将不同的传感器封装成统一的接口
  + Math: 数学库，包括三角函数、矩阵、向量、四元数、等数学运算，是对 CMSIS-DSP 库的封装
+ Utils: 工具库，目前里面只有一个没用到的微秒级延时函数
+ WheelLeg: 上层应用
  + Chassis: 底盘控制
  + Gimbal: 云台控制
  + Leg: 腿部解算
  + Task: 任务管理
  + Strategy: 决策

## 项目进度

- 通信模块
    - [x] CAN
    - [X] UART
        - [x] DBUS
    - [ ] SPI
    - [ ] I2C
- 电机驱动
    - [x] RM3508
    - [x] DM4310
    - [ ] UniTree A1
    - [ ] MF9025
- 传感器
    - [ ] BMI088
    - [ ] IST8310
- 控制器
    - [x] PID
    - [ ] LQR
- 观测器
    - [ ] 卡尔曼滤波器
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