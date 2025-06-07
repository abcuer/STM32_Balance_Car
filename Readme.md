# 全能智能平衡小车：蓝牙遥控+自动跟随+自我保护！

![前视图](./src/7a1b74cef987346cb069695975bb60c.jpg)
![俯视图](./src/e7bc3cd2b40d70d83f9f5210f651f61.jpg)
![后视图](./src/3dd9fb143a53159b16f8972a5833be3.jpg)
![PCB](./src/0a053a80354407da4dd48eb7ba30a58.jpg)

# [PCB开源地址](https://oshwhub.com/fascinating_sea/stm32_balancecar)

# [视频演示](https://www.bilibili.com/video/BV1s4JLzUE3u/?vd_source=990d350c3d7fb93042ed4a36bf2b1c83)
## 电子模块(带商品链接)
### 总价(不算焊接工具)：￥195
- **[IN5824二极管*3(SS54 SMA) ￥2.18](https://e.tb.cn/h.6F2CfQNJmlFCtSV?tk=1M0LVkzgXYz )**
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
## 功能说明

- **平衡模式**：点亮白灯表示进入平衡模式，具备较强的抗干扰能力。
- **蓝牙避障模式**：点亮蓝灯表示进入蓝牙模式。在此模式下，可通过手机控制小车移动；当小车距离障碍物小于25cm时，小车停止运动，并触发声光报警；当障碍物远离至60cm以外时，恢复蓝牙控制功能。
- **超声波跟随模式**：点亮红灯表示进入跟随模式。小车会自动跟随前方30cm范围内的物体；若物体距离小于30cm，小车将自动后退，以保持与物体30cm的安全距离。
- **提起检测**：当小车被拿起并持续超过1秒时，小车将自动停止运行。
- **着陆检测**：当小车处于直立状态（Pitch角度在正常范围内），并且放置时间超过1秒时，小车将自动恢复运行。
- **倒地检测**：当小车的Pitch角度超过70度，并且速度超出正常范围时，小车将自动停止运行。