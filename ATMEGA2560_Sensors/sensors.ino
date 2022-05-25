#include <Wire.h>
#include <LiquidCrystal.h>
#include <Keypad.h>
#define NOTE_C4  262

int red_led = 49;
int green_led = 48;
int position = 1;
const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {{'1', '2', '3'}, {'4', '5', '6'}, {'7', '8', '9'}, {'*', '0', '#'}};
byte rowPins[ROWS] = {18, 19, 20, 21};
byte colPins[COLS] = {15, 16, 17};
const String kilidin_sifresi = "1111";
String girilen_sifre;
int lm35 = A0;
float voltaj_deger = 0;
float sicaklik = 0;
int gelen_veri = 0;
int yangin = 53;
int alarm_buzzer = 8;
int hareket = 13;
int hareket_led = 10;

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
LiquidCrystal lcd1(9, 7, 6, 1, 0, 14);

void setup()
{
  lcd.setCursor(0, 0);
  lcd.print("SICAKLIK DURUMU");
  lcd1.setCursor(0, 0);
  lcd1.print("SIFRELI KAPI");
  delay(100);
  girilen_sifre.reserve(32);
  sifre_durumu(true);
  tone(8, NOTE_C4, 10);
  noTone(8);
  lcd.begin(16, 4);
  lcd1.begin(16, 4);
  pinMode(hareket, INPUT);
  pinMode(hareket_led, OUTPUT);
  pinMode(yangin, INPUT);
  pinMode(alarm_buzzer, OUTPUT);
  pinMode(green_led, OUTPUT);
  pinMode(red_led, OUTPUT);
}

void loop()
{
  int yangin_ctrl = 0;
  int hareket_ctrl = digitalRead(hareket);
  yangin_ctrl = digitalRead(yangin);
  gelen_veri = analogRead(lm35);
  voltaj_deger = (350 / 1023.0) * gelen_veri;
  sicaklik = voltaj_deger / 10.0;
  lcd.clear();
  lcd.home();
  lcd.print("Ortam = ");
  lcd.print(sicaklik);
  lcd.print("^C");
  lcd.setCursor(0, 1);
  if (hareket_ctrl > 0) {
    digitalWrite(hareket_led, HIGH);
  }
  else {
    digitalWrite(hareket_led, LOW);
  }
  if (sicaklik < 20) {
    lcd.print("Sicaklik Azaldi");
  }
  else if (sicaklik > 30) {
    lcd.print("Sicaklik Artti");
  }
  if (yangin_ctrl == 1) {
    digitalWrite(alarm_buzzer, HIGH);
  }
  else {
    digitalWrite(alarm_buzzer, LOW);
  }
  while (position == 1)
  {
    int a;
    char key = keypad.getKey();

    if (key)
    {

      if (key == '*')
      {
        girilen_sifre = "";
        sifre_durumu(true);
      }

      else if (key == '#')
      {
        if (kilidin_sifresi == girilen_sifre)
        {
          sifre_durumu(false);
          digitalWrite(green_led, HIGH);
          digitalWrite(red_led, LOW);
          position = 2;
        }

        else
        {
          lcd1.clear();
          lcd1.setCursor(0, 0);
          lcd1.print("HATALI SIFRE   ");
          digitalWrite(green_led, LOW);
          digitalWrite(red_led, HIGH);
        }

        girilen_sifre = "";
      }

      else
      {
        girilen_sifre += key;
        lcd1.print("*");
      }
    }
  }

  while (position == 0)
  {
    char key = keypad.getKey();

    if (key)
    {

      if (key == '*')
      {
        girilen_sifre = "";
        sifre_durumu(false);
      }

      else if (key == '#')
      {
        if (kilidin_sifresi == girilen_sifre)
        {
          sifre_durumu(true);
          position = 2;
          digitalWrite(green_led, HIGH);
          digitalWrite(red_led, LOW);
        }

        else
        {
          lcd1.clear();
          lcd1.setCursor(0, 0);
          lcd1.print("HATALI SIFRE   ");
          digitalWrite(green_led, LOW);
          digitalWrite(red_led, HIGH);
        }

        girilen_sifre = "";
      }

      else
      {
        girilen_sifre += key;
        lcd1.print("*");
      }
    }
  }
  delay(200);
}

void sifre_durumu(int kapinin_sifresi)
{
  if (kapinin_sifresi)
  {
    lcd1.clear();
    lcd1.setCursor(0, 0);
    lcd1.print("KILITLI KAPI");
    lcd1.setCursor(0, 1);
    lcd1.print("SIFRE: ");
  }

  else
  {
    lcd1.clear();
    lcd1.setCursor(0, 0);
    lcd1.print("KAPI ACILDI");
    lcd1.setCursor(0, 1);
    lcd1.print("HOSGELDINIZ :)");
  }
}
