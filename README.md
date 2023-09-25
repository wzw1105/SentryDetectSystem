## 文件结构
```bash
├─ CMakeLists.txt
├─ robot_bringup            # launch 文件功能包
├─ robot_msgs
├─ sentry_beta              # 哨岗检测节点
│  ├─ CMakeLists.txt
│  ├─ model                 # Tensorrt 模型
│  ├─ background.png        # 场地背景图
│  ├─ libtensorrt_pro.so    # tensorRT_pro 静态库（仓库中没有，需要编译，见README）
│  ├─ sentry_node.cpp       # 哨岗检测节点主程序
│  ├─ sort                  # 哨岗目标检测与目标跟踪代码库
│  │  ├─ Hungarian.cpp      # 匈牙利算法
│  │  ├─ Hungarian.h
│  │  ├─ pv_kalman_filter.cpp # 二维卡尔曼滤波器Eigen实现
│  │  ├─ pv_kalman_filter.h
│  │  ├─ rsort.cpp          # 目标跟踪算法主程序
│  │  ├─ rsort.h
│  │  ├─ trrt_detector.cpp  # tensort推理 后处理 输出格式转换
│  │  ├─ trrt_detector.h
│  │  ├─ visualize.cpp      # 可视化函数
│  │  └─ visualize.h
│  └─ tensorrt              # tensorRT_pro项目的头文件
└─ tools                    # 模块用到的其他工具
   ├─ homo_mat              # Master Slave对应的相机外参单应变换矩阵
   └─ homography.cpp        # 哨岗相机外参标定模块

```

## 项目介绍
![image](images/幻灯片1.PNG)

![image](images/幻灯片2.PNG)

![image](images/幻灯片3.PNG)

![image](images/幻灯片4.PNG)

![image](images/幻灯片5.PNG)

![image](images/幻灯片6.PNG)

![image](images/幻灯片7.PNG)

![image](images/幻灯片8.PNG)

![image](images/幻灯片9.PNG)

![image](images/幻灯片10.PNG)

![image](images/幻灯片11.PNG)

![image](images/幻灯片12.PNG)

![image](images/幻灯片13.PNG)

![image](images/幻灯片14.PNG)

![image](images/幻灯片15.PNG)

![image](images/幻灯片16.PNG)

![image](images/幻灯片17.PNG)
