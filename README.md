# AAC_GetADTSFrame
AAC码流解析,从AAC码流中分析得到它的基本单元ADTS frame

音频码流在视频播放器中的位置如下所示

![image text](https://github.com/MTerence/AAC_GetADTSFrame/blob/master/20160118101456758.png)

这个小Demo是一个AAC码流解析程序，该程序可以从AAC码流中分析得到它的基本单元ADTS frame，并且可以简单解析ADTS frame 首部的字段，通过该程序可以实现不同的AAC码流处理功能

#原理

AAC原始码流（又称为裸流）是由一个一个的ADTS Frame 组成的，他们的结构如下：

