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

