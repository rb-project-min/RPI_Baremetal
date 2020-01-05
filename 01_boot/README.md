# 01_Boot

start.Sをちゃんと使えるようにします。  
なお、実験のためGPIO16を使用します。  
RaspberryPiの初期型の場合はオンボードLEDをON/OFF  
その他はGPIO16をON/OFFするので、LED,抵抗を接続すること。(またはテスターで電圧確認)

0101_simple  
とりあえず動くようにする(RaspberryPi2以降はGPIOが動かない)

0102_rpi2-4  
RaspberryPi2以降でも動作するようにする

0103_mode_change  
システムモードに変更して動くようにする

0104_cache  
キャッシュ,FPU等を有効化する

0105_interrupt  
割り込みテーブルを書き込む

0106_stack  
スタックポインタを設定する

0107_multicore  
RaspberryPi2以降でマルチコアを有効にする  
(以下、RaspberryPi2以降ではGPIO16～19をON/OFFする)

0108_memory1  
MMUを有効にしてキャッシュを制御する

0109_memory2  
メモリを連続領域として利用できるようにする(特に2GB以上のモデルに有効)

0110_BssClear  
BSSセグメントの初期化
