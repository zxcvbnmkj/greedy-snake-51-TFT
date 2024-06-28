# greedy-snake-51-TFT
## 简介
硬件上采用STC89C51单片机、LCD12864显示屏；软件上采用C语言编程，Keil软件调试生成HEX文件。
游戏中食物的随机产生，蛇吃完食物后，蛇身加长且游戏积分增加，蛇碰撞到蛇身或墙时游戏结束。
## 效果展示



https://github.com/zxcvbnmkj/greedy-snake-51-TFT/assets/133875139/1de6fba9-b811-4247-98eb-cfecd8e8ecc9



https://github.com/zxcvbnmkj/greedy-snake-51-TFT/assets/133875139/d1c5b55c-bf1f-429a-b712-5ca323a90d77



## 游戏界面状态显示
初始状态：

![image](https://github.com/zxcvbnmkj/greedy-snake-51-TFT/assets/133875139/14becb0e-f17a-4ebd-8082-3304620c0f99)

结束状态：

![image](https://github.com/zxcvbnmkj/greedy-snake-51-TFT/assets/133875139/be5bdf0a-6c51-47e8-9899-14b16d76f754)


## 游戏处理
1.接电以后，初始化游戏，在游戏区域内的固定位置出现初始贪吃蛇，蛇身长度为三个单位。并且在随机位置出现第一个豆子。

2.蛇的移动：使蛇按照方向健的方向移动。开始游戏时，蛇头和蛇尾已经固定不动，按开始健游戏开始，蛇只能向左或者向右转，不能向后。

3.刷新豆子：豆子被吃掉后在随机位置出现豆子。豆子不能出现在和蛇身或者障碍物重合，香则重新刷新豆子。

4.吃豆子：蛇头吃到豆子，蛇身变长一格，加在蛇头

5.分数：吃1个豆子增加1分

6.判定死亡：当蛇头碰到屏幕边缘，碰到码物，或者碰到蛇自己的身体时，蛇死亡。游戏结束

```
本项目继承自：https://github.com/zxcvbnmkj/simulation-furniture-control-based-on-51-microcontroller
```
