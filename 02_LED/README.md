# 02_LED

オンボードLチカ

RaspberryPiはモデルによってオンボードLEDの操作方法が異なります。  
それぞれの操作方法をまとめて、LED操作のライブラリを作ることを目的とします。

0201_RPI1  
RaspberryPi Model A/Model B におけるオンボードLチカ

0202_RPI0  
RaspberryPi Zero/Zero W  
ComputeModule におけるオンボードLチカ

0203_RPI2  
RaspberryPi Model A+/Model B+  
RaspberryPi2 Model B におけるオンボードLチカ

0204_CM3  
ComputeModule 3/3+ におけるオンボードLチカ

0205_RPI3  
RaspberryPi3 Model B におけるオンボードLチカ

0206_RPI3plus  
RaspberryPi3 Model A+/Model B+ におけるオンボードLチカ

0207_RPI4  
RaspberryPi4 Model B におけるオンボードLチカ

具体的にまとめると以下の通り

|Model                |ACT LED  |PWR LED  |
|---------------------|:-------:|:-------:|
|RaspberryPi Model A  |GPIO16   |常時ON   |
|RaspberryPi Model B  |GPIO16   |常時ON   |
|RaspberryPi Model A+ |GPIO47   |GPIO35   |
|RaspberryPi Model B+ |GPIO47   |GPIO35   |
|RaspberryPi2 Model B |GPIO47   |GPIO35   |
|Compute Module       |GPIO47   |なし     |
|RaspberryPi Zero     |GPIO47   |なし     |
|RaspberryPi Zero W   |GPIO47   |なし     |
|RaspberryPi3 Model B |ExpGPIO 2|ExpGPIO 7|
|Compute Module 3     |ExpGPIO 2|なし     |
|RaspberryPi3 Model A+|GPIO29   |ExpGPIO 2|
|RaspberryPi3 Model B+|GPIO29   |ExpGPIO 2|
|Compute Module 3+    |ExpGPIO 2|なし     |
|RaspberryPi4 Model B |GPIO42   |ExpGPIO 2|

