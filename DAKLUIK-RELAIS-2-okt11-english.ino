/* roof open/close automation with 4 buttons: 1- totally close, 2- close a step, 3-open a step, 4 open all the way. 
 *  been running from oct 2017 till now(aug 2018)
 *  mostly without problems, no soldered contacts, just pins and cables
 *  those made for a few problems.
 *  no connection to weather station made yet. 
 */


const int plus1 = 5;    // relay 1 plus contacts Green cable in middle connector, in relay OFF position 
const int min2 = 6;     // relay 2 min contacts Black cable in relay middle connection in OFF position
const int pwr3 = 7;     // relay 3 for motor power; +  to relay 1
const int pwr4 = 8;     // relay 4 for motor power; -  to relay 2
const int knop1 = 10;  // buttons 1234 flatcable connections: Gnd2143
const int knop2 = 9;
const int knop3 = 12;
const int knop4 = 11;
const int LED = 13;
const int swlinks = 2; // switch at end of toothrail left side (roof closed)
const int swrechts= 3; // switch at end of rail right side (totally open)
const int gaopen = 1; //   for future connections (in case of sun: Open roof) // isr uitgezet, uno kan alleen op 2 en 3
const int gadicht = 4; //  for future connection: close roof interrupt (in case of rain?)
volatile boolean helemaallinks = 0;  // volatile so it is available in all functions, like interrupts
volatile boolean helemaalrechts = 0;  // indicator of where the motor is, entirely left or right or neither.
volatile boolean zon = 0;             // sun?
volatile boolean regen = 0;            // rain? 
int button = 0;            // don't remember what this is for, will see
int interval = 10;         // size of the step of the motor makes the distance travelled; depends on the motor used and the voltage to the motor.

int tijd = 0;              // set counter 
int kort=20; // step counter 'short' in 0,01 seconds, with this motor 70- on 9 volts means 3 steps till open. 40 means 5 steps
int lang=200; // time to travel from one side to the other, actually takes 160 or so, we take 200 to include a margin. in 0.01 seconds.
int i = 0; // sensor counter

// Interrupt Service Routine (ISR) links (left)
void isrlinks ()
{
  if (digitalRead (swlinks) == LOW) // if end reached
  {
    digitalWrite (LED, HIGH);   // turn on led to acknowledge button pressed
    helemaallinks = 1;  // this means it 's totally left now, so it won't go further
    helemaalrechts= 0;    // thus it is not right
     digitalWrite(plus1,HIGH);     // go right for 0.01 second (10000 us)
      digitalWrite(min2,HIGH);     // LOW =relay ON, So right(open) to relieve end switch
      digitalWrite(pwr3,LOW);      // power on
      digitalWrite(pwr4,LOW);
      digitalWrite(LED,HIGH);
      delayMicroseconds(10000);  // the only delay that works in an interrupt function. 10000 us=0.01 second, maximum:16383.
  }
}  // end of eindlinks interrupt

// Interrupt Service Routine (ISR) Ga Open
void openen()
{
  if (digitalRead (gaopen) == LOW)
  {
    digitalWrite (LED, HIGH);
      zon = 1;
      regen=0;
  }
}  // end of openen interrupt

// Interrupt Service Routine (ISR) Ga Dicht
void sluiten ()
{
  if (digitalRead (gadicht) == LOW)
  {
    digitalWrite (LED, HIGH);
     regen=1;
     zon=0;
   }
}  // end of sluiten interrupt
  

// Interrupt Service Routine (ISR) rechts
void isrrechts()
{
  if (digitalRead (swrechts) == LOW)
  {
    digitalWrite (LED, HIGH);
    helemaalrechts = 1;
    helemaallinks=0;
  digitalWrite(plus1,LOW);         //stress relief
      digitalWrite(min2,LOW);     // LOW =relay ON, close 0.01 sec.
      digitalWrite(pwr3,LOW);      // power ON
      digitalWrite(pwr4,LOW);
      digitalWrite(LED,HIGH);
  delayMicroseconds(10000);  // delay in ISR . 10000 us=0.01 sec. max 16383.
   
  }
}  // end of eindrechts interrupt

void setup() {

 pinMode(plus1, OUTPUT);
 pinMode(min2, OUTPUT);
 pinMode(pwr3, OUTPUT);
 pinMode(pwr4, OUTPUT);
 pinMode(knop1, INPUT_PULLUP);
 pinMode(knop2, INPUT_PULLUP);
 pinMode(knop3, INPUT_PULLUP);
 pinMode(knop4, INPUT_PULLUP);
 pinMode(LED, OUTPUT);
 pinMode(swlinks, INPUT); // vv pullup
 pinMode(swrechts, INPUT); // vv pullup
 pinMode(gaopen, INPUT); // vv pullup
 pinMode(gadicht, INPUT); // vv pullup
 digitalWrite (swlinks, HIGH);  // internal pull-up resistor
   attachInterrupt (digitalPinToInterrupt (swlinks), isrlinks, CHANGE);  // attach interrupt handler
 digitalWrite (swrechts, HIGH);  // internal pull-up resistor
  attachInterrupt (digitalPinToInterrupt (swrechts), isrrechts, CHANGE);  // attach interrupt handler
   digitalWrite (gaopen, HIGH);  // internal pull-up resistor
   digitalWrite (gadicht, HIGH);  // internal pull-up resistor
 isrlinks();
 isrrechts();
}

void loop() {
 // everything OFF
 // so motor ready to move
 delay(10);
digitalWrite(pwr3, HIGH);
digitalWrite(pwr4, HIGH);
digitalWrite(LED, LOW);
  
if (digitalRead(knop1)==0)                                 // KNOP 1
{
  isrlinks();
  tijd=lang;
  if (helemaallinks ==0) 
  {
    links();
    helemaalrechts=0; // cannot be both 1
  }
 else { digitalWrite (LED, HIGH);
       delay(350);
       }
  
 }

  

else if (digitalRead(knop2)==0)                               // KNOP 2
{
  isrlinks();
  tijd=kort;
 if (helemaallinks ==0) 
  {
    links();
    helemaalrechts=0;
  }
 else { digitalWrite (LED, HIGH);
       delay(350);
       }
}
else if (digitalRead(knop3)==0)                             // KNOP 3
{
  isrrechts();
  tijd=kort;
  if (helemaalrechts==0)
  {
    rechts();
    helemaallinks=0;
  }
   else { digitalWrite (LED, HIGH);
       delay(350);
       }
}
else if (digitalRead(knop4)==0)                              // KNOP 4
{
  isrrechts();
  tijd=lang;
  if (helemaalrechts==0)
  {
    rechts();
    helemaallinks=0;
  }
   else { 
       digitalWrite (LED, HIGH);
       delay(350);
       }
}
else if (digitalRead(gaopen)==LOW)
{
  zon=1; 
  digitalWrite (LED, HIGH);               
   tijd=(2 * lang);     // alternative for gadicht pin not supporting isr 
    rechts();            // actual function call
    zon=0;               // 
    }
else if (digitalRead(gadicht)==LOW)  
{
  regen=1;     
  digitalWrite (LED, HIGH);               
   tijd=(2 *lang);      // alternative for gadicht pin not supporting isr 
    links();            // 
    regen=0;            // 
}


digitalWrite(pwr3, HIGH);
digitalWrite(pwr4, HIGH);
digitalWrite(LED, LOW);
if ((helemaallinks==1) && (helemaalrechts==1)) // debug from testfailure
{ 
  helemaallinks=1;
  helemaalrechts=0;
}
}
  
                                                         // RECHTS
void rechts() 
{
   
  i=0;
  if (helemaalrechts==1) { i= tijd; } //check end switch, before begin
 while ( i < tijd) 
    {
      delay(10);  //0,01s counter time, delay up front: even less time between interrupt and stop motor.
      if (helemaalrechts==1) { //check end switch (triggered by isr while moving)
        i= tijd;  
       
         }
      else { i++; }
      digitalWrite(plus1,HIGH);
      digitalWrite(min2,HIGH);     // LOW =relais ON, (open)
      digitalWrite(pwr3,LOW);      // power to relais 1 and 2; motor runs.
      digitalWrite(pwr4,LOW);
      digitalWrite(LED,HIGH);
      
    }
   
 digitalWrite(pwr3, HIGH);
 digitalWrite(pwr4, HIGH);
 digitalWrite(LED,LOW);
      
 }
                                                      // LINKS
void links()
{
   i=0;
    if (helemaallinks == 1){ i=tijd; } //check end switch, before start
    while ( i < tijd)
    {
      delay(interval);  //0,01s time counter upfront to shorten time to stop motor.
      if (helemaallinks == 1){ i=tijd; } //check end switch ,while moving
      else { i++;}
      
      digitalWrite(plus1, LOW);  // motor left, close  
      digitalWrite(min2, LOW);
      digitalWrite(pwr3, LOW);
      digitalWrite(pwr4, LOW);
      digitalWrite(LED,HIGH);
        
     }
     
 
 digitalWrite(pwr3, HIGH);
 digitalWrite(pwr4, HIGH);
 digitalWrite(LED,LOW);
      
}
