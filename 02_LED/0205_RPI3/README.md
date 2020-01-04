# 0205_RPI3

RaspberryPi3 Model B におけるオンボードLチカ

RaspberryPi3 Model Bでは、ACT LEDがExpGPIO 2で、  
PWR LEDはExpGPIO 7で、それぞれ操作可能です。  
ExpGPIO 7は、初期状態では“入力”のため、そのままでは動作しません。  
そのため、ExpGPIO 7を“出力”設定変更する必要があります。

手順)  
・mailboxによりExpGPIO 7を“出力”設定に変更する  
・mailboxによりExpGPIO 2、ExpGPIO 7を一定間隔でON/OFFさせる
