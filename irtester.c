#include <stdio.h>
#include <wiringPi.h>
#include "ifttt.h"
#include <time.h>


void ALARM_OFF(void);
void ALARM_ARMING(void);
void ALARM_ARMED(void);
void ALARM_TRIGGERED(void);
void ALARM_SOUNDING(void);


int main(int argc, char *argv[])
{
  int i;
  wiringPiSetup () ;
  pinMode(0, INPUT);	/*motion detector signal*/
  pinMode(1, OUTPUT);	/*red led*/
  pinMode(2, OUTPUT);	/*green led*/
  pinMode(3, INPUT);	/*switch*/
  pinMode(4, OUTPUT);  /*buzzer*/
  
  while(1) {
  	  ALARM_OFF();
	/*printf("Waiting for reset\n");
    while(digitalRead(0) == 1);
    printf("Waiting for event\n");
    while(digitalRead(0) == 0);
    printf("Alarm\n");*/
  }

  /*NOTREACHED*/
  return 0 ;
}

/*green led on, red led off, buzzer is silent, goes from off to arming by pressing button*/
void ALARM_OFF(void)
{
  int i;
  wiringPiSetup () ;
  pinMode(0, INPUT);	/*motion detector signal*/
  pinMode(1, OUTPUT);	/*red led*/
  pinMode(2, OUTPUT);	/*green led*/
  pinMode(3, INPUT);	/*switch*/
  pinMode(4, OUTPUT);  /*buzzer*/
 
 pullUpDnControl(3, PUD_UP) ;
  
  while(1) 
  {
     digitalWrite(2, HIGH);  /* turn on green light*/ 
     digitalWrite(1,LOW); /* turn off  Red light*/  	
     digitalWrite(4, LOW); delay(1000); /* turn off  Buzzer*/  
     
     
    if(digitalRead(3) == 0) /* while switch reads in a 0*/  
    {
    	printf("calling alarm Arming \n");   	
    	ALARM_ARMING();
    }
     
  }
  
}

/*The buzzer is silent. For 10 seconds the system blinks LED 1 on and off for 1 second duration. At
the end of 10 seconds the system leaves LED 1 off but turns LED 2 on.
The system then transits to ALARM_ARMED*/
void ALARM_ARMING(void)
{
  pinMode(0, INPUT);	/*motion detector signal*/
  pinMode(1, OUTPUT);	/*red led*/
  pinMode(2, OUTPUT);	/*green led*/
  pinMode(3, INPUT);	/*switch*/
  pinMode(4, OUTPUT);  /*buzzer*/
  const int RUNTIME = 10;  
  time_t start, current;

  while(1)
  {
      digitalWrite(4, LOW); delay(1000); /* turn off  Buzzer*/  
      digitalWrite(2, LOW) ; /*  green off*/
      
      
	start = time(NULL);
	do {
	current = time(NULL);
	
/* do stuff */	
 	digitalWrite (1, HIGH) ; delay (1000); /* blink red led in 1 second intervals*/
 	digitalWrite(1, LOW); delay(1000);
/* do stuff */

	} while (difftime(current, start) < RUNTIME);
	
	break;
  }
  	digitalWrite (2, HIGH) ; /* blink green stays on*/
  	
  	/* call Alarm Alarmed */
    printf("calling alarm ARMED \n");   	  	
   	ALARM_ARMED();
 	
}


/*LED 1 is off. LED 2 is on. The buzzer is silent. The system remains in ALARM_ARMED mode until one of two events
happens. Event one: The button is pressed (this is the system disarmed
event). If this happens the system reverts to the ALARM_OFF state. The
second option is that the alarm is triggered. If this event occurs then
the system moves to ALARM_TRIGGERED.*/
void ALARM_ARMED(void)
{
  pinMode(0, INPUT);	/*motion detector signal*/
  pinMode(1, OUTPUT);	/*red led*/
  pinMode(2, OUTPUT);	/*green led*/
  pinMode(3, INPUT);	/*switch*/
  pinMode(4, OUTPUT);  /*buzzer*/
  pullUpDnControl(3, PUD_UP) ;

  
  digitalWrite(1, LOW); /* red light off*/
  digitalWrite(2, HIGH) ; /* green stays on*/
  digitalWrite(4, LOW); delay(1000); /* turn off  Buzzer*/  

  /* 2 events: The button is pressed (this is the system disarmed
event). If this happens the system reverts to the ALARM_OFF state

second option, ir detector, triggers an alarm
*/ 
  while(1)
  { 
  	  if(digitalRead(3) == 0) /* while switch reads in a 0 aka is pressed*/  
  	  {
    	printf("Alarm DISARMED \n");   	
    	ALARM_OFF();
      }
      
  	  if(digitalRead(0) == 1) /* while IRdetector sees intruder*/  
  	  {
    	printf("Alarm TRIGGERED \n");   
    	ALARM_TRIGGERED();   	
      }
  }
}

/*An intruder has been detected and the intruder
has 10 seconds to disable the alarm. During the ALARM_TRIGGERED
state the buzzer is silent and the two LED’s blink on and off once every
two seconds. This process continues for 10 seconds. If during this
period the button is pressed, the system is deactivated and the system
revers to the ALARM_OFF state. If the button is not pressed within ten
seconds then the system moves into ALARM_SOUNDING.*/
void ALARM_TRIGGERED(void)
{
  pinMode(0, INPUT);	/*motion detector signal*/
  pinMode(1, OUTPUT);	/*red led*/
  pinMode(2, OUTPUT);	/*green led*/
  pinMode(3, INPUT);	/*switch*/
  pinMode(4, OUTPUT);  /*buzzer*/
 pullUpDnControl(3, PUD_UP) ;
  
  const int RUNTIME = 10;  
  time_t start, current;
  
  digitalWrite(4, LOW); delay(1000); /* turn off  Buzzer*/  
  digitalWrite(1, LOW); /* turn off  red*/  
  digitalWrite(2, LOW);/* turn off  green, to prep for blinking*/  
   
    while(1)
    {
         
		start = time(NULL);
		do {
		current = time(NULL);
		
	/* do stuff */	
 	digitalWrite (1, HIGH) ; delay(75); /* blink red led in 2 second intervals*/
 	digitalWrite (2, HIGH) ; delay(75); /* blink green led in 2 second intervals*/	

 	digitalWrite(1, LOW); delay(75); /* green off*/
 	digitalWrite(2, LOW); delay(75); /* red off*/
	
	
  	  if(digitalRead(3) == 0) /* while switch reads in a 0 aka is pressed*/  
  	  {
    	printf("Alarm DISARMED \n");   	
    	ALARM_OFF();
      }
	/* do stuff */
	
		} while (difftime(current, start) < RUNTIME);
		break;
	/* time's up, call alarm sounding */		
  }
  	printf("Alarm Sounding \n");
  	ALARM_SOUNDING();

}

/* The buzzer and both LEDs are on for two
seconds, then off for two seconds and a single alarm message is sent to
the alarm company (an IFTTT event is triggered). The system remains in
this state unit the button is pushed, at which point the system reverts to
the ALARM_OFF state*/
void ALARM_SOUNDING(void)
{
  pinMode(0, INPUT);	/*motion detector signal*/
  pinMode(1, OUTPUT);	/*red led*/
  pinMode(2, OUTPUT);	/*green led*/
  pinMode(3, INPUT);	/*switch*/
  pinMode(4, OUTPUT);  /*buzzer*/
  pullUpDnControl(3, PUD_UP) ;

  printf("message sent to ifttt \n");
   ifttt("https://maker.ifttt.com/trigger/alarm_triggered/with/key/dOAsUOxosjh0MW7O3HSvFl", "Status: Alarm TRIGGERED", "7:05:57pm Saturday, October 21, 2018", "Backyard");  
  while(1)
  {
	  digitalWrite(4, HIGH); delay(2000); /* turn ON  Buzzer*/  
	  digitalWrite(1, HIGH); /* turn on  red*/  
	  digitalWrite(2, HIGH);/* turn on  green, to prep for blinking*/  

	  digitalWrite(4, LOW); delay(2000); /* turn ON  Buzzer*/  
	  digitalWrite(1, LOW); /* turn off  red*/  
	  digitalWrite(2, LOW);/* turn off  green, to prep for blinking*/  	  
	  
  	  if(digitalRead(3) == 0) /* while switch reads in a 0 aka is pressed*/  
  	  {
  	  	printf("Alarm DISARMED \n");   	
    	ALARM_OFF();
  	  }
  }
  
}














