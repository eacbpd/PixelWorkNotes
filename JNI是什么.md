# JNI是什么 The Java Native Interface (JNI)
[翻译自原文][0]

从JVM的角度来看，有两种类型的代码:“Java™”和“本机”。Java本机接口(JNI)在两者之间建立了一个定义良好且独立于平台的接口。
本机代码可以以两种截然不同的方式与Java一起使用:作为运行中的JVM中的“本机方法”，以及作为使用“调用API”创建JVM的代码。本节描述两者的区别。

## Native methods 本地方法
Java本机方法用Java声明，用另一种语言(如C或c++)实现，并在必要时由JVM加载。要使用本机方法，必须:
### 1.在Java代码中声明本机方法。Declare 
当javac编译器在Java源代码中遇到本机方法声明时，它会记录方法的名称和参数。因为Java源代码不包含实现，所以编译器将该方法标记为“本机”。然后，JVM可以在调用该方法时正确解析该方法。
### 2.实现本机方法。Implement 
本机方法是作为可加载二进制库中的外部入口点实现的。本机库的内容是特定于平台的。JNI为JVM提供了一种以独立于平台的方式使用任何本机方法的方法。
JVM对本地方法执行调用。当JVM处于本机方法中时，JNI提供了一种“回调”JVM的方法。
### 3.加载VM要使用的本机方法代码。Load
除了声明本机方法外，还必须在运行时找到并加载包含该方法的本机库。
两个Java接口加载本地库:
```
 java.lang.System.load ()
 java.lang.System.loadLibrary ()
```
通常，声明了本机方法的类会在其静态初始化器中加载本机库。

## Invocation API 调用API
创建JVM涉及到本地代码。用于此目的的JNI方面称为JNI调用API。要使用调用API，需要静态或动态地绑定到特定于实现的共享库，并调用它导出的JNI_*函数。
## The JNI specification and implementation JNI规范和实现
JNI规范对选定的实现细节含糊不清。它为简单和可扩展的C和c++本机接口提供了一个可重用的框架。JNI模型也是JVMTI规范的基础。
Oracle公司的商标规范和Java兼容性工具包(Java Compatibility Kit, JCK)确保符合规范，但不符合实现。本机代码必须符合规范，而不是符合实现。针对未指定行为编写的代码容易出现可移植性和前向兼容性问题。

[0]: https://www.ibm.com/support/knowledgecenter/en/SSYKE2_8.0.0/com.ibm.java.vm.80.doc/docs/jni_overview.html "IBM关于JDI的介绍"