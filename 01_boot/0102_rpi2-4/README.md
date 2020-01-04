# 0102_rpi2-4

RaspberryPi2～4でも動作させます。

RaspberryPi2以降はGPIOにアクセスするためのアドレスが変更されているため、それぞれの設定が必要になる。
確認のためにMainIDレジスタ(CPUIDが格納される)を使用する。

・RaspberryPi 1/Zeroの場合
　MainIDレジスタ: 0x410FB767
　Peripheral Base Addr: 0x20000000
　Local Peripherals Addr: なし
・RaspberryPi 2/3の場合
　MainIDレジスタ: 0x410FC075 または 0x410FD034
　Peripheral Base Addr: 0x3F000000
　Local Peripherals Addr: 0x40000000
・RaspberryPi 4の場合
　MainIDレジスタ: 0x410FD083
　Peripheral Base Addr: 0xFE000000
　Local Peripherals Addr: 0xFF800000

なお、MainIDの中で末尾4bit(RevisionID)が変更される可能性を考慮して、このビットはクリアしておく。

手順)
・MainIDレジスタを読み込む
・末尾4bitをクリア
・MainIDレジスタごとにアドレスを代入
・このアドレスをmain関数内で使用する

