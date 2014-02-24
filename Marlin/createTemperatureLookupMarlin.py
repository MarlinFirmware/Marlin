#!/usr/bin/python
#
# Creates a C code lookup table for doing ADC to temperature conversion
# on a microcontroller
# based on: http://hydraraptor.blogspot.com/2007/10/measuring-temperature-easy-way.html
"""Thermistor Value Lookup Table Generator

Generates lookup to temperature values for use in a microcontroller in C format based on: 
http://hydraraptor.blogspot.com/2007/10/measuring-temperature-easy-way.html

The main use is for Arduino programs that read data from the circuit board described here:
http://make.rrrf.org/ts-1.0

Usage: python createTemperatureLookup.py [options]

Options:
  -h, --help        show this help
  --rp=...          pull-up resistor
  --t0=ttt:rrr      low temperature temperature:resistance point (around 25C)
  --t1=ttt:rrr      middle temperature temperature:resistance point (around 150C)
  --t2=ttt:rrr      high temperature temperature:resistance point (around 250C)
  --num-temps=...   the number of temperature points to calculate (default: 20)
"""

from math import *
import sys
import getopt

class Thermistor:
    "Class to do the thermistor maths"
    def __init__(self, rp, t1, r1, t2, r2, t3, r3):
        t1 = t1 + 273.15               # low temperature (25C)
        r1 = r1                        # resistance at low temperature
        t2 = t2 + 273.15               # middle temperature (150C)
        r2 = r2                        # resistance at middle temperature
        t3 = t3 + 273.15               # high temperature (250C)
        r3 = r3                        # resistance at high temperature
        self.rp = rp                   # pull-up resistance
        self.vadc = 5.0                # ADC reference
        self.vcc = 5.0                 # supply voltage to potential divider
        a1 = log(r1)
        a2 = log(r2)
        a3 = log(r3)
        z = a1 - a2
        y = a1 - a3
        x = 1/t1 - 1/t2
        w = 1/t1 - 1/t3
        v = pow(a1,3) - pow(a2,3)
        u = pow(a1,3) - pow(a3,3)
        c3 = (x-z*w/y)/(v-z*u/y)
        c2 = (x-c3*v)/z
        c1 = 1/t1-c3*pow(a1,3)-c2*a1
        self.c1 = c1
        self.c2 = c2
        self.c3 = c3

    def res(self,adc):
        "Convert ADC reading into a resolution"
        res = self.temp(adc)-self.temp(adc+1)
        return res

    def v(self,adc):
        "Convert ADC reading into a Voltage"
        v = adc * self.vadc / (1024 )   # convert the 10 bit ADC value to a voltage
        return v

    def r(self,adc):
        "Convert ADC reading into a resistance in Ohms"
        v = adc * self.vadc / (1024 )   # convert the 10 bit ADC value to a voltage
        r = self.rp * v / (self.vcc - v)    # resistance of thermistor
        return r

    def temp(self,adc):
        "Convert ADC reading into a temperature in Celcius"
        v = adc * self.vadc / (1024 )   # convert the 10 bit ADC value to a voltage
        r = self.rp * v / (self.vcc - v)    # resistance of thermistor
        lnr = log(r)
        Tinv = self.c1 + (self.c2*lnr) + (self.c3*pow(lnr,3))
        return (1/Tinv) - 273.15        # temperature

    def adc(self,temp):
        "Convert temperature into a ADC reading"
        y = (self.c1 - (1/(temp+273.15))) / (2*self.c3)
	x = sqrt(pow(self.c2 / (3*self.c3),3) + pow(y,2))
        r = exp(pow(x-y,1.0/3) - pow(x+y,1.0/3)) # resistance of thermistor
        return (r / (self.rp + r)) * (1024)

def main(argv):

    rp = 4700;
    t1 = 25;
    r1 = 100000;
    t2 = 150;
    r2 = 1641.9;
    t3 = 250;
    r3 = 226.15;
    num_temps = int(36);
    
    try:
        opts, args = getopt.getopt(argv, "h", ["help", "rp=", "t1=", "t2=", "t3=", "num-temps="])
    except getopt.GetoptError:
        usage()
        sys.exit(2)
        
    for opt, arg in opts:
        if opt in ("-h", "--help"):
            usage()
            sys.exit()
        elif opt == "--rp":
            rp = int(arg)
        elif opt == "--t1":
            arg =  arg.split(':')
            t1 = float( arg[0])
            r1 = float( arg[1])
        elif opt == "--t2":
            arg =  arg.split(':')
            t2 = float( arg[0])
            r2 = float( arg[1])
        elif opt == "--t3":
            arg =  arg.split(':')
            t3 = float( arg[0])
            r3 = float( arg[1])
        elif opt == "--num-temps":
            num_temps =  int(arg)

    max_adc = (1024 ) - 1
    min_temp = 0
    max_temp = 350
    increment = int(max_adc/(num_temps-1));
            
    t = Thermistor(rp, t1, r1, t2, r2, t3, r3)
    tmp = (min_temp - max_temp) / (num_temps-1)
    print tmp
    temps = range(max_temp, min_temp + tmp, tmp);

    print "// Thermistor lookup table for Marlin"
    print "// ./createTemperatureLookupMarlin.py --rp=%s --t1=%s:%s --t2=%s:%s --t3=%s:%s --num-temps=%s" % (rp, t1, r1, t2, r2, t3, r3, num_temps)
    print "// Steinhart-Hart Coefficients: %.15g, %.15g,  %.15g " % (t.c1, t.c2, t.c3)
    print "//#define NUMTEMPS %s" % (len(temps))
    print "const short temptable[NUMTEMPS][2] PROGMEM = {"

    counter = 0
    for temp in temps:
        counter = counter +1
        if counter == len(temps):
            print "   {(short)(%.2f*OVERSAMPLENR), %s}  // v=%s r=%s res=%s C/count" % ((t.adc(temp)), temp, t.v(t.adc(temp)), t.r(t.adc(temp)),t.res(t.adc(temp)))
        else:
            print "   {(short)(%.2f*OVERSAMPLENR), %s}, // v=%s r=%s res=%s C/count" % ((t.adc(temp)), temp, t.v(t.adc(temp)), t.r(t.adc(temp)),t.res(t.adc(temp)))
    print "};"
    
def usage():
    print __doc__

if __name__ == "__main__":
    main(sys.argv[1:])
