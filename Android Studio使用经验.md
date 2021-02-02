## 安装Android Studio
包含Android-SDK和Android-ndk安装，是伴随Android Studio IDE安装过程重新下载的，Android-ndk是r22版本。

## 安装Gradle
[Gradle][0]是一套基于java实现的编译工具，下载解压后，通过配置Android Studio使用，具体方式我没记住，下次有机会再安装时补充这部分。

## 完成一个基于JNI的安装工程的主要步骤
### 创建
菜单栏->File->New->NewProject->Phone and Table页签->Native C++
### [配置gradle]可能会用到
Android Studio IDE缺少ndk.dir路径，IDE默认的r20ndk找不到，编辑Gradle Scripts的local.properties可以加一行
`ndk.dir=C\:\\Users\\eacbpd\\AppData\\Local\\Android\\Sdk\\ndk\\22.0.7026061`
### 编辑资源管理器中的相关源码
app/java/com.xxxxxx/MainActivity 修改java源码，加载不同的原生库(.lib .dll .so) 声明和调用其中的方法

app/cpp目录的Native Code以及CMakeLists.txt
### 利用Gradle编译
Gradle Scripts的多个build.gradle上鼠标右键"Synchronize 'xxx'"
### 调试运行app
* Shift+F9调试    调试的提示效果有点怪？
* Shift+F10运行
## 访问安装模拟设备(AVD)的存储器
"工具栏/View/Tool Windows/Device File Pxplorer" 选择模拟器在找到对应的文件夹upload即可


[0]: https://gradle.org/releases/ "gradle下载地址 本文基于gradle-6.5.1 Download: binary-only"