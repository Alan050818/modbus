This is a implementation of RS485 modbus gateway using an USR-TCP232-304 device together with a pymodbus library to take remote control of RTU slave devices over rs485 bus

The components are:

1.- Linux Machine acting as modbus syncronous client i.e. Raspberry pi and pymodbus (Ethernet side)

2.- The Bus using UTP wire (RS485 side)

3.- RS485 Reles

4.- The USR-TCP232-304 gateway to interfacing between Ethernet world and rs485 bus

To achieve this on Raspberry pi you must to install pymodbus library

#### pip install pymodbus


then use this diagram to connect your USR-TCP-232 , RS485 Rele and your Linux machine


![DIAGR](https://raw.githubusercontent.com/pumanzor/modbus/master/USR-TCP232-304/imgs/rs485-schema.png)


and use this code snipet to switch the channel 1 of rele to ON

```

#!/usr/bin/env python

from pymodbus.client.sync import ModbusTcpClient as ModbusClient
from pymodbus.transaction import ModbusRtuFramer

import pymodbus
import serial
from pymodbus.pdu import ModbusRequest

import logging
FORMAT = ('%(asctime)-15s %(threadName)-15s '
          '%(levelname)-8s %(module)-15s:%(lineno)-8s %(message)s')
logging.basicConfig(format=FORMAT)
log = logging.getLogger()
log.setLevel(logging.DEBUG)

import paho.mqtt.client as mqtt
import paho.mqtt.publish as publish
import json, time
import inspect


client1 = ModbusClient('192.168.1.10', port=502, framer=ModbusRtuFramer)
client1.connect()


rq = client1.write_register(1, 256, unit=3)
rr = client1.read_holding_registers(1, 1, unit=3)
dassert(rq, lambda r: r.function_code < 0x80)
status = rr.registers[0]
print(status)
```

explanation of

 rq = client1.write_register(1, 256, unit=3)

* First value = Register Address
* Second value : Register number
* Third value : Device ID

in this case if you are using a RS485 rele (purchased by aliexpress)

First value could be 1 or 2 -- 1 is the first channel or rele and 2 is the second channel or rele

Second Value could be, 256 = ON, 512 = Off

Third Value is the slave address

in this case is 3 you must to setup the dipswitch to 3, like this:

![DIP](https://raw.githubusercontent.com/pumanzor/modbus/master/USR-TCP232-304/imgs/dip.jpg)

As you can see decimal 3 equals to 11 binary, then setup the dip switch 110001, last switch 6 must be set ON because is RTU mode


to read copy this snipet code in your Linux or raspberry machine where you installed pymodbus

```
#!/usr/bin/env python

from pymodbus.client.sync import ModbusTcpClient as ModbusClient
from pymodbus.transaction import ModbusRtuFramer

import pymodbus
import serial
from pymodbus.pdu import ModbusRequest

import logging
FORMAT = ('%(asctime)-15s %(threadName)-15s '
          '%(levelname)-8s %(module)-15s:%(lineno)-8s %(message)s')
logging.basicConfig(format=FORMAT)
log = logging.getLogger()
log.setLevel(logging.DEBUG)

import json, time
import inspect

client1 = ModbusClient('192.168.1.10', port=502, framer=ModbusRtuFramer)
client1.connect()

#client1.read_holding_registers(A, B , unit=C)
#where A=Address of register(s), B=register number, unit=slave id

rr0 = client1.read_holding_registers(1,1 , unit=3)
print(rr0.registers)

```
