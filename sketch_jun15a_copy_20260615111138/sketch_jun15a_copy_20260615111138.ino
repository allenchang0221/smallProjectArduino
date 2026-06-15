#include <LedControl.h>

// 參數：DIN=11, CLK=13, CS=10, 總共 4 個晶片
LedControl lc = LedControl(11, 13, 10, 4);

// 完整展開的 [4][8][8] 三維陣列，目前全部為 0（全暗）
// 這裡的每一個數字，都精確對應到螢幕上的那顆 LED 燈
byte myDisplayMatrix[4][8][8] = {
  // === 模組 0 (最左邊的 8x8) ===
  {
    { 0, 0, 0, 0, 0, 0, 0, 0 },  // 第 0 列
    { 0, 0, 0, 0, 0, 0, 0, 0 },  // 第 1 列
    { 0, 0, 0, 0, 0, 0, 0, 0 },  // 第 2 列
    { 0, 0, 0, 0, 0, 0, 0, 0 },  // 第 3 列
    { 0, 0, 0, 0, 0, 0, 0, 0 },  // 第 4 列
    { 0, 0, 0, 0, 0, 0, 0, 0 },  // 第 5 列
    { 0, 0, 0, 0, 0, 0, 0, 0 },  // 第 6 列
    { 0, 0, 0, 0, 0, 0, 0, 0 }   // 第 7 列
  },

  // === 模組 1 (左數第二個 8x8) ===
  {
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 } },

  // === 模組 2 (右數第二個 8x8) ===
  {
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 } },

  // === 模組 3 (最右邊的 8x8) ===
  {
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 } }
};
void setup() {
  Serial.begin(9660);
  // 初始化 4 個晶片
  for (int i = 0; i < 4; i++) {
    lc.shutdown(i, false);  // 喚醒晶片
    lc.setIntensity(i, 4);  // 設定適中亮度
    lc.clearDisplay(i);     // 清除畫面
  }

  // 範例：你可以直接在初始化後改動某些點，再刷到螢幕上
  // 例如點亮模組 0 的中心四個燈
  // myDisplayMatrix[0][3][3] = 1;
  // myDisplayMatrix[0][3][4] = 1;
  // myDisplayMatrix[0][4][3] = 1;
  // myDisplayMatrix[0][4][4] = 1;

  // 刷新畫面
  updateScreen3D();
}
int inButtom[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
int blockLength = 4;
bool restart = false;
void loop() {
  blockLength = 4;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 8; j++) {
      for (int k = 0; k < 8; k++) {
        myDisplayMatrix[i][j][k] = 0;
      }
    }
  }
  for (int i = 0; i < 8; i++) {
    inButtom[i] = 0;
  }
  restart = false;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 8; j++) {
      for (int k = 0;; k++) {
        if (Serial.available()) {
          char data = Serial.read();
          while (data == '1') {
            if (Serial.available()) {
              data = Serial.read();
            }
          }
          if (data == '2') {
            restart = true;
          }
        }
        if (k % 8 > 8 - blockLength) {
          continue;
        }
        for (int l = 0; l < blockLength; l++) {
          myDisplayMatrix[i][(k + l) % 8][7 - j] = 1;  //模擬時7-j改成j
        }
        updateScreen3D();
        delay(200 -(i*8+j)*10);
        if (digitalRead(2)) {
          while (digitalRead(2)) {
          }
          if (j == 0) {
            for (int l = 0; l < blockLength; l++) {
              inButtom[(k + l) % 8] = 1;  //模擬時7-j改成j
            }

          } else {
            int blockPuted = 0;
            for (int l = 0; l < blockLength; l++) {
              if (inButtom[(k + l) % 8]) {
                blockPuted++;
              }
            }
            for (int l = 0; l < 8; l++) {
              inButtom[l] = 0;
            }
            for (int l = 0; l < blockLength; l++) {
              inButtom[(k + l) % 8] = 1;
            }
            blockLength = blockPuted;
            if (blockPuted == 0) {
              restart = true;
            }
          }
          break;
        }
        for (int l = 0; l < blockLength; l++) {
          myDisplayMatrix[i][(k + l) % 8][7 - j] = 0;  //模擬時7-j改成j
        }
        if (restart) {
          break;
        }
      }
      if (restart) {
        break;
      }
    }
    if (restart) {
      break;
    }
  }
  if (restart) {
    Serial.println("lose...");
  } else {
    Serial.println("win~");
  }
  while (true) {
    if (Serial.available()) {
      if (Serial.read() == '2') {
        break;
      }
    }
  }
  // 保持靜態
}

// 核心功能：將 [4][8][8] 陣列轉譯並刷新到畫面上
void updateScreen3D() {
  for (int chip = 0; chip < 4; chip++) {
    for (int row = 0; row < 8; row++) {

      byte rowData = 0B00000000;

      for (int col = 0; col < 8; col++) {
        if (myDisplayMatrix[chip][row][col] == 1) {
          rowData |= (1 << (7 - col));  // 轉為位元資料
        }
      }
      lc.setRow(chip, row, rowData);
    }
  }
}