#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
String userInput;
void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  Serial.begin(9600);
}

int door_open_time = 0;
int password_wrong_time = 0;
enum PasswordState {
  ENTER, CORRECT, WRONG
};
enum DoorState {
  CLOSE, OPEN, TIMEOUT
};
PasswordState password_state = ENTER;
DoorState door_state = CLOSE;
const int MAX_DOOR_OPEN_TIME = 5000; // 最大开门时间 5 秒
const int PROMPT_DELAY = 5000;
String data;
String password = "17343075"; // 开门的密码
const char* empty_line = "                ";
const char* mask = "****************";
void loop() {
  passwordVerify();
  checkPrompt();
  displayMessage();
  checkDoor();
}

void checkPrompt() {
  if (password_state == WRONG) {
    if (millis() - password_wrong_time > PROMPT_DELAY) {
      password_state = ENTER;
    }
  }
}

void lcdPrintLine(String msg, int line) {
  lcd.setCursor(0, line);
  lcd.print(msg);
  if (msg.length() < 16)
    lcd.print(empty_line + msg.length());
}

void displayMessage() {
  switch (door_state) {
    case CLOSE: {
        switch (password_state) {
          case WRONG:
            lcdPrintLine("Wrong password!", 0);
            lcdPrintLine("Try again", 1);
            break;
          case ENTER:
          default:
            lcdPrintLine("Enter password:", 0);
            lcdPrintLine(mask + (16 - data.length()), 1);
            break;
        }
      }
      break;
    case OPEN:
      lcdPrintLine("Door is open", 0);
      lcdPrintLine("", 1);
      break;
    case TIMEOUT:
      lcdPrintLine("Open timeout!", 0);
      lcdPrintLine("Close door now!", 1);
      break;
    default:
      break;
  }
}

void timeoutDoor() {
  door_state = TIMEOUT;
  Serial.write("timeout");
}

void passwordWrong() {
  closeDoor();
  password_state = WRONG;
  password_wrong_time = millis();
  delay(10);
  Serial.write("fail");
}

void passwordCorrect() {
  openDoor();
  password_state = CORRECT;
  Serial.write("success");
}

void openDoor() {
  door_state = OPEN;
  door_open_time = millis(); // 记录开门时刻
  delay(10);
  Serial.write("open");
}

void closeDoor() {
  door_state = CLOSE;
  Serial.write("close");
}

void checkDoor() {
  if (door_state == OPEN) {
    if (millis() - door_open_time > MAX_DOOR_OPEN_TIME) {
      // 开门时间超时
      timeoutDoor();
    }
  }
}

void passwordVerify() {
  if (Serial.available()) {
    char ch = (char) Serial.read();
    switch (ch) {
      case '#':
        if (door_state == CLOSE) {
          if (data == password) {
            passwordCorrect();
          } else {
            passwordWrong();
          }
          data = "";
        }
        break;
      case 'A':
        data = data.substring(0, data.length() - 1);
        break;
      case 'B':
        closeDoor(); // 模拟关门
        break;
      default:
        if (ch >= '0' && ch <= '9' && door_state == CLOSE) {
          password_state = ENTER;
          String new_data = String(ch);
          data += new_data;
        }
        break;
    }
  }
}
