# 0207_RPI4

RaspberryPi4 Model B におけるオンボードLチカ

RaspberryPi4 Model B では、ACT LEDがGPIO42で、  
PWR LEDはExpGPIO 2で、それぞれ操作可能です。  

手順)  
・GPIO42を出力に設定する  
・GPIO42を一定間隔でON/OFFさせる
・mailboxによりExpGPIO 2を一定間隔でON/OFFさせる
