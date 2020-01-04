# 0107_multicore

マルチコアを有効にします。

RaspberryPi2以降はマルチコアCPUとなるため、複数のスレッドを同時に動作させることができます。  
ただ、スレッドを起動させるのが複雑なので、ちゃんと組まないとうまく動きません。  
マルチコアを起動させるためには、以下の要素が必要になります。  
・他コアの起動  
　メインスレッドから他のコアを動作させるには、他コアのスリープ状態を解除する必要があります。  
　そのために必要な命令がwfe(イベント待ちスリープ)とsev(イベント発生)です。  
　wfeはコアをスリープさせ、イベント発生を待ちます。消費電力が抑えられるのかな？  
　sevは全コアに対してイベントを発生させます。これにより全コアが起動します。  
・mailboxによるスレッド間通信  
　RaspberryPi2以降は、スレッドごとにmailboxという通信機能があります。  
　これを使用して関数ポインタを送れば、目的の関数を起動することができます。  
　アドレスは以下の通り

　Core0用  
　Mailbox0 Write:0x40000080 Read/Clear:0x400000C0  
　Mailbox1 Write:0x40000084 Read/Clear:0x400000C4  
　Mailbox2 Write:0x40000088 Read/Clear:0x400000C8  
　Mailbox3 Write:0x4000008C Read/Clear:0x400000CC

　Core1用  
　Mailbox0 Write:0x40000090 Read/Clear:0x400000D0  
　Mailbox1 Write:0x40000094 Read/Clear:0x400000D4  
　Mailbox2 Write:0x40000098 Read/Clear:0x400000D8  
　Mailbox3 Write:0x4000009C Read/Clear:0x400000DC

　Core2用  
　Mailbox0 Write:0x400000A0 Read/Clear:0x400000E0  
　Mailbox1 Write:0x400000A4 Read/Clear:0x400000E4  
　Mailbox2 Write:0x400000A8 Read/Clear:0x400000E8  
　Mailbox3 Write:0x400000AC Read/Clear:0x400000EC

　Core3用  
　Mailbox0 Write:0x400000B0 Read/Clear:0x400000F0  
　Mailbox1 Write:0x400000B4 Read/Clear:0x400000F4  
　Mailbox2 Write:0x400000B8 Read/Clear:0x400000F8  
　Mailbox3 Write:0x400000BC Read/Clear:0x400000FC

　（ただし、RaspberryPi4の場合は0xFF8000??となる）

初期化手順)  
・Core0(メインスレッド)の初期化処理を完了させる  
・各コアのMailbox3に初期化処理の開始アドレスを書き込む  
・sev命令で全コア起動  
・全コアの初期化処理が完了するまでループ  

イベント待ちルーチン)  
・wfeでイベント待ち  
・イベント発生したら、自コアのMailbox3を読み込む(即クリアする)  
・アドレスが存在したらジャンプ、なければ再度wfeでイベント待ち  

イベント発生ルーチン)  
・起動させたいコアのMailbox3に関数ポインタを書き込む  
・sev命令でイベント発生  

本例題では、コアごとにGPIO16～19をON/OFFします。
