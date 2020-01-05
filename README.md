# RaspberryPiでBaremetal

Baremetalのための試行錯誤の履歴

01_boot  
まず起動できないと話にならないので、startup.Sを作成します。  
(実験のため、GPIO16～19を使用します)

02_LED  
各モデルのオンボードLED(PWR,ACT)を使えるようにします。

03_UART  
RaspberryPi3以前ではUART0～UART1を、RaspberryPi4では加えてUART2～UART5を使えるようにします。
