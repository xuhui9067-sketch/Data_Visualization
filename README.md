# Data_Visualization
High-frequency (1kHz+) sensor data acquisition system. The system receives Modbus protocol data from the underlying hardware via TCP Socket, performs real-time filtering processing, and then smoothly plots it on the QML front-end interface. Meanwhile, it persistently stores the logs at high speed to the local database.

├── main.cpp              # 程序入口，负责模块初始化与线程分发
├── datareceiver.cpp      # TCP 数据接收模块，负责原始协议解析
├── dataprocessor.cpp     # 数据处理中心，包含滤波算法与多缓冲管理
├── databasemanager.cpp   # 数据库管理，处理高频数据存盘
├── customplotitem.cpp    # 桥接 QCustomPlot 与 QML 的绘图组件
├── Main.qml              # 界面展示逻辑
└── mock_sensor.py        # 测试脚本，模拟高频传感器发送数据
