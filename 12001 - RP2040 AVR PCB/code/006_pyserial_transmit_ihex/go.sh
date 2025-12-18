cp ../001_atmega328p_blink/main.hex .
sudo chmod 777 /dev/ttyACM0
python transmit.py
