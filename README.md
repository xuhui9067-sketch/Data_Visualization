# Data_Visualization
模拟工业场景下的高频（1kHz+）传感器数据采集系统。系统通过TCP Socket接收底层硬件的Modbus协议数据，进行实时滤波处理后，在QML前端界面进行平滑绘制，并将日志高速持久化至本地数据库。

main.cpp              # 程序入口，负责模块初始化与线程分发
datareceiver.cpp      # TCP 数据接收模块，负责原始协议解析
dataprocessor.cpp     # 数据处理中心，包含滤波算法与多缓冲管理
databasemanager.cpp   # 数据库管理，处理高频数据存盘
customplotitem.cpp    # 桥接 QCustomPlot 与 QML 的绘图组件
Main.qml              # 界面展示逻辑
mock_sensor.py        # 测试脚本，模拟高频传感器发送数据
