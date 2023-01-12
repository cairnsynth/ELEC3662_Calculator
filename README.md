# ELEC3662_Calculator

Simple calculator written in C to run on the Tiva Launchpad.

## Functional Description

Calculator has the following mathematical functions:

- Addition, subtraction, multiplication, and division
- Calculate exponentials 
  - *10^x
- Supports input of multiple operation expressions, i.e
  - 1+2-3*4/5
- Calculations follow correct order of precedence
- Math and syntax error handling

### Extra Features

- Password locking
  - Password is stored in on-chip flash memory
- Custom graphic display

## Implementation

The calculator implements a basic form of Dijkstra's Shunting Yard algorithm in order to convert an infix equation into postfix for calculation.
This equation handles operator precedence, increasing the efficiency of the actual calculations. While not currently implemented, support for parentheses,
functions (sinx, cosx, tanx) could be added with little work.
The Shunting Yard function takes a char array as input, so could be repurposed for another program.

## Circuit Schematic

![circuit diagram final](https://user-images.githubusercontent.com/95569413/212055082-e3704b91-0ad6-4d97-989b-276b565acf8b.png)


### Connections

-LCD
|**NAME** |**PIN**|**CONNECTION**|
|:--------|:-----:|----------:|
|Vss      |(1)    |GND        |
|Vdd      |(2)    |VBUS       |
|Vo       |(3)    |R1 (10K)   |
|RS       |(4)    |PA3        |
|R/W      |(5)    |GND        |
|E        |(6)    |PA2        |
|DB3-0    |(10-7) |GND        |
|DB7-4    |(14-11)|PB3-0      | 
|LED B/L+ |(15)   |R2 (10R)   |
|LED B/L- |(16)   |GND        |

-KeyPad
|**PIN**|**CONNECTION**|
|:------|-------------:|
|1|PD0|
|2|PD1|
|3|PD2|
|4|PD3|
|5|PE0|
|6|PE1|
|7|PE2|
|8|PE3|
