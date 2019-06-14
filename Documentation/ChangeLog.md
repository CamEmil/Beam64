### ChangeLog
**June 14th, 2019**  
-- Working initial prototype  
-- Moved this to changelog file (probably will no longer update this document)

**May 21st, 2019**  
-- Completed and tested functions for:  
    -- Requesting controller data and reading it into memory  
	-- Reading command bytes from the N64 and responding with appropriate response  
-- Moved functions that handle communication to a Beam64 library  
-- Set up two bluetooth modules (hc-05) as master and slave and confirmed data transfer works  

**May 15th, 2019**  
-- Completed the assembly function to read an incoming byte  
-- Began work on a library to hold the functions for communicating with the controller and console  
-- Code is still very messy  

**May 7th, 2019**  
-- Ordered three bluetooth transcievers for prototyping with arduino  
-- Wrote a more precise function for sending commands  
-- Began working on an assembly function to read incoming controller data  

**April 28th, 2019:**  
-- Found a [potential microcomputer](http://www.ti.com/product/CC2541) with bluetooth 4 integrated into the system, plenty of chinese vendors selling it on aliexpress  
-- Discovered that the N64 controller protocol can be approximated with UART, will probably write assembly to do the pin polling instead  

**April 11th, 2019:**  
-- Figured out how to read directly from the digital pin registers  

**April 9th, 2019:**  
-- Digging into arduino source code to speed up function calls  

**April 6th, 2019:**  
-- Added Arduino code for early testing of communication with N64 controller  
-- Able to send command byte 0x00 to controller  
-- Able to see that data is being sent back, still need latch it properly  

---  