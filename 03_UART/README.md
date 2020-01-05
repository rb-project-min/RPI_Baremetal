# 03_UART

UARTによるシリアルコンソール

RaspberryPi3以前ではUART0～UART1を、RaspberryPi4では加えてUART2～UART5を使えるようにします。

0301_UART1  
UART1を使用してシリアルコンソール

0302_UART0  
UART0を使用してシリアルコンソール

0303_UART2  
UART2を使用してシリアルコンソール(RaspberryPi4専用)

0304_UART3  
UART3を使用してシリアルコンソール(RaspberryPi4専用)

0305_UART4  
UART4を使用してシリアルコンソール(RaspberryPi4専用)

0306_UART5  
UART5を使用してシリアルコンソール(RaspberryPi4専用)

0307_UART  
UART0～5を使用してシリアルコンソール(UART2～5はRaspberryPi4専用)

GPIOをまとめると以下の通り

|UART |TXD   |RXD   |Function(TX/RX)|CTS   |RTS   |Function(CTS/RTS)|
|:---:|:----:|:----:|:-------------:|:----:|:----:|:---------------:|
|UART0|GPIO14|GPIO15|ALT0           |GPIO16|GPIO17|ALT3             |
|UART1|GPIO14|GPIO15|ALT5           |GPIO16|GPIO17|ALT5             |
|UART2|GPIO 0|GPIO 1|ALT4           |GPIO 2|GPIO 3|ALT4             |
|UART3|GPIO 4|GPIO 5|ALT4           |GPIO 6|GPIO 7|ALT4             |
|UART4|GPIO 8|GPIO 9|ALT4           |GPIO10|GPIO11|ALT4             |
|UART5|GPIO12|GPIO13|ALT4           |GPIO14|GPIO15|ALT4             |
