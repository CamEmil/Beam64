# Beam64
### Wireless Modification for N64 Controller  
The goal of Beam64 is to permanently modify an N64 controller to wirelessly communicate with an N64 console with latency that is undetectable by humans.  

**This modification is currently in the prototyping stage.**  
The goal of this prototyping stage is to:  
1. Fully understand the N64 Controller communication protocol
2. Explore bluetooth as a method for transmitting controller data wirelessly
3. Determine if latency will be an issue
4. Is arduino fast enough at 16MHZ to sample the incoming data?
5. Have fun!




---

### ChangeLog
**April 28th, 2019:**
-- Found a [potential microcomputer](http://www.ti.com/product/CC2541) with bluetooth 4 integrated into the system, plenty of chinese vendors selling it on aliexpress  
-- Discovered that the N64 controller protocol can be approximated with UART, will probably write assembly to do the pin polling instead  

**April 11th, 2019:**  
-- Figured out how to read directly from the digital pin registers  

**April 9th, 2019:**  
-- Digging into arduino source code to speed up function calls  

**April 6th, 2019:**  
Added Arduino code for early testing of communication with N64 controller   
- Able to send command byte 0x00 to controller
- Able to see that data is being sent back, still need latch it properly  

---  


References

[N64 Controller Protocol Info 1](http://afermiano.com/index.php/n64-controller-protocol)  
[N64 Controller Protocol Info 2](https://kthompson.gitlab.io/2016/07/26/n64-controller-protocol.html) 
[Project with controller to USB](https://os.mbed.com/users/fomartin/notebook/n64-controller-interface/) 
[N64 Patent](https://patentimages.storage.googleapis.com/a0/db/08/11d1c70ea3e80b/US6454652.pdf)  
[Controller Chip Pinout](https://bitbuilt.net/forums/index.php?threads/official-controller-chip-pinout.58/)  



[Prototype_Block_Diagram]: .\Diagrams\Exports\System_High_Level.png