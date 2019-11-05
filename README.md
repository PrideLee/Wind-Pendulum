# Wind-Pendulum

- We design a wind-pendulum, through controling four blades' rotate speed to realize specific motion of wind-pendulum, such as line, cicle, Lissajous figure, cardioid, etc.
- Hardwares include STM32F103 SCM, swing rod, cardan joint, laser pointer, keyboard, bldcm, propeller, MPU6050 Motion Tracking devices, and DC power supply.
- Using MPU6050 measure the pendulum angle, then calculating the position of bldcm by trigonometric formul. Based on the deviation of real position and expected position, PID algorithm output the duty cycle of PWM to control wind-pendulum's motion.

## 摘要
   本风力摆系统以STM32F4单片机为核心，结合电机控制技术，PID控制算法。由支架，摆杆，万向结，激光笔，无刷电机，螺桨，MPU6050运动处理组件，键盘，开关电源,单片机等模块组成。实现通过键盘输入选择功能，MPU6050检测摆的运动状态和位置，并传回单片机，单片机利用PID控制算法，比例环节进行快速响应，积分环节实现无静差，微分环节减小超调，加快动态响应。从而使该系统具有良好的性能，根据反馈回来的信号，通过PWM波对风机的转速进行调整，从而使激光笔在地面上画出题目中指定长度指定方向的直线，或指定半径的圆周等功能。

<\center>
[![Watch the video](https://github.com/PrideLee/Wind-Pendulum/blob/master/wind_pendulum_cover.png)](https://github.com/PrideLee/Wind-Pendulum/blob/master/demo.mp4)


