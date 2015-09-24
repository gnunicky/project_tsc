GOAL OF PROJECT:
When you press the botton in End Device DIO1 (with state high) switch to state low and the Coordinator send a frame changes the state of the DIO0 LED in Router.

Problem:
The the coordinator must read status the button continuously the pin DIO1 or End Device just press the botton send a frame a coordinator changes the state 
of the DIO0 LED in Router.

Solutions:
The project use of the libraries xbee api. The implement of in different languages:
- c language : http://attie.co.uk/libxbee
- python language : https://code.google.com/p/python-xbee/
- java language : https://code.google.com/p/xbee-api/


ZIGBEE COORDINATOR API=1: 
- Type: XB24-ZB version 21A7 (XB24-Z7WIT-004 revK)
- Serial Number/Address: 0013A20040B18060
- PAN ID: 13
- DH: 0
- DL: FFFF

ZIGBEE ROUTER 1 API=1:
- Type: XB24-ZB version 23A7 (XB24-Z7WIT-004 revK)
- Serial Number/Address: 0013A20040A238D4 - Short Address: "51AF" o "Q\xaf"
- PAN ID: 13
- DH: 0013A200
- DL: 40B18060

ZIGBEE END DEVICE 1  API=1:
- Type: XB24-ZB version 23A7 (XB24-Z7WIT-004 revK)
- Serial Number/Address: 0013A20040B1805B - Short Address: "3168"o "1h" 
- PAN ID: 13
- DH: 0013A200
- DL: 40B18060

ZIGBEE ROUTER 2 API=1:
- Type: XB24-ZB version 23A7 (XB24-Z7WIT-004 revK)
- Serial Number/Address: 0013A200408B5F51 - Short Adress:"185C"
- PAN ID: 13
- DH: 0013A200
- DL: 40B18060

ZIGBEE END DEVICE 2 API=1:
- Type: XB24-ZB version 23A7 (XB24-Z7WIT-004 revK)
- Serial Number/Address: 0013A20040AF5A99 - Short Adress:"C949"  
- PAN ID: 13
- DH: 0013A200
- DL: 40B18060

ZIGBEE END DEVICE 3 API=1:
- Type: XB24-ZB version 29A7 (XB24-Z7WIT-004 revK)
- Serial Number/Address: 0013A20040B18074 - Short Adress:"78FA"   
- PAN ID: 13
- DH: 0013A200
- DL: 40B18060


THE CONFIGURATION NETWORK IS:
            Coordinator     <- Router 1   <-     End Device 1
                                |                    |
                                |                    |  
                           LED1 (DIO0)          BUTTON1 in (DIO1)       

                             <- Router 2  <- End Device 2   <-     End Device 3
                                                     |                    |
                                                     |                    |  
                                                 LED2 (DIO0)          BUTTON2 in (DIO1)       

P.S. Initial Condition: 
     LED1 = 5 Digital Output Off DIO0 in Router 1     
     BUTTON1 = 3 Digital Input in DIO1 in End Device 1
     LED2 = 5 Digital Output Off DIO0 in End Device 2     
     BUTTON2 = 3 Digital Input in DIO1 in End Device 3
