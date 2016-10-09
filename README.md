# 多边形处理作业
软件32 王昭 2013013330
***

### 一、实验环境
* OS：Windows 10
* CPU：Intel Core i5-3210M @ 2.50Ghz × 4
* Memory：8GB
* Visual Studio：2012
* Qt：5.4.0

### 二、综述

在本次实验中，我以Qt界面库为载体实现了一系列关于多边形操作处理的程序。工程中实现了三个类：

* UIWidget：画布，多边形的绘制及与用户的交互主要以该类为依托。其中也包括了对多边形z-index的存储；
* Polygon：多边形，实现了填充以及裁剪算法，其中顶点以单精度浮点数的二元组列表来存储，存储方法为：封闭曲线之间以（FLT_MAX,FLT_MAX）隔开，封闭曲线本身的顶点以外环逆时针，内环顺时针的顺序存储顶点；
* Utils：功能函数类，该类中只包括静态方法，供工程中其他部分使用。

### 三、使用方法

* 绘制多边形：在画布上依次点击左键以确定多边形的顶点，点击右键将自动封闭多边形。注意：不要输入自交或其他不合法的多边形。
* 选中多边形：鼠标滑过多边形上一点，即可选中之，选中多边形将会使该多边形进入高亮状态从而易于辨识。或者也可以在右方的多边形列表中选取某个多边形，鼠标滑过列表中某个item时，其对应的多边形会进入高亮状态，点击左键即可选中之。
* 添加内环：选中某个多边形后，点击上方工具栏中“进入内环模式”后，即可输入内环，输入方式与输入普通多边形相同。注意：不要输入非法的内环。
* 填充：选中某个多边形后，点击上方工具栏中“填充多边形”，即可对该多边形进行填充。
* 平移：选中某个多边形后，在画布中按住鼠标中键并移动，即可移动该多边形。
* 缩放：选中某个多边形后，在画布中滚动鼠标滚轮，即可放大/缩小该多边形。
* 旋转：选中某个多边形后，在画布中按住鼠标右键并移动，即可旋转该多边形。注意：右键点击位置距离多边形中心远些效果较好，否则旋转会太快。
* 翻转：选中某个多边形后，点击上方工具栏中“水平翻转”/“竖直翻转”，即可对该多边形进行对应操作。
* 裁剪：选中某个多边形后，点击上方工具栏中“裁剪”后，在右方的多边形列表中再选中一个多边形，即可对这两个多边形进行裁剪操作。

### 四、实现方法

* 绘制线段：使用Bresenham算法。
* 计算线段交点：根据输入的四个点，手动分别计算两条直线方程，最终算出交点。
* 填充：使用扫描线算法，所以会导致当多边形的一部分平移出画布时，多边形内部会出现变空的情况。
* 判断点的序列是否为顺时针：使用Shoelace formula来判断。
* 平移、缩放、旋转、翻转：对多边形中的每个点都乘以对应的矩阵来实现坐标的变换。由于多边形的顶点是由浮点数来存储，所以多次缩放、旋转并不会使得多边形有形状上的变化。
* 裁剪：使用Weiler-Atherton算法，步骤与课件上的介绍相同。其中，算法中两个列表由list来存储，list中的每个交点都会存储在另外一个list中迭代器的位置，以模拟指针的效果，之后就按照算法所述遍历列表即可。
* z-index：每个多边形都具有与其编号相同的z-index值，同时画布会维护一个z-index的map，保证只显示z-index最高的颜色。

### 五、程序效果

* 裁剪

![填充1](/img/1.png)

![填充2](/img/2.png)

* 填充

![填充1](/img/3.png)

![填充2](/img/4.png)

### 六、存在的问题

* 裁剪算法的局限性：如果一个多边形完全属于另外一个多边形，则裁剪结果为空，这是因为它们之间没有任何交点。这种情况我采取了特判，但如果一个多边形的内环完全属于另外一个多边形，则裁剪结果将不会包括这个内环，这也是这个算法的一个问题。
* 交互方式还不够好：输入多边形时没有进行非法判定，需要用户自律地输入。

### 七、代码管理

代码托管在github上，地址：https://github.com/nero19960329/polygons