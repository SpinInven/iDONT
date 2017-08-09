#ifndef LEDSM_H
#define LEDSM_H
#define STATE_LED_INIT_OFF               (0)
#define STATE_LED_OFF                    (1)
#define STATE_LED_BLINKING_ON            (2)
#define STATE_LED_BLINKING_ON_WAIT       (3)
#define STATE_LED_BLINKING_OFF           (4)
#define STATE_LED_BLINKING_OFF_WAIT      (5)


#define ESP8266_LED (5)

unsigned int __led_ms;

int has_passed(unsigned int start, unsigned int duration)
{
  return millis() - start > duration;
}
void led_state_machine(unsigned char* state)
{
  switch(*state)
  {
    case STATE_LED_INIT_OFF:
      *state = STATE_LED_OFF;
    break;
    case STATE_LED_OFF: break;
    case STATE_LED_BLINKING_ON:
      digitalWrite(ESP8266_LED, LOW);
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
      digitalWrite(ESP8266_LED, HIGH);
      __led_ms = millis();
      *state = STATE_LED_BLINKING_OFF_WAIT;
    break;
    case STATE_LED_BLINKING_OFF_WAIT: 
      if (has_passed(__led_ms, 300))
      {
        *state = STATE_LED_BLINKING_ON;
      }
    break;

    
  }
  
}
#endif

