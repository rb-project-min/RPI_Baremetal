# 0108_memory1

MMUを有効にします。

単にキャッシュを有効にすると、必要ないところまでキャッシュされ、正しく動かない可能性があります。  
そのため、アドレスごとにキャッシュを制御して、正しく動作させる必要があります。  
具体的には、GPU領域とI/O領域以降はキャッシュなし、その他はキャッシュあり。  
なお、0104_cacheのキャッシュ制御は無効にしないとフリーズするので注意。

メモリマップは以下の通り

RaspberryPi1/Zeroの場合

0x00000000-0x00003FFF 割り込みテーブル  
0x00004000-0x00007FFF MMUテーブル  
0x00008000-0x1DFFFFFF カーネル他  
0x1E000000-0x1EFFFFFF GPU(16MBの場合)  
0x1F000000-0x1FFFFFFF 空き領域  
0x20000000-0x20FFFFFF I/O  
0x21000000-0xFFFFFFFF 未使用

RaspberryPi2/3の場合

0x00000000-0x00003FFF 割り込みテーブル  
0x00004000-0x00007FFF MMUテーブル  
0x00008000-0x1DFFFFFF カーネル他  
0x3E000000-0x3EFFFFFF GPU(16MBの場合)  
0x3F000000-0x3FFFFFFF I/O  
0x40000000-0x40FFFFFF Local Peripheral  
0x41000000-0xFFFFFFFF 未使用

RaspberryPi4の場合

0x00000000-0x00003FFF 割り込みテーブル  
0x00004000-0x00007FFF MMUテーブル  
0x00008000-0x1DFFFFFF カーネル他  
0x3E000000-0x3EFFFFFF GPU(16MBの場合)  
0x3F000000-0xFDFFFFFF 空き領域(4GBモデルの場合)  
0xFE000000-0xFF7FFFFF I/O  
0xFF800000-0xFFFFFFFF Local Peripheral

初期化手順)  
・mailboxでGPUからGPUメモリ領域のアドレスを取得  
・0x00000000～GPU下端アドレス:キャッシュあり  
・GPU下端アドレス～GPU上端アドレス:キャッシュなし  
・GPU上端アドレス～I/O下端アドレス:キャッシュあり  
・I/O下端アドレス～:キャッシュなし  
・上記4エリアの情報(テーブル)を0x00004000～0x00008000に書き込む  
・ドメインアクセス制御レジスタを設定  
・TLB,分岐予測器アレイ,命令キャッシュを無効にする  
・データキャッシュを無効にする  
・TTBCR, TTBR0の書き換え  
・MMUを有効にする
