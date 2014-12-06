jack_autoconnect
================

Tiny application that reacts on port registrations by clients and connects them. 
The port names are interpreted as regular expressions and more than one pair can be defined upon calling.

Example
================
Automatically connect the first two ports of a newly created instance of MPlayer to the analyzer plugin of CALF jack host:
```
./jack_autoconnect "MPlayer.*:out_0" "calf.*:analyzer_in_l" "MPlayer.*:out_1" "calf.*:analyzer_in_r"
```

Automatically connect the jack ports of mpd to any connected netadapter client. That's what I created this for:
```
./jack_autoconnect "Music Player Daemon:left" ".*:to_slave_1" "Music Player Daemon:right" ".*:to_slave_2"
```

