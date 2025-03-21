
开发工具与设计流程
-------------

### 硬件部分

<div style="display: flex; flex-wrap: wrap; gap: 20px; justify-content: center; align-items: center; min-height: 100vh; padding: 20px;">
  <div style="flex: 0 1 440px; background: #fff; border-radius: 8px; box-shadow: 0 2px 12px rgba(0,0,0,0.1); 
              display: flex; flex-direction: column; justify-content: center; align-items: center;">
    <div style="padding: 12px; font-weight: 500; color: #333; text-align: center;">AD原理图	</div>
    <a href="assets/img/ad.png" target="_blank" 
       style="display: block; overflow: hidden; transition: transform 0.3s ease;"
       onmouseover="this.style.transform='scale(1.03)'" 
       onmouseout="this.style.transform='scale(1)'">
      <img src="assets/img/ad.png" 
           style="width: 100%; height: auto; max-width: 440px; object-fit: contain;">
    </a>
  </div>
  <div style="flex: 0 1 440px; background: #fff; border-radius: 8px; box-shadow: 0 2px 12px rgba(0,0,0,0.1);
              display: flex; flex-direction: column; justify-content: center; align-items: center;">
    <div style="padding: 12px; font-weight: 500; color: #333; text-align: center;">PCB</div>
    <a href="assets/img/pcb.png" target="_blank" 
       style="display: block; overflow: hidden; transition: transform 0.3s ease;"
       onmouseover="this.style.transform='scale(1.03)'" 
       onmouseout="this.style.transform='scale(1)'">
      <img src="assets/img/pcb.png" 
           style="width: 100%; height: auto; max-width: 440px; object-fit: contain;">
    </a>
  </div>
</div>

<div style="display: flex; flex-wrap: wrap; gap: 20px; justify-content: center; padding: 15px;">
  <div style="flex: 1 1 400px; max-width: calc(50% - 10px); min-width: 300px; margin: 10px; text-align: center;"
       onmouseover="this.style.transform='scale(1.02)'; this.style.boxShadow='0 10px 20px rgba(0,0,0,0.15)'" 
       onmouseout="this.style.transform='scale(1)'; this.style.boxShadow='0 2px 8px rgba(0,0,0,0.1)'"
       style="transition: all 0.3s ease; box-shadow: 0 2px 8px rgba(0,0,0,0.1); border-radius: 8px; overflow: hidden;">
    <div style="padding:12px; background:#f8f9fa; border-bottom:1px solid #eee; font-weight:500; color:#2d3436;">
      红外指令
    </div>
    <a href="assets/img/XDXK202208047_01900.jpg" target="_blank" style="display:block; overflow:hidden;">
      <img src="assets/img/XDXK202208047_01900.jpg" 
           style="width:100%; height:auto; max-width:440px; transition: transform 0.3s ease;"
           onmouseover="this.style.transform='scale(1.1)'" 
           onmouseout="this.style.transform='scale(1)'">
    </a>
  </div>
  <div style="flex: 1 1 400px; max-width: calc(50% - 10px); min-width: 300px; margin: 10px; text-align: center;"
       onmouseover="this.style.transform='scale(1.02)'; this.style.boxShadow='0 10px 20px rgba(0,0,0,0.15)'" 
       onmouseout="this.style.transform='scale(1)'; this.style.boxShadow='0 2px 8px rgba(0,0,0,0.1)'"
       style="transition: all 0.3s ease; box-shadow: 0 2px 8px rgba(0,0,0,0.1); border-radius: 8px; overflow: hidden;">
    <div style="padding:12px; background:#f8f9fa; border-bottom:1px solid #eee; font-weight:500; color:#2d3436;">
      Proteus虚拟仿真
    </div>
    <a href="assets/img/proteus.png" target="_blank" style="display:block; overflow:hidden;">
      <img src="assets/img/proteus.png" 
           style="width:100%; height:auto; max-width:440px; transition: transform 0.3s ease;"
           onmouseover="this.style.transform='scale(1.1)'" 
           onmouseout="this.style.transform='scale(1)'">
    </a>
  </div>
</div>



### 软件开发工具

本系统的软件设计主要使用以下工具：

1.  **Keil5**：用于编写和编译程序代码。
2.  **STC-ISP**：用于将编译好的程序下载到单片机中。
3.  **Proteus**：用于将编译好的程序进行仿真调试。

#### 安装和配置步骤

1.  **安装 Keil5**：确保安装的是 Keil5 C51 版本，因为这是专门为 51 单片机设计的开发环境。
2.  **安装 STC-ISP**：该软件用于将编译后的程序下载到单片机中。
3.  **其他工具（可选）**：如有道词典和福昕阅读器等辅助工具。

### 软件设计流程

系统的软件设计可以分为以下几个步骤：

1.  **系统初始化**：上电后执行 main 函数，进行必要的初始化操作，包括 I/O 端口、定时器、中断和显示模块的初始化。
2.  **主循环设计**：主循环采用 `switch` 语句，根据不同的运行状态（如自动模式、手动模式、自然风模式）执行相应的功能模块。
3.  **中断服务程序**：使用定时器 0、定时器 1 和外部中断 0 来处理不同的任务，如计时、红外信号捕捉和温度测量。

### 程序模块设计

#### 主程序模块

主程序模块包含三种模式：

1.  **自动模式**：根据温度传感器采集的温度值自动调节风扇转速。
2.  **手动模式**：通过按键或红外遥控手动调节风扇转速。
3.  **自然风模式**：模拟自然风的变化，使风扇转速随机变化。

#### 中断服务程序

中断服务程序包含三个中断：

1.  **定时器 0 中断**：每 50 ms 触发一次，用于直流电机的占空比来调节转速。
2.  **定时器 1 中断**：用于测量红外遥控信号的低电平持续时间，记录有效的数据。
3.  **外部中断 0**：用于捕捉红外遥控信号的下降沿，启动定时器 1。

系统的中断资源使用了定时器 0 中断、定时器 1 中断、外部中断 0。定时器 0 中断每 50 ms 触发一次，根据当前温度，当计时位累计达到一定值时将更新一次电机占空比。外部中断 0 用于捕捉红外遥控信号的下降沿，并打开定时器 1 计算红外遥控信号的低电平持续时间，如果检测到有效的引导码，则开始记录接下来的低电平时长数据存储在 `irdata[]` 中。

#### 子模块设计

包含了以下模块：

1.  **温度采集模块**：使用 DS18B20 温度传感器采集当前温度，并与设定值进行比较。
2.  **显示模块**：使用 LCD1602 显示当前温度、风扇档位和工作模式。
3.  **按键和红外遥控模块**：用于手动控制风扇和切换工作模式。
4.  **电机控制模块**：使用 PWM 信号调节风扇转速。