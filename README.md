# Beam64
## Wireless N64 Controller Modification
_**This project and document is a work in progress**_   
Goal:  
Permanently modify an N64 controller to support wireless communication with no significant impact caused by increased latency  

### The N64 Controller Protocol  
Communication between controller and console is birdirectional over a single wire. The console acts as master and requests information from the controller by sending commands over the wire. Each command from the console is 8 bits long followed by a stop bit. When a command is receieved, the controller responds with the appropriate data followed by a controller stop bit. Communication operates at 250khz and each bit is 4 microseconds long. On the wire logical 1's are described with 1 microsecond of low voltage followed by 3 microseconds of high. Similarily, logical 0's are described with 3 microseconds of low voltage followed by 1 microsecond of high voltage. More detailed information is available in the links at the bottom of the document. 


## Proof of Concept (completed July 5th)  
#### Goals  
1. Fully understand the N64 Controller communication protocol  
2. Explore bluetooth as a method for transmitting controller data wirelessly   
3. Determine if latency will be an issue  
4. Determine if the Arduino can sample the data accuractly    
	
#### Hardware  
2 x Arduino Nano  
2 x HC-05 Bluetooth Transceivers  
1 x N64 Console  
1 x N64 Controller with wires exposed  

#### High Level System Diagram  

This is what the high level of the prototype looks like:   


![High Level Diagram][high_level_system]  

#### Proof of Concept Operation  
Here I will briefly describe the operation of the system.  
Every 15ms **Arduino Nano 1** sends a single byte command to the **N64 controller** to request the current state of the buttons and joystick. The **N64 controller** responds with this information (called button data in the diagram) put in 4 bytes of data. I then encode the button data slightly so that each of the bytes can be uniquely identified. Although this increases the data size to 6 bytes, it is an important step since it is possible for bytes to be dropped on the receiving end at **Arduino Nano 2**. Once the button data is encoded, they are sent serially using the USART chip on the Nano to **bluetooth transciever 1**. **Bluetooth transciever 2** receives the button data and spits it out by USART to **Arduino Nano 2**. 


## Version 1  
#### Functional Requirements  
##### Communication  
(F1) Controller communicates wirelessly with N64 console  
(F2) Controller automatically extablishes communication with console on boot  
(F3) Controller enters sleep mode if not used for 5 minutes  

##### Power  
(F4) Controller can be recharged without removing batteries  
(F5) Controller battery life is greater than or equal to 4 hours  
(F6) Controller displays current battery capacity  

#### Non-functional Requirements  
(NF1) There is no noticeable increase in button input latency compared to wired controller  



---

#### References  

[N64 Controller Protocol Info 1](http://afermiano.com/index.php/n64-controller-protocol)  
[N64 Controller Protocol Info 2](https://kthompson.gitlab.io/2016/07/26/n64-controller-protocol.html)  
[Project with controller to USB](https://os.mbed.com/users/fomartin/notebook/n64-controller-interface/)  
[N64 Patent](https://patentimages.storage.googleapis.com/a0/db/08/11d1c70ea3e80b/US6454652.pdf)  
[Controller Chip Pinout](https://bitbuilt.net/forums/index.php?threads/official-controller-chip-pinout.58/)   

[high_level_system]: https://github.com/CamEmil/Beam64/raw/master/Diagrams/Exports/System_High_Level.png "High Level Diagram"  
