# Qt_back_up_project_uestc
## 环境配置：
windows系统、visual studio 2019、C++ 17及以上、QT 5以上
## QT 安装:
安装教程：https://blog.csdn.net/m0_62919535/article/details/129340079  
注：只需要安装完QT即可，后续vs studio上的QT插件可选择安装(QT安装过程中选择组件里必须选择MSVC 2019 64-bit)
## 依赖库安装：
1. OpenSSL库：
安装地址：http://slproweb.com/products/Win32OpenSSL.html  
具体安装过程：https://blog.csdn.net/zyhse/article/details/108186278  
将OpenSSL-Win64文件夹加到该项目中
3. msvc2019_64库：
找到QT所在目录，将E:\Qt\6.6.1\msvc2019_64文件夹复制到该项目中
## 环境变量设置：
将 .../msvc2019_64/bin 放到系统环境变量中
## 代码运行：
用visual studio 2019 打开该cmakelist项目并运行
