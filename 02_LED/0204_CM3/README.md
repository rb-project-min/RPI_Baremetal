# 0204_CM3

ComputeModule 3/3+ におけるオンボードLチカ

ComputeModule 3/3+では、ACT LEDがExpGPIO 2で操作可能ですが、  
PWR LEDはそもそも存在しないため、操作できません。  
ここでは、とりあえずACT LEDのみでLチカを行います。  
通常のGPIOとは異なり、mailboxによる操作になります。

手順)  
・mailboxによりExpGPIO 2を一定間隔でON/OFFさせる
