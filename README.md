### 2020-2-12

```
# 透传
AT+CIPMUX=0
AT+CIPSTART="TCP","ip",port

# 多连模式
AT+CIPMUX=1
AT+CIPSTART=id,"TCP","ip",port
```