# inferring-location-based-on-sun-path
## Documentation of the original project for which the algorithm is being developed
 The code for this original version is also included in the repository under the name LightTracker1.
 ### LightTracker1
  #### Authors
    Concept, Hardware, 3D design and printing - Djordje Herceg
    Software, Documentation, Interactive capabilities - Aleksandar Horvat
    9.7.2024
  
  #### Libraries
    ESP32Servo
    BH1750
    
  #### Hardware components
    SG-90 servo x 2
    BH1750 Ambient Light Sensor x 1
    LED x 1
  
  #### Hardware connection scheme
    ## BH1750
    VCC -- 3.3V
    GND -- GND
    SCL -- ESP pin 22
    SDA -- ESP pin 21
    ADDR -- GND
  
  #### SG-90 SERVO
    Brown (top) - GND
    Red (middle) - 5V
    Orange (bottom) - PWM
  
  #### DESCRIPTION
    This version uses an "ONE_TIME_HIGHRES_MODE" which is slow. It takes almost one second to obtain a reading.
  
  #### PROJECT ASSIGNMENT
    Implement the following commands:
    READ vangle, hangle - move the sensor to (vangle, hangle), read and return the lux value
    TABLE vstart, vend, hstart, hend - move the sensor from hstart to hend in 5-degree steps, then move from vstart to vend in 5-degree steps and take a light reading at each step. Return the table of read values
    
    In each case, the read value is {vangle, hangle, lux}
    The table has the shape  
    {
      { {vangle11, hangle11, lux11}, {vangle12, hangle12, lux12}, {vangle1n, hangle1n, lux1n} },
      { {vangle21, hangle21, lux21}, {vangle22, hangle22, lux22}, {vangle2n, hangle2n, lux2n} },
      ...
      { {vanglem1, hanglem1, luxm1}, {vanglem2, hanglem2, luxm2}, {vanglemn, hanglemn, luxmn} }
    }
    
    Sweep strategy: for each vertical angle, sweep horizontally from hmin to hmax, then increase vangle, and then sweep from hmax to hmin.
