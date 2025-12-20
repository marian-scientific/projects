cp ../001_atmega328p_blink/main.hex .
sudo chmod 777 /dev/ttyACM2
python transmit.py
