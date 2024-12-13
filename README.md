Workaround for a hard crash bug in a Eidos Printess labeler machine for a industrial enviroment. 
PC application pings the printserver and Eidos printer at regular intervals and communicates with the hardware to retrieve the physical printer status.
If the printserver responds (to verify network is not down) and the printer does not AND the printer is turned on, it is assumed the printer crashed.
PC application will then request a printer power cycle and reset from the hardware. When the printer becomes ready again the PC application will 
emulate a mouse click on the printer application's (running on same PC) "reprint" button. 
