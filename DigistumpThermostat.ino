#include <DigiCDC.h>
#include <OneWire.h>

#define SKIP_ROM_MSG          (0xCC)
#define CONVERT_T_MSG         (0x44)
#define READ_SCRATCHPAD_MSG   (0xBE)
#define DS_MIN                (1)
#define DS_MAX                (110)
#define DS_ERR                (0)
#define DS_PIN                (2)
#define HEATER_PIN            (1)
#define TARGET_TEMP           (75)

OneWire ds(DS_PIN);
byte data[12];
int temp;


boolean readTemperature(){
  ds.reset();
  /* Start conversion */
  ds.write(SKIP_ROM_MSG, 1);
  ds.write(CONVERT_T_MSG, 1);

  /* Extra power for temp conversion */
  pinMode(DS_PIN, OUTPUT);
  digitalWrite(DS_PIN,HIGH);
  /* Wait some time... */
  SerialUSB.delay(1500);
  
}

boolean getTemperature(){
  byte i;  
  byte present = 0;
  pinMode(DS_PIN, INPUT);
  
  
  present = ds.reset();  

  if(!present){
    temp = 0;
    return false;
  }
  
  // Issue Read scratchpad command
  ds.write(SKIP_ROM_MSG, 1);
  ds.write(READ_SCRATCHPAD_MSG, 1);
  
  // Receive 9 bytes
  for ( i = 0; i < 9; i++) {
    data[i] = ds.read();
  }
  // Calculate temperature value
  temp = ( (data[1] << 8) + data[0] )/16;
  return true; 
}


void setup() {                
  // initialize the digital pin as an output.
  SerialUSB.begin(); 
  pinMode(HEATER_PIN, OUTPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  readTemperature();  
  getTemperature();
  SerialUSB.println(temp);

  if(temp < TARGET_TEMP){
    digitalWrite(HEATER_PIN,HIGH);
  }else{
    digitalWrite(HEATER_PIN,LOW);
  }
  
  SerialUSB.delay(1000);
  
  
}
