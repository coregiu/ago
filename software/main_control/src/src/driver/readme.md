### 舵机控制示例
```c
pca_mg9xx(0,0,0,0,1); //通道0 从0度转到0度
delay_ms(2000);
pca_mg9xx(0,90,90,0,1); //通道0 从90度转到90度
delay_ms(2000);
pca_mg9xx(0,180,180,0,1); //通道0 从180度转到180度
delay_ms(2000);
```