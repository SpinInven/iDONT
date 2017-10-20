#ifndef LEDSM_H
#define LEDSM_H

#include "utils.h"

#define STATE_LED_INIT_OFF               (0)
#define STATE_LED_OFF                    (1)

#define STATE_LED_INIT_BLINKING          (2)
#define STATE_LED_BLINKING_ON            (2)
#define STATE_LED_BLINKING_ON_WAIT       (3)
#define STATE_LED_BLINKING_OFF           (4)
#define STATE_LED_BLINKING_OFF_WAIT      (5)


#define STATE_LED_INIT_ON                (6)
#define STATE_LED_ON                     (7)



#define NODE_MCU_D0 (16)
#define NODE_MCU_D1 (5)
#define NODE_MCU_D2 (4)
#define NODE_MCU_D3 (0)
#define NODE_MCU_D4 (2)
#define NODE_MCU_D5 (14)
#define NODE_MCU_D6 (12)
#define NODE_MCU_D7 (13)
#define NODE_MCU_D8 (15)
#define NODE_MCU_D9 (3)
#define NODE_MCU_D10 (1)

#define ESP8266_LED (5)

#define MAIN_LED  NODE_MCU_D6
#define SECOND_LED  NODE_MCU_D2

void initialize_node_mcu_io()
{
  pinMode(NODE_MCU_D6, OUTPUT);  // LED 1
  pinMode(NODE_MCU_D2, OUTPUT);  // LED 2
  pinMode(NODE_MCU_D0, OUTPUT);  // Relay 1
  pinMode(NODE_MCU_D1, OUTPUT);  // Relay 2
  pinMode(NODE_MCU_D5, INPUT);   // Doorbell Sw
  pinMode(NODE_MCU_D7, INPUT);   // Silent Doorbell Sw
}
void initialize_hal()
{
   initialize_node_mcu_io();
}

void enable_bell()
{
      digitalWrite(NODE_MCU_D0, LOW);
      digitalWrite(NODE_MCU_D1, LOW);
}
void disable_bell()
{
      digitalWrite(NODE_MCU_D0, HIGH);
      digitalWrite(NODE_MCU_D1, HIGH);
}

unsigned int __led_ms;


void led_state_machine(unsigned char* state, unsigned char LED_PIN)
{
  switch(*state)
  {
    case STATE_LED_INIT_OFF:
      digitalWrite(LED_PIN, HIGH);
      *state = STATE_LED_OFF;
    break;
    case STATE_LED_OFF: break;
    case STATE_LED_BLINKING_ON:
      digitalWrite(LED_PIN, LOW);
      __led_ms = millis();
      *state = STATE_LED_BLINKING_ON_WAIT;
    break;
    case STATE_LED_BLINKING_ON_WAIT:
      if (has_passed(__led_ms, 100))
      {
        *state = STATE_LED_BLINKING_OFF;
      }
    break;
    case STATE_LED_BLINKING_OFF:
      digitalWrite(LED_PIN, HIGH);
      __led_ms = millis();
      *state = STATE_LED_BLINKING_OFF_WAIT;
    break;
    case STATE_LED_BLINKING_OFF_WAIT: 
      if (has_passed(__led_ms, 300))
      {
        *state = STATE_LED_BLINKING_ON;
      }
    break;
    case STATE_LED_INIT_ON:
      digitalWrite(LED_PIN, LOW);
      *state = STATE_LED_ON;
    break;
    case STATE_LED_ON: break;
    
  }
  
}
#endif

