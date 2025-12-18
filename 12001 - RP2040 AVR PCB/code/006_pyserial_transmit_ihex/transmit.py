import serial
import struct
from intelhex import IntelHex

# Configuration
SERIAL_PORT = '/dev/ttyACM0'
HEX_FILE = 'main.hex'
PAGE_SIZE = 256

def upload_hex():
    # Load the hex file
    ih = IntelHex(HEX_FILE)
    
    # Open serial connection
    with serial.Serial(SERIAL_PORT, 115200, timeout=2) as ser:
        
        print("--> Entering Programming Mode...")
        ser.write(struct.pack('>BIB', 0x01, 0, 0)) # Opcode 0x01
        if ser.read(1) != b'\x00':
            print("Failed to initialize AVR!"); return

        print("--> Erasing Chip...")
        ser.write(struct.pack('>BIB', 0x02, 0, 0)) # Opcode 0x02
        ser.read(1) # Wait for ACK

        # Get the range of data in the HEX file
        start_addr = ih.minaddr()
        end_addr = ih.maxaddr()
        
        print(f"--> Writing Flash ({start_addr} to {end_addr})...")

        # Iterate through memory in 256-byte pages
        for addr in range(start_addr, end_addr + 1, PAGE_SIZE):
            # Extract 256 bytes from the hex object (fills gaps with 0xFF)
            page_data = ih.tobinarray(start=addr, size=PAGE_SIZE)
            
            # Construct the 260-byte packet: 
            # 1-byte Op (0xFF), 4-byte Addr, 256-bytes Data
            header = struct.pack('>BI', 0xFF, addr)
            packet = header + bytes(page_data)
            
            ser.write(packet)
            
            # Wait for RP2040 to finish the SPI page write
            if ser.read(1) == b'\x00':
                print(f"    Page at {hex(addr)}: OK")
            else:
                print(f"    Page at {hex(addr)}: FAILED"); break

        print("--> Exiting...")
        ser.write(struct.pack('>BIB', 0xFE, 0, 0)) # Opcode 0xFE
        print("Done!")

if __name__ == "__main__":
    upload_hex()
