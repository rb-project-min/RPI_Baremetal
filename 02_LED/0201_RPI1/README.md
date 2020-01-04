# 0201_RPI1

RaspberryPi Model A/BにおけるオンボードLチカ

RaspberryPi Model A/Bでは、ACT LEDがGPIO16で操作可能ですが、  
PWR LEDは常時ONとなっているため操作できません。  
ここでは、とりあえずACT LEDのみでLチカを行います。

手順)  
・GPIO16を出力に設定する  
・GPIO16を一定間隔でON/OFFさせる
