/// Vars used in code
long  sensorValue = 0;
int   Temp_Gain = 1000; /// gain error (x10*-1.0)+1000 I.E.>  if error = -0.5% Temp_Gain = 1005
int   Temp_Offset = -7;  /// offset error /10 I.E.> if error = 2.1 degrees Temp_Offset = 21
long  FValue = 0;      
float FValue1000 = 0,tempC=0;   // using float here uses a lot of flash mem, this can be converted to long and then some tricks can be done on the display output,but I have lots of flash for this demo.....

void setup() {
  Serial.begin(9600);
}

void loop() {
  ////////////////////////////////////////////////////////////////////////
  //// measure internal temperature and display

  FValue1000 = 0;
  for (int count = 0; count < Temp_Gain; count++)
  {

    ADC10CTL1 = INCH_10 + ADC10DIV_3;         // Temp Sensor ADC10CLK/4
    ADC10CTL0 = SREF_1 + ADC10SHT_3 + REFON + ADC10ON + ADC10IE;
    TACCR0 = 30;                              // Delay to allow Ref to settle
    TACCTL0 |= CCIE;                          // Compare-mode interrupt.
    TACTL = TASSEL_2 | MC_1;                  // TACLK = SMCLK, Up mode.
    LPM0;                                     // Wait for delay.
    TACCTL0 &= ~CCIE;                         // Disable timer Interrupt
    ADC10CTL0 |= ENC + ADC10SC;               // Sampling and conversion start
    __bis_SR_register(CPUOFF + GIE);          // LPM0 with interrupts enabled

    sensorValue = ADC10MEM;                   // store the ADC10 value

    FValue=(((sensorValue) - 630) * 761) / 1024;   // do math on ADC10 value to convert to degrees F
    FValue1000 = FValue1000 + FValue;              // store value (this is how we can apply gain to the measurement)
  }

  FValue1000 = (FValue1000/Temp_Gain);    // do gain error
  FValue1000 = FValue1000 + Temp_Offset;  // do offset error

  tempC=((FValue1000-32)*5)/9;

  Serial.println(tempC);
  delay(100);

}
