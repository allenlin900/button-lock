# button-lock
# MSP430 按鍵密碼鎖

##  專案簡介
這是一個簡易按鍵密碼鎖示範程式。  
使用單一按鍵透過長按或短按來輸入密碼，密碼正確時會以紅色LED閃爍方式表示成功，  
若密碼錯誤則會亮起錯誤綠色LED。

## 使用技術
- C 語言（適用 MSP430 MCU）
- 使用Timer_A 量測按鍵按下時間，區分短按長按

## 預設密碼
- 密碼組合長度：4 位
- 組合邏輯：
  - `LONG_PRESS` = 長按
  - `SHORT_PRESS` = 短按
- 預設密碼：**長 短 短 短**

## 按鍵判斷
- 按下到放開的時間 < 50,000 μs 視為短按
- 按下到放開的時間 ≥ 50,000 μs 視為長按
- 使用 `Timer_A` 連續模式量測

## 功能說明
- **成功輸入**：P1.0 LED 閃爍 3 次（表示解鎖成功）
- **輸入錯誤**：P1.6 LED 亮 1 秒後熄滅（表示密碼錯誤）
- 每次輸入完 4 次按鍵後，自動比對密碼並重置輸入





