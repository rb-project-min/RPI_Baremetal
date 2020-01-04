# 0202_RPI0

RaspberryPi Zero/Zero W  
ComputeModule におけるオンボードLチカ

RaspberryPi Zero/Zero W/ComputeModuleでは、ACT LEDがGPIO47で操作可能ですが、  
PWR LEDはそもそも存在しないため、操作できません。  
ここでは、とりあえずACT LEDのみでLチカを行います。

手順)  
・GPIO47を出力に設定する  
・GPIO47を一定間隔でON/OFFさせる
