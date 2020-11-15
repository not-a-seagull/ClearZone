# ClearZone

This is a tech demo for a game engine, built by John "not\_a\_seagull" Nunley, Thomas "Dubl" Jeffrey, and Braden "LOTR2023" McPhail. 

## Build Instructions

Build the GTK+ interface:

```
sudo apt install libgtk-3-dev
mkdir build && cd build
cmake ..
```

Build the Terminal interface:

```
sudo apt install libncurses-dev
mkdir build && cd build
cmake .. -DUI_NAME=termui
```

Web UI coming soon?

## License 

GPLv3
