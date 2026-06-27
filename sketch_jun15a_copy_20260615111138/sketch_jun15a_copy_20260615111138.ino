#include <LedControl.h>


LedControl lc = LedControl(11, 13, 10, 4);



byte myDisplayMatrix[4][8][8] = {
  
  {
    { 0, 0, 0, 0, 0, 0, 0, 0 },  
    { 0, 0, 0, 0, 0, 0, 0, 0 },  
    { 0, 0, 0, 0, 0, 0, 0, 0 },  
    { 0, 0, 0, 0, 0, 0, 0, 0 },  
    { 0, 0, 0, 0, 0, 0, 0, 0 },  
    { 0, 0, 0, 0, 0, 0, 0, 0 },  
    { 0, 0, 0, 0, 0, 0, 0, 0 },  
    { 0, 0, 0, 0, 0, 0, 0, 0 }   
  },

  
  {
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 } },

 
  {
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 } },

  
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
  
  for (int i = 0; i < 4; i++) {
    lc.shutdown(i, false);  
    lc.setIntensity(i, 4);  
    lc.clearDisplay(i);     
  }
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
          myDisplayMatrix[i][(k + l) % 8][7 - j] = 1;
        }
        updateScreen3D();
        delay(300 -(i*8+j)*9);
        if (digitalRead(2)) {
          while (digitalRead(2)) {
          }
          if (j == 0) {
            for (int l = 0; l < blockLength; l++) {
              inButtom[(k + l) % 8] = 1;
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
          myDisplayMatrix[i][(k + l) % 8][7 - j] = 0;
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
}

void updateScreen3D() {
  for (int chip = 0; chip < 4; chip++) {
    for (int row = 0; row < 8; row++) {

      byte rowData = 0B00000000;

      for (int col = 0; col < 8; col++) {
        if (myDisplayMatrix[chip][row][col] == 1) {
          rowData |= (1 << (7 - col)); 
        }
      }
      lc.setRow(chip, row, rowData);
    }
  }
}