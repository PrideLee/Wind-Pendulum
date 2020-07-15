# Wind-Pendulum

A wind-pendulum system was designed to controlling four blades' rotate speed to realize specific motion of wind-pendulum, such as line, round, Lissajous figure, cardioid, etc. The hardware includes STM32F103 SCM, swing rod, Cardan joint, laser pointer, keyboard, blades, propeller, MPU6050 Motion Tracking devices, and DC power supply. MPU6050 was used to measure the pendulum angle, then calculate the blades' position by the trigonometric formula. PID control algorithm generates the duty cycle of PWM to control wind pendulum's motion based on the deviation of the real position and expected position.

**You can dowmload the demo from [here](https://github.com/PrideLee/Wind-Pendulum/blob/master/demo.mp4).**

**You can dowmload the full report from [here](https://github.com/PrideLee/Wind-Pendulum/blob/master/%E9%A3%8E%E5%8A%9B%E6%91%86%E6%8E%A7%E5%88%B6%E7%B3%BB%E7%BB%9F_git.pdf).**


## 摘要
   本风力摆系统以STM32F4单片机为核心，结合电机控制技术，PID控制算法。由支架，摆杆，万向结，激光笔，无刷电机，螺桨，MPU6050运动处理组件，键盘，开关电源,单片机等模块组成。实现通过键盘输入选择功能，MPU6050检测摆的运动状态和位置，并传回单片机，单片机利用PID控制算法，比例环节进行快速响应，积分环节实现无静差，微分环节减小超调，加快动态响应。从而使该系统具有良好的性能，根据反馈回来的信号，通过PWM波对风机的转速进行调整，从而使激光笔在地面上画出题目中指定长度指定方向的直线，或指定半径的圆周等功能。

<div align=center><img width="500" height="500" src="https://github.com/PrideLee/Wind-Pendulum/blob/master/wind_pendulum_cover.png"/></div>


