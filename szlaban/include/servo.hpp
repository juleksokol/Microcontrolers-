



#ifndef SERVO_HPP 
#define SERVO_HPP

void set_lights(bool red, bool green);

void servo_setup(unsigned int angle);

void open_gate();

void close_gate();

void action(unsigned long now);


#endif // SERVO_HPP