# 全能智能平衡小车：蓝牙遥控+自动跟随+自我保护！
# [视频演示](https://www.bilibili.com/video/BV1s4JLzUE3u)
## [PCB开源](https://oshwhub.com/fascinating_sea/stm32_balancecar)
## 标准库：https://github.com/abcuer/STM32_Balance_Car
## HAL/RTOS：https://github.com/abcuer/HAL_Balance_Car
## 功能说明

- **平衡模式**：**白灯**表示**平衡模式**，具备较强的稳定平衡能力。
- **蓝牙避障模式**：**蓝灯**表示**蓝牙模式**。可通过手机控制小车运动；当小车距离物体**小于设定距离**时，小车触发**声光警报**；当小车处于**安全距离**，**恢复正常**。
- **超声波跟随模式**：**红灯**进入**跟随模式**。小车会**跟随设定距离内的物体**；若物体距离**小于设定距离**，小车将**自动后退**，与物体**保持安全距离**。
- **提起检测**：当小车**被拿起并持续一段时间**后，小车将**停止运行**。
- **着陆检测**：当小车处于**直立状态，放置地上一段时间**后，小车将**恢复运行**。
- **倒地检测**：当小车处于**倒地状态**，小车将**停止运行**。
## 电子模块(带商品链接)
- [IN5824二极管*3(SS54 SMA) ￥2.18](https://e.tb.cn/h.6F2CfQNJmlFCtSV?tk=1M0LVkzgXYz )
- [塔克 R5 Pro系列两轮自平衡小车 ￥106](https://e.tb.cn/h.6uAF5g45EmSc1Lb?tk=GEIKVkA9akD)
- [STM32F103C8T6最小系统板(进口-typec口) ￥9](https://e.tb.cn/h.6F2Kzzjs2VY6GzD?tk=upy0VkzbU8N)
- [MPU6050陀螺仪(进口) ￥11](https://e.tb.cn/h.6F2REAKSzepmDb5?tk=JMgfVkBHsvY)
- [0.96寸OLED显示屏(GND开头) ￥7](https://e.tb.cn/h.6FaghWoRMnjl6Zf?tk=mbYlVkz4WC8)
- [HC-SR04测距模块 ￥3.1](https://e.tb.cn/h.6F29ufjsiZKU3F7?tk=2HK8VkBFq1j)
- [Tb6612FNG电机驱动 ￥7.5](https://e.tb.cn/h.6F2A73fFesjWPn6?tk=6jn1VkziE4Q)
- [有源蜂鸣器(低电平触发) ￥2.8](https://e.tb.cn/h.6F2PDBCUr0MiSKL?tk=fSuMVkBEkeY)
- [3mm LED灯 ￥2.72]( https://e.tb.cn/h.6FahatjLZO8mKRz?tk=iPnXVkAzauR)
- [按键(6 6 5mm直插) ￥2.1]( https://e.tb.cn/h.6uzuv80ohaMW2xr?tk=CVcmVkzlw2f )
- [0603贴片电容10uF(滤波) ￥2]( https://e.tb.cn/h.6uw25iNHxM6SlGP?tk=qVt0VkzE5cR )
- [SS12D10耐压开关(建议弯脚) ￥2](https://e.tb.cn/h.6uBBTqe01U2vcxu?tk=QgzGVkBKpRA)
- [5.5*2.1DC插口(耐高温) ￥2.28]( https://e.tb.cn/h.6FaZAarcmwBMbPl?tk=mwa4VkBzccQ)
- [DC-DC降压模块固定输出 5V ￥3.4](https://e.tb.cn/h.6FatQAOa1dSCjAw?tk=34RnVkzJe2M )
- [12V锂电池2500mAh(DC公母头) ￥22.6](https://e.tb.cn/h.6FaEa2BSxeb0pBQ?tk=a8VHVkztrz0)
- [1*4p 1*8p 1*20p排母 ￥7](https://e.tb.cn/h.6FZKytSz8cwDzKC?tk=oqIVVkAIaMW)
- [1*2p排针 ￥2.3]( https://e.tb.cn/h.6FZsiVNPa38YEo6?tk=emUCVkAHc1w )

![前视图](./src/7a1b74cef987346cb069695975bb60c.jpg)
![俯视图](./src/e7bc3cd2b40d70d83f9f5210f651f61.jpg)
![后视图](./src/3dd9fb143a53159b16f8972a5833be3.jpg)
![PCB](./src/0a053a80354407da4dd48eb7ba30a58.jpg)