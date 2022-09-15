/*
 * Volkswagen 純正カーナビ（クラリオン製 712SDCWなど）搭載車に付属している
 * CAN-BUSアダプターのステアリングリモコン線が出力する抵抗値を
 * SUZUKI車などのステアリングリモコン線が出力する抵抗値に変換する
 * 
 * リレー 6系統, 抵抗 6系統
 */

const int INPUT_PIN = A5; // 電圧測定用入力ピン
const int Vcc = 5.0; // 入力電圧
const int Rt = 994; // 基準抵抗

// スイッチ判別用の定数（取り回しを良くするために出力ピンの定数を流用）
const int SW_NONE = 0; // 1.305k
const int SW_MUTE = 2; // 55.8
const int SW_VOLUP = 4; // 130.2
const int SW_VOLDOWN = 6; // 239
const int SW_MODE = 8; // 55.8
const int SW_SEEKUP = 10; // 1.03k - 1.21k
const int SW_SEEKDOWN = 12; // 1.25k - 1.28k


int pressedButton(float r_in) {
  // TODO: 現地合わせ
  if (r_in > 5000) {
    return SW_NONE;
  }
  if (r_in > 50 && r_in < 60) {
    return SW_MUTE;
  }
  if (r_in > 115 && r_in < 135) {
    return SW_VOLUP;
  }
  if (r_in > 220 && r_in < 250) {
    return SW_VOLDOWN;
  }
// CAN-BUSアダプター出力にMODEスイッチなし
//  if (r_in > 900 && r_in < 1100) {
//    return SW_MODE;
//  }
  if (r_in > 720 && r_in < 760) {
    return SW_SEEKUP;
  }
  if (r_in > 1500 && r_in < 1600) {
    return SW_SEEKDOWN;
  }
  return SW_NONE;
}

void connectResistor(int button) {
  if (button == SW_NONE) {
    digitalWrite(SW_MUTE, LOW);
    digitalWrite(SW_VOLUP, LOW);
    digitalWrite(SW_VOLDOWN, LOW);
    digitalWrite(SW_MODE, LOW);
    digitalWrite(SW_SEEKUP, LOW);
    digitalWrite(SW_SEEKDOWN, LOW);
  }
  else {
    digitalWrite(button, HIGH);
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(SW_MUTE, OUTPUT);
  pinMode(SW_VOLUP, OUTPUT);
  pinMode(SW_VOLDOWN, OUTPUT);
  pinMode(SW_MODE, OUTPUT);
  pinMode(SW_SEEKUP, OUTPUT);
  pinMode(SW_SEEKDOWN, OUTPUT);
}

void loop() {  
  // 抵抗値の読み取り
  int analog = analogRead(INPUT_PIN); // アナログ値の読み取り
  float v_x = analog * Vcc/1024.0; // 読み取ったアナログ値を電圧に変換(Rxにかかる電圧)
  float r_x = v_x * Rt / (Vcc-v_x); // 抵抗値の計算

  // debug prints
  Serial.print("RAW: ");
  Serial.print(analog);
 
  Serial.print(" | Voltage: ");
  Serial.print(v_x);
  Serial.print(" V");

  Serial.print(" | Resistor： ");
  Serial.print(r_x, 0);
  Serial.print(" Ω");

  // 抵抗値を元にどのスイッチが押されているのかを判別する
  int button = pressedButton(r_x);

  // debug prints
  switch (button) {
    case SW_NONE:
      Serial.println(" | SW_NONE");
      break;
    case SW_MUTE:
      Serial.println(" | SW_MUTE");
      break;
    case SW_VOLUP:
      Serial.println(" | SW_VOLUP");
      break;
    case SW_VOLDOWN:
      Serial.println(" | SW_VOLDOWN");
      break;
    case SW_MODE:
      Serial.println(" | SW_MODE");
      break;
    case SW_SEEKUP:
      Serial.println(" | SW_SEEKUP");
      break;
    case SW_SEEKDOWN:
      Serial.println(" | SW_SEEKDOWN");
      break;
  }

  // 押されているボタンに対応する抵抗値を出力する
  connectResistor(button);
  
  delay(100);
}
