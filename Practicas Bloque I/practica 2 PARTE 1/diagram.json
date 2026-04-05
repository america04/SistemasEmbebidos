{
  "version": 1,
  "author": "Anonymous maker",
  "editor": "wokwi",
  "parts": [
    { "type": "wokwi-arduino-uno", "id": "uno", "top": 0, "left": 0, "attrs": {} },
    {
      "type": "wokwi-pushbutton",
      "id": "btn1",
      "top": 92.6,
      "left": 297.6,
      "rotate": 90,
      "attrs": { "color": "red", "xray": "1" }
    },
    {
      "type": "wokwi-led",
      "id": "led1",
      "top": -109.2,
      "left": -5.8,
      "attrs": { "color": "green" }
    },
    {
      "type": "wokwi-resistor",
      "id": "r1",
      "top": -15.25,
      "left": 19.2,
      "attrs": { "value": "220" }
    },
    {
      "type": "wokwi-resistor",
      "id": "r2",
      "top": -34.45,
      "left": 153.6,
      "attrs": { "value": "10000" }
    }
  ],
  "connections": [
    [ "btn1:1.r", "uno:5V", "green", [ "v70.2", "h-179.4" ] ],
    [ "btn1:2.l", "uno:2", "green", [ "v-93.2", "h-6.8" ] ],
    [ "uno:2", "r2:2", "red", [ "v0" ] ],
    [ "r2:1", "uno:GND.1", "red", [ "v0", "h-38.4", "v19.2" ] ],
    [ "r1:2", "uno:8", "blue", [ "v0", "h94.8", "v9.6" ] ],
    [ "led1:A", "r1:1", "gold", [ "v0" ] ],
    [ "led1:C", "uno:GND.1", "gold", [ "v9.6", "h0.4" ] ]
  ],
  "dependencies": {}
}