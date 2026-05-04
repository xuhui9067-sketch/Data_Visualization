import socket
import struct
import time
import math
import random

# 发送数据
def send_modbus_float():

    # 设置服务器的地址和端口，localhost表示本机，8888 是端口号
    server_address = ('localhost', 8888)

    # 创建一个 TCP socket，AF_INET表示 IPv4 地址，SOCK_STREAM 表示面向连接的 TCP 协议
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # 连接到上面的服务端地址
    sock.connect(server_address)

    print("模拟器1已启动...")

    # Modbus TCP 报文头里的“事务标识符”，通常每个请求/响应对会递增
    transaction_id = 0

    # 时间变量，用于计算正弦波的相位
    t = 0.0

    # 100Hz 采样间隔
    dt = 0.01

    # 正弦波振幅
    amplitude = 5.0

    # 正弦波频率 (Hz)
    frequency = 1.0

    # 正弦波偏移量
    offset = 25.0

    # 高斯噪声的标准差
    noise_std = 0.2

    try:
        # 无限循环，模拟传感器持续不断发送数据
        while True:

            # 生成理想正弦波值
            ideal = offset + amplitude * math.sin(2 * math.pi * frequency * (t+1))

            # 添加随机噪声
            noise = random.gauss(0, noise_std)

            # 最终的模拟传感器读数
            val = ideal + noise

            # 将浮点数 val 打包成 4 个字节的二进制数据，'>f' 表示大端字节序
            float_bytes = struct.pack('>f', val)

            # 构建 Modbus TCP 的 MBAP 报文头
            mbap = struct.pack('>HHHB', transaction_id, 0, 6 + 1, 1)

            # 构建 Modbus PDU（协议数据单元）
            pdu = struct.pack('>BB', 0x03, 4) + float_bytes

            # 将 MBAP 头和 PDU 拼接成完整的 Modbus TCP 报文
            packet = mbap + pdu

            # 通过 socket 发送整个报文到服务端
            sock.sendall(packet)

            # 事务标识符递增
            transaction_id = (transaction_id + 1) % 65535

            # 让程序暂停 dt 秒，以达到 100Hz 的发送频率
            time.sleep(dt)

            # 将时间变量 t 增加 dt，用于下一次循环计算正弦波的相位
            t += dt

    finally:
        # 关闭 socket 连接
        sock.close()

if __name__ == '__main__':
    send_modbus_float()