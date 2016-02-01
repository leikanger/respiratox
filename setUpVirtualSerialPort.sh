# IMPORTANT: apitude install socat !!!
# A virtual serial port can be set up using [socat] terminal command. 
socat -d -d pty,raw,echo=0 pty,raw,echo=0  &
