# folder for auto-car files
PID method
reference: https://tutorial.cytron.io/2012/06/22/pid-for-embedded-design/

PID formular we get to apply:
Output = Kp* error + Ki* Integral + 

In our smart car case, 
the suggestion is to PID control the turning speed and make the speed of car constant for the first trial of PID control.
(Perhas the speed can also has another algorithm later )

Variable Declaration:
Output = Turning angle
Error = Target Value – Current Value = distance between the (magnetic line) and (the mid-point of the car) --> relate to the reading value of magnetic sensors
Integral = integral(error,dt)+ Error
Derivative = Error – Last Error


Further Explain:
Kp* error
=>Make the turning angle ∝ error, means if the error is large, lefe/right turning angle will be large too.
=>Small Kp will make turning smooth but turning speed slow. Large Kp will make turning speed quick but maybe a bit unstable. =>Suitable Kp value can be test by experiment.

Ki* Integral
=> make sure the PID system notice the tiny error that cannot be adjust by (Kp* error).
=> Purpose of Integral is to amplify the tiny error.
=> integration of error over time = sum of the error in a certain time => we don't have to know the function of Error

Kd*derivative(error,dt)
=>  The damping effect of the D controller allows the system to have a higher value of Kp and/or Ki without overshooting.
=>  the derivative is the rate of change of the error -> Derivative = Error – Last Error
=>  -ve values of derivative -> improvement; +ve values of derivative -> overshoot
=>  In code:
   Last Error = Error
   Error = Set Point – Process Variable
   Derivative = Error – Last Error
=> If the Kd is too high, the system will become jerky if the feedback loop is noisy.

Implement by code:

Last Error = Error
Error = Set Point – Process Variable
Integral = Integral + Error
Derivative = Error – Last Error
Control Variable = (Kp * Error) + (Ki * Integral) + (Kd * Derivative)










