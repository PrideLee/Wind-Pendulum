;分号后面的为注释，请根据注释说明进行相应的参数修改
; 基本格式是Mxxx Ixxx,Mxxx Txxx或者Mxxx Sxxx,其中Ixx(是字母I,integer的首字母，不是数字1)后面接的是整数,可以为十进制，也可以为十六进制(以0x开头)，Sxx后面接的是浮点数
; 
M8000 I1    ;;显示屏上面的编码器支持与否，I1表示支持，I0表示不支持,设置完后重启才生效
M8001 I1    ;;非自锁式复位开关支持与否，I1表示支持，I0表示不支持,设置完后重启才生效

;下面是步进电机方向控制，I1与I-1的方向刚好相反，所以，如果电机方向不对，要么改接线，要么改这个方向参数
M8002 I-1;		;X步进电机方向，I1或I-1
M8003 I1		;Y步进电机方向
M8004 I-1		;Z步进电机方向
M8005 I1		;E步进电机方向

;速度相关设置，速度以mm/s为单位，加速度以mm/s^2为单位

M8006	I100	        ;起步速度,当运动速度超过此速度时，以此速度为起始速度开始加速，否则以运动速度恒速运动
M8007	I30	        ;拐弯时的最大速度，在打填充时，会有大量往复运动，此值大，噪音大，值小，速度慢
M8008	I1500           ;加速度,该值越大，实际运行的平均速度越大，但是噪音也大，该值小，实际的速度也会越小，

;机器相关参数,参数设置完，请打印一个立方体，然后用尺子量一下尺寸，以确认参数没有问题
M8009  S0.0025;XY每一步的mm值，如20齿，齿距为2.032mm,1.8度步进电机，16细分，则为(20*2.032)/((360/1.8)*16)
;M8009 X0.0127 Y0.0127	;如果需要独立设置X,Y步进电机参数，可以用这个指令(去除本行;即可)
M8010  S0.00125	;Z每一步的mm值,计算公式:导程/((360/1.8)*16),导程为螺杆转一圈平台上升的高度
M8011  S0.00225		;E每一步的mm值,这个值等于挤出齿轮的周长除以3200，如果有减速装置还需除以减速比，如果你发现设置出丝比较稀，或是经常断层，建议将此值设置得比真实值小一点，出丝更好

;各种参数的速度最大值，为了保证机器能够稳定，请根据实测结果进行设置
M8012  I200		;XY运动的最大速度mm/s
M8013  I30		;Z运动的最大速度mm/s
M8014  I120		;挤出机的最大速度mm/s

;Z归零速度,makerware切片软件切片，会忽略这个归零速度，因为其gcode中有指定归零速度
M8015  I8		;Z归零时的第一次归零速度，速度较快
M8015  S30		;XY归零时的第一次归零速度，速度较快
M8016  I4		;Z归零时的第二次归零速度，速度较慢
M8016  S5		;XY归零时的第二次归零速度，速度较慢


;打印前的预挤出
M8017	I10		;预挤出长度mm，第一层与底板是否粘牢直接影响打印质量，多一点预挤出会使底板粘得更好
M8018	I20		;挤出机的最大预挤出速度mm/s,非减速齿轮送料，最大速度通常都在100以上，

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;下面的两个参数和与Replicator/Makerware软件兼容密切相关，如果不打算兼容该类软件，可以不用管
M8019	I50		;支持的最大退丝速度,单位是mm/s，当为0时，表示与E最大速度相同。默认Makerware双头切片退丝非常慢，退丝停顿时间非常长
M8020	S1.5	;退丝长度,mm，回抽距离，回抽过小会漏丝，回抽过大，停顿时间过长，也容易漏丝，请根据您的送料类型及挤出头类型合理设置，如果为0，退丝长度遵从切片软件
M8033	S8		;makerware软件切换挤出头时的退丝长度,makerware切片指定退丝非常长，再回挤时容易卡丝,如果为零，遵从切片软件设置
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;设置机器使用的默认耗材直径,单位是mm
M8021	S1.75		;耗材直径

;挤出头最高温度
M8022 I245		;挤出头支持的最高温度，设置此温度为了防止用户误操作挤出头温度造成挤出头损坏, 热阻只能到260度，peek管也大概在260度开始软化

;热床最高温度
M8023 I120		;热板最高温度，温度过高容易损坏热板

;X，Y，Z最大行程，请根据实际打印尺寸进行设置，单位是mm
;请务必认真设置此参数！！！！！！
M8024 I350		;X最大行程,在lcd使能声音的情况下，超出行程的运动会造成蜂鸣器鸣叫
M8025 I195		;Y最大行程
M8026 I192		;Z最大行程

;设置挤出头个数,最少1个，目前最多2个
M8027	I1		;配置挤出头个数

;第二个挤出头当成Z来使用，有些大机器需要两个Z轴，所以可以把E2当成Z轴来使用,该命令会强制挤出头个数为1
;M8027  Z0		;如果使能该功能，请将前面的';'去掉，要禁止该功能，用';'将其注释掉即可

;有些机器会希望禁止热床，可能通过该指令禁止热床
M8027  T1		;1：使能热床  ,0：禁止热床

;高级设置
M8028	S0.00	        ;退丝补偿，机器，退丝后再挤出，因为弹性作用，其实会比原位置退一点点，如果不懂就直接为零

;限位开关结构类型
;0:reprap机型，通常使用repetier-host和cura,限位开关位置在x,y的零点，限位接X-,Y-,Z-
;1:makerbot replicator类似机型，通常使用 replicatorG/makerware/makerbot desktop切片软件,限位位置在x,y的最大值处,限位接X+,Y+,Z-
;2:ultimaker机型，双边都带限位开关
M8029	I2		;0: 单边零点限位，如mendel,i3...    1:单边最大点限位，如makerbot机型    2：双边限位，如ultimaker机型	

;限位开关接线类型
M8029   T0		;0: 限位开关常开  	1:限位开关常闭

;Z轴限位开关位置
M8029	S0		;0：挤出头离平台最近时限位,限位接Z-	1：挤出头离平台最远时限位,限位接Z+

;如果挤出头是makerbot结构，挤出机一加热就需要散热，否则挤出机会过热，如果挤出头是ultimaker的结构，风扇是否需要开启则由切片软件指定
M8030	I0	        ;如果需要强制挤出机大于某温度时自动开启风扇，设置一个大于0的温度.如果设为0,风扇开头由切片软件控制

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;设置主板风扇自启动温度,一定要放在M8030 I0命令之后，makerbot主板上有两个风扇，有一侧面风扇高于某温度时会自开启，可以将风扇接主板风扇，并配置此参数
;M8030 I0 T-1		;I后面的参数表示温度，当为0是，表示主板风扇不会随温度变化，否则当挤出头温度达到指定温度时，风扇会开启

;设置第二个挤出头(右边挤出头)的偏移，单位是mm，下列两个参数只有在喷头设为2时，才生效
M8031	S35.0		;X方向的偏移
M8032	S0		;Y方向的偏移

;SD卡是否支持文件夹的显示
M8034	I1		;0:不支持 1：支持

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;调平相关设置,最多可以取5个点，X,Y为浮点数，如果X,Y都小于1，则表示相对行程的比例，否则表示为X,Y的绝对坐标值，单位为mm
M8035	I4		;调平点个数，
M8036	X104 Y5	;有多少个点，就有多少个该指令
;如果行程为(200,200),则M8036 X0.1 Y0.1与 M8036 X20 Y20是一样的效果
M8036	X296 Y5
M8036	X296 Y189
M8036	X104 Y189
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;机器类型
M8080 I0	        ;机器类型:0:XYZ普通类型，  2:Hbot类型
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;热阻类型
;对于热电阻，同样参数的热电阻其实也略有差别,不同的热电阻在低温时温度比较接近，在高温时，显示的温度差别可达40度，
如果是热敏电阻,请务必设置正确的热电阻类型
M8081 I0	;温度传感器类型:0: 理想NTC 100K 1% 3950 B  ,1:K型热电偶 ,2:EPCOS NTC 100K 1% 3950 B,对应marlin的传感器类型1 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;屏幕类型
M8082 I0	        ;LCD类型：0 ：2.8寸 , 1:3.5寸


M8085 I3000	        ;开机logo持续时间，最小100ms,最大6000ms

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;M8086	I1	;步进细分分频，默认16细分，如果需要8分频，设为2，如果是需要4分频，设为4,......,请不要轻易修改细分数!!!!!
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;温度的PID参数设置,如果你不是专家，并且你的当前温度控制比较稳定，可以不需要动这个参数
;M301 P22.2 I1.08 D114 

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;自动调制PID命令，这个命令会阻塞很长时间
;M303 E0 S150 C5	;温度150度，自动周期震荡4次，自动调制后的参数可能通过repetier-host看到调制后的pid参数
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


;保存参数，此参数一定不能少，否则参数无法保存到设备
M8500		;保存配置