# 0208_All

RaspberryPi全モデルでオンボードLチカ

RaspberryPiのリビジョンIDを取得し、分岐することで  
すべてのモデルでオンボードLチカができるようにする。  
ついでに、ここまでの内容をGPIO/mailbox/LEDに分けました。

手順)  
・mailboxにてリビジョンIDを取得し、GPIOの初期設定を行う  
・GPIOを一定間隔でON/OFFさせる
