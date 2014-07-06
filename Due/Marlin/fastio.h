#ifndef	_FASTIO_H
#define	_FASTIO_H



// --------------------------------------------------------------------------
// Function stubs
// --------------------------------------------------------------------------

/// Read a pin wrapper
#define READ(IO) digitalRead(IO)
/// Write to a pin wrapper
#define WRITE(IO, v) digitalWrite(IO,v)

/// toggle a pin wrapper
#define TOGGLE(IO)

/// set pin as input wrapper
#define SET_INPUT(IO) pinMode (IO,INPUT)
/// set pin as output wrapper
#define SET_OUTPUT(IO) pinMode (IO,OUTPUT)

/// check if pin is an input wrapper
#define GET_INPUT(IO)
/// check if pin is an output wrapper
#define GET_OUTPUT(IO)

/// check if pin is an timer wrapper
#define GET_TIMER(IO)

#endif	/* _FASTIO_H */
