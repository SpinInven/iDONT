#define STATE_LED_INIT_OFF               (0)
#define STATE_LED_OFF                    (1)
#define STATE_LED_BLINKING_ON            (2)
#define STATE_LED_BLINKING_ON_WAIT       (3)
#define STATE_LED_BLINKING_OFF           (4)
#define STATE_LED_BLINKING_OFF_WAIT      (5)


#define ESP8266_LED (5)

os_timer_t myTimer;

bool tickOccured;

// start of timerCallback
void timerCallback(void *pArg) {

      tickOccured = true;

} // End of timerCallback

void led_sm_setup()
{
  tickOccured = false;
  os_timer_setfn(&myTimer, timerCallback, NULL);
  os_timer_arm(&myTimer, 300, true);
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
      
      os_timer_arm(&myTimer, 100, true);
      *state = STATE_LED_BLINKING_ON_WAIT;
    break;
    case STATE_LED_BLINKING_ON_WAIT:
      if (tickOccured)
      {
        *state = STATE_LED_BLINKING_OFF;
        tickOccured = false;
      }
    break;
    case STATE_LED_BLINKING_OFF:
      digitalWrite(ESP8266_LED, HIGH);
      os_timer_arm(&myTimer, 400, true);
      *state = STATE_LED_BLINKING_OFF_WAIT;
    break;
    case STATE_LED_BLINKING_OFF_WAIT: 
      if (tickOccured)
      {
        *state = STATE_LED_BLINKING_ON;
        tickOccured = false;
      }
    break;

    
  }
  
}

