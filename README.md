# AirSense智能空气检测仪

## 1. 开发环境 ##
**IDE :**
Keil v5.12
Keil.STM32F1xx_DFP.1.1.0
Visual Studio 2013 (Optional)

**MCU :**
STM32F103C8T6, 72MHz, 64k Flash, 20k RAM
Use SWD debug interface

## 2. 目录结构 ##
```
AirSense:.
│  README.md              (本文件)
│  
├─APP                     (应用程序源文件)
│      main.c             (main函数)
│      BSP.c              (BSP)
│      BSP.h          
│      HMI.c              (HMI模块)
│      HMI.h          
│      Sensor.c           (传感器模块)
│      Sensor.h       
│      WiFi.c             (WiFi模块)
│      WiFi.h         
│      
├─MDK                     (Keil工程)
│  │  
│  ├─Listings             (编译输出的Listing中间文件)
│  │      
│  ├─Objects              (编译输出的Object中间文件)
│  │      
│  └─RTE                  (部分库函数源文件)
│                  
├─uCOS
│  ├─CONFIG
│  │      includes.h      (uCOS主包含文件)
│  │      os_cfg.h        (uCOS配置文件)
│  │      
│  ├─CORE                 (uCOS内核源文件)
│  │      
│  └─PORT                 (uCOS底层相关移植文件)
│          os_cpu.h
│          os_cpu_a.asm
│          os_cpu_c.c
│          
└─VisualStudio            (Visual Studio工程)
```

进行应用编程时只需修改`APP`文件夹中的内容即可。

## 3. 各模块主要功能说明 ##

### 3.1. BSP模块
`BSPInit()`函数先完成SysTick初始化，之后调用其它初始化函数，完成系统初始化工作

`MCO_ENABLE`宏确定测试点是作为MCO输出时钟还是作为GPIO使用。当`MCO_ENABLE == 0u`时，可调用`TP_H`与`TP_L`这两个宏来控制测试点输出

`TaskInit()`函数创建其它系统任务

实现了`fputc()` & `fgetc()`以支持`printf()` & `scanf()`





