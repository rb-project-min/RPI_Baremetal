# 0206_RPI3plus

RaspberryPi3 Model A+/Model B+ におけるオンボードLチカ

RaspberryPi3 Model A+/Model B+ では、ACT LEDがGPIO29で、  
PWR LEDはExpGPIO 2で、それぞれ操作可能です。  

手順)  
・GPIO29を出力に設定する  
・GPIO29を一定間隔でON/OFFさせる
・mailboxによりExpGPIO 2を一定間隔でON/OFFさせる
