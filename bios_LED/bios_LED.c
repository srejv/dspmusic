//bios_LED.c DSP/BIOS application to flash LEDs

void blink_LED0()											
{
	DSK6416_LED_toggle(0);
}
void blink_LED1()
{
	DSK6416_LED_toggle(1);
}
void blink_LED2() 										
{
   	DSK6416_LED_toggle(2);
}
void blink_LED3()								 		  
{
  	DSK6416_LED_toggle(3);
}

void main()
{
  DSK6416_LED_init();
  return;
}
