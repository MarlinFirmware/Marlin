WARNING: 
--------
This is an experimental modification to allow PID on your bed heater.

This will run at the same frequency as the main PID loop.  Make sure you heater FET or SSR can do this. I use a fotek SSR-10DA and it's fine

add something like this to you configuration (pulling this branch will get you this

~~~~~~~~~~~~~~~~~~~~
#define PIDTEMPBED

//from FOPDT model - kp=.39 Tp=405 Tdead=66, Tc set to 79.2, argressive factor of .15 (vs .1, 1, 10)
    #define  DEFAULT_bedKp 10.00
    #define  DEFAULT_bedKi .023
    #define  DEFAULT_bedKd 305.4
~~~~~~~~~~~~~~~~~~~~


Autotune works for the bed.  Give it an "M303 E-1 C8 S90" to run autotune on the bed at 90 degrees for 8 cycles.



