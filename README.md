# 前言

参考资料：

&nbsp;&nbsp;&nbsp;&nbsp;STM32F1开发指南(精英版)-库函数版本_V1.2.pdf

&nbsp;&nbsp;&nbsp;&nbsp;ATK-ESP8266 WIFI用户手册_V1.2.pdf

&nbsp;&nbsp;&nbsp;&nbsp;ATK_ESP8266V1.2.pdf

&nbsp;&nbsp;&nbsp;&nbsp;ATK-ESP8266 WIFI模块使用说明(战舰V3&精英版)_AN1509A.pdf

源码参考：正点原子 （库函数版本，适合战舰V3和精英STM32开发板）扩展实验13 ATK-ESP8266 WIFI模块测试实验) 这个例程功能比较全，也很强大，我对这个例程进行了精简和删改，LCD显示也注释了。

开发板：正点原子 STM32F103 精英版

语言：C语言

开发环境：Keil5

手机：准备的软件是 网络调试助手 或 网络测试

![在这里插入图片描述](https://img-blog.csdnimg.cn/20210427135349145.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0lrYXJvc181MjE=,size_16,color_FFFFFF,t_70)



**开发板**使用了 LED KEY  TFTLCD USART SPI ESP8266模块 

## ESP8266模块 相关资料

更多参考底部

![在这里插入图片描述](https://img-blog.csdnimg.cn/20210427112338377.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0lrYXJvc181MjE=,size_16,color_FFFFFF,t_70)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20210427113040612.png)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20210427113050134.png)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20210427113107917.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0lrYXJvc181MjE=,size_16,color_FFFFFF,t_70)

## 代码下载：

[码云](https://gitee.com/ikaros-521/STM32_ESP8266_AP_mode_TCP_UDP) [GitHub](https://github.com/Ikaros-521/STM32_ESP8266_AP_mode_TCP_UDP)

## 功能介绍：

复位1秒后，进入`ap_demo()`函数，默认配置为AP模式，TCP 服务端，端口为8086。KEY0按下发送测试数据，KEY_UP按下结束测试。期间会有串口打印，最好复位后就把串口打开看信息。

**main.c**中可以设置网络模式和IP地址。

```c
u8 netpro = 0;	//网络模式 0,TCP服务器 1,TCP客户端 2,UDP模式
u8 ipbuf[16] = "192.168.4.2"; 	//IP (根据你的设备连上模块后分配到的IP填写）
```

**common.c**中可以设置 端口 wifi信息等


```c
//连接端口号:8086,可自行修改为其他端口.
const u8* portnum="8086";		 

//WIFI STA模式,设置要去连接的路由器无线参数,请根据你自己的路由器设置,自行修改.
const u8* wifista_ssid="wifi_name";			//路由器SSID号
const u8* wifista_encryption="wpawpa2_aes";	//wpa/wpa2 aes加密方式
const u8* wifista_password="password"; 	//连接密码

//WIFI AP模式,模块对外的无线参数,可自行修改.
const u8* wifiap_ssid="ATK-ESP8266";			//对外SSID号
const u8* wifiap_encryption="wpawpa2_aes";	//wpa/wpa2 aes加密方式
const u8* wifiap_password="12345678"; 		//连接密码 
```

### 拓展应用

#### 功能介绍：

AP STA STA+AP 模式下，建立tcp/udp连接后，发送指定命令，控制LED1和蜂鸣器的开关。

命令如下：（每行都是一条命令，不要有换行等）具体实现在`common.c`中

```c
LED1 ON
LED1 OFF
BEEP ON
BEEP OFF
GET T&H
```

参考文章：[STM32F103+ESP8266（WiFi）模块 简单应用之远程控制LED和蜂鸣器、读取温湿度](https://blog.csdn.net/Ikaros_521/article/details/116232001)

# 接线

## 接ATK MODULE

![在这里插入图片描述](https://img-blog.csdnimg.cn/20210427134246594.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0lrYXJvc181MjE=,size_16,color_FFFFFF,t_70)

![在这里插入图片描述](https://img-blog.csdnimg.cn/2021042711271067.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0lrYXJvc181MjE=,size_16,color_FFFFFF,t_70)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20210427112721787.png)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20210427144443182.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0lrYXJvc181MjE=,size_16,color_FFFFFF,t_70)

## 没有ATK MODULE情况

```c
VCC   -》  5V
GND   -》  GND
TXD   -》  PB11
RXD   -》  PB10
RST   -》  PA4
IO_0  -》  PA15
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/20210427133959913.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0lrYXJvc181MjE=,size_16,color_FFFFFF,t_70)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20210427134415650.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0lrYXJvc181MjE=,size_16,color_FFFFFF,t_70)

# 效果图

## TCP 服务端

```c
u8 netpro = 0;	//main.c ap_demo() 网络模式 0,TCP服务器 1,TCP客户端 2,UDP模式
```

烧写完后，复位，打开串口调试工具。

![在这里插入图片描述](https://img-blog.csdnimg.cn/202104271349002.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0lrYXJvc181MjE=,size_16,color_FFFFFF,t_70)

可以从上图中看到，wifi的具体信息

```c
IP地址:192.168.4.1 端口:8086
请用设备连接WIFI热点:SSID:ATK-ESP8266
加密方式:wpawpa2_aes
密码:12345678
状态:TCP服务器
```

根据上面的信息连接wifi（ATK-ESP8266），连接成功后查看详细信息

![在这里插入图片描述](https://img-blog.csdnimg.cn/20210427140604627.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0lrYXJvc181MjE=,size_16,color_FFFFFF,t_70)

### 网络调试助手

打开软件，选择顶部“tcp client”，点击“增加”，输入上面的IP地址和端口，进行“增加”

![在这里插入图片描述](https://img-blog.csdnimg.cn/2021042714204429.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0lrYXJvc181MjE=,size_16,color_FFFFFF,t_70)

提示“连接成功”，并且左上角出现 小块显示增加的连接

![在这里插入图片描述](https://img-blog.csdnimg.cn/20210427142119400.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0lrYXJvc181MjE=,size_16,color_FFFFFF,t_70)

此时串口收到连接信息，连接成功

![在这里插入图片描述](https://img-blog.csdnimg.cn/20210427142332614.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0lrYXJvc181MjE=,size_16,color_FFFFFF,t_70)

手机编辑文本“123321”，点击发送，STM32按下KEY0发送测试数据，顶部收到数据。最后长按连接块，“断开”连接。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20210427142544926.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0lrYXJvc181MjE=,size_16,color_FFFFFF,t_70)

串口打印如下

![在这里插入图片描述](https://img-blog.csdnimg.cn/20210427142932751.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0lrYXJvc181MjE=,size_16,color_FFFFFF,t_70)

### 网络测试

打开手机软件“网络测试”，选择TCP

![在这里插入图片描述](https://img-blog.csdnimg.cn/20210427140821883.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0lrYXJvc181MjE=,size_16,color_FFFFFF,t_70)

右上角 “创建连接”，填入上面串口打印的**IP:端口** ，点击“确定”

![在这里插入图片描述](https://img-blog.csdnimg.cn/20210427140914916.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0lrYXJvc181MjE=,size_16,color_FFFFFF,t_70)

右上角显示“已连接”就是连上了，然后我们发送“好家伙”，再STM32按KEY0（发送测试数据）

![在这里插入图片描述](https://img-blog.csdnimg.cn/20210427141306572.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0lrYXJvc181MjE=,size_16,color_FFFFFF,t_70)

最后按右上角“已连接”，断开连接

![在这里插入图片描述](https://img-blog.csdnimg.cn/20210427141448359.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0lrYXJvc181MjE=,size_16,color_FFFFFF,t_70)

串口助手收到的数据如下

![在这里插入图片描述](https://img-blog.csdnimg.cn/20210427140359764.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0lrYXJvc181MjE=,size_16,color_FFFFFF,t_70)

## 双管齐下

![在这里插入图片描述](https://img-blog.csdnimg.cn/20210427143409406.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0lrYXJvc181MjE=,size_16,color_FFFFFF,t_70)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20210427143706412.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0lrYXJvc181MjE=,size_16,color_FFFFFF,t_70)

## TCP 客户端

```c
u8 netpro = 1;	//main.c ap_demo() 网络模式 0,TCP服务器 1,TCP客户端 2,UDP模式
u8 ipbuf[16] = "192.168.4.2"; 	//IP (根据你的设备连上模块后分配到的IP填写）
```

手机创建TCP 服务端 监听8086端口，等待客户端主动连接

![在这里插入图片描述](https://img-blog.csdnimg.cn/20210427151427700.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0lrYXJvc181MjE=,size_16,color_FFFFFF,t_70)

注意下时间线0.0

![在这里插入图片描述](https://img-blog.csdnimg.cn/20210427145741923.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0lrYXJvc181MjE=,size_16,color_FFFFFF,t_70)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20210427150035364.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0lrYXJvc181MjE=,size_16,color_FFFFFF,t_70)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20210427150257353.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0lrYXJvc181MjE=,size_16,color_FFFFFF,t_70)

## UDP 客户端

```c
u8 netpro = 2;	//main.c ap_demo() 网络模式 0,TCP服务器 1,TCP客户端 2,UDP模式
u8 ipbuf[16] = "192.168.4.2"; 	//IP (根据你的设备连上模块后分配到的IP填写）
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/20210427151023904.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0lrYXJvc181MjE=,size_16,color_FFFFFF,t_70)

手机创建UDP server，监听8086端口

![在这里插入图片描述](https://img-blog.csdnimg.cn/20210427151619570.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0lrYXJvc181MjE=,size_16,color_FFFFFF,t_70)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20210427150827276.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0lrYXJvc181MjE=,size_16,color_FFFFFF,t_70)

按下KEY0，主动发送数据

![在这里插入图片描述](https://img-blog.csdnimg.cn/20210427151224429.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0lrYXJvc181MjE=,size_16,color_FFFFFF,t_70)

手机连接建立，点击进入

![在这里插入图片描述](https://img-blog.csdnimg.cn/2021042715091220.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0lrYXJvc181MjE=,size_16,color_FFFFFF,t_70)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20210427151315159.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0lrYXJvc181MjE=,size_16,color_FFFFFF,t_70)

# 参考用图

![在这里插入图片描述](https://img-blog.csdnimg.cn/20210427144032246.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0lrYXJvc181MjE=,size_16,color_FFFFFF,t_70)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20210427144221322.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0lrYXJvc181MjE=,size_16,color_FFFFFF,t_70)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20210427144338457.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0lrYXJvc181MjE=,size_16,color_FFFFFF,t_70)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20210427144139223.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0lrYXJvc181MjE=,size_16,color_FFFFFF,t_70)

# 拓展

## STA模式

### TCP服务端

准备2个手机，或者能产生wifi信号的设备。

一个手机开启手机热点，设置wifi信息

![在这里插入图片描述](https://img-blog.csdnimg.cn/20210427155208711.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0lrYXJvc181MjE=,size_16,color_FFFFFF,t_70)

另一个手机连接热点，查看详细信息，分配到的ip是 `192.168.43.39`

![在这里插入图片描述](https://img-blog.csdnimg.cn/20210427155319465.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0lrYXJvc181MjE=,size_16,color_FFFFFF,t_70)

修改`main.c`，调用 `atk_8266_test();`

```c
atk_8266_test();		//进入ATK_ESP8266测试
// ap_demo();
```

修改`common.c`，填写新的STA模式的wifi信息

```c
//连接端口号:8086,可自行修改为其他端口.
const u8* portnum="8086";		 

//WIFI STA模式,设置要去连接的路由器无线参数,请根据你自己的路由器设置,自行修改.
const u8* wifista_ssid="ikaros";			//路由器SSID号
const u8* wifista_encryption="wpawpa2_aes";	//wpa/wpa2 aes加密方式
const u8* wifista_password="12345678"; 	//连接密码

//WIFI AP模式,模块对外的无线参数,可自行修改.
const u8* wifiap_ssid="ATK-ESP8266";			//对外SSID号
const u8* wifiap_encryption="wpawpa2_aes";	//wpa/wpa2 aes加密方式
const u8* wifiap_password="12345678"; 		//连接密码 
```

修改 `wifista.c` 的 `atk_8266_wifista_test(void)` ，IP改成另一个手机的IP（这里给客户端模式用的，暂时没用）

```c
u8 ipbuf[16] = "192.168.43.39"; 	// IP缓存
```

程序重编，烧写，打开串口，看到提示按键

![在这里插入图片描述](https://img-blog.csdnimg.cn/20210427155055894.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0lrYXJvc181MjE=,size_16,color_FFFFFF,t_70)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20210427155843852.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0lrYXJvc181MjE=,size_16,color_FFFFFF,t_70)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20210427160021589.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0lrYXJvc181MjE=,size_16,color_FFFFFF,t_70)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20210427160147168.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0lrYXJvc181MjE=,size_16,color_FFFFFF,t_70)

### TCP客户端

这里就用到上面说的ip了

修改 `wifista.c` 的 `atk_8266_wifista_test(void)` ，IP改成另一个手机的IP（这里给客户端模式用的，暂时没用）

```c
u8 ipbuf[16] = "192.168.43.39"; 	// IP缓存
```

不重复赘述，直接上结果

![在这里插入图片描述](https://img-blog.csdnimg.cn/20210427160658953.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0lrYXJvc181MjE=,size_16,color_FFFFFF,t_70)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20210427160909584.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0lrYXJvc181MjE=,size_16,color_FFFFFF,t_70)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20210427161050421.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0lrYXJvc181MjE=,size_16,color_FFFFFF,t_70)

![在这里插入图片描述](https://img-blog.csdnimg.cn/2021042716113513.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0lrYXJvc181MjE=,size_16,color_FFFFFF,t_70)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20210427161152967.png)
