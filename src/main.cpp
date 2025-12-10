#include <Arduino.h>

static const int pin_input = 2;
static const int pin_motor = 4;
static const int pin_protc = A0;
static const int pin_led = 5;

const unsigned long max_tempo = 10000; /* 10 segundos */
bool ativo = false;
unsigned long tempo = 0;

void ativa_motor()
{
  ativo = true;
  tempo = millis();
  digitalWrite(pin_motor, HIGH);
  digitalWrite(pin_led, HIGH);
  Serial.println("MOTOR ON");
}

void desliga_motor()
{
  ativo = false;
  digitalWrite(pin_motor, LOW);
  digitalWrite(pin_led, LOW);
  Serial.println("MOTOR OFF");
}

/* para uma proteção de 1,5A.
1,5 A * 0,1 Ohm = 0,15V
o ganho do amp op (não inversor) foi colocado em 21; 3,15 V no ADC do mcu
o adc mapea valores de 0 até 1023, sendo 1023 <-> 5V; logo se o valor do adc for 645
indica sobrecorrente na carga.*/

void protc_leitura()
{
  int tensao = analogRead(pin_protc);
  if (tensao > 645)
  {
    desliga_motor();
    Serial.println("SOBRECORRENTE - MOTOR OFF");
  }
}

void setup()
{
  Serial.begin(9600);
  pinMode(pin_input, INPUT_PULLUP);
  pinMode(pin_motor, OUTPUT);
  pinMode(pin_led, OUTPUT);
}

void loop()
{
  if (digitalRead(pin_input) == 0 && ativo == false)
  {
    delay(50); /* pequeno debounce */
    ativa_motor();
  }

  if (ativo)
  {
    protc_leitura();
    if ((millis() - tempo) > max_tempo)
    {
      desliga_motor();
    }
  }
}
