// Include files
#include "uart.h"
#include "sysreg.h"
#include "gpio.h"
#include "irq.h"


// Function prototypes

void init_GPIO4_output();
void set_GPIO4();
void clear_GPIO4();
void init_GPIO17_output();
void set_GPIO17();
void clear_GPIO17();
void init_GPIO27_output();
void set_GPIO27();
void clear_GPIO27();
void init_GPIO22_input();
void init_GPIO23_input();

// Declare a global shared variable
unsigned int sharedValue;



////////////////////////////////////////////////////////////////////////////////
//
//  Function:       main
//
//  Arguments:      none
//
//  Returns:        void
//
//  Description:    This function first start in state 1. When Switch A is
//  pushed, an interrupt service routine (ISR) should put your program into
//  State 2. When Switch B is pushed, the ISR should put your program into
//  State 1. State 1: LEDs are illuminated one after the other in the pattern:
//  1, 2, 3, 1, 2, 3, etc. Each will be on for about 0.5 second.
//  State 2: LEDs will be turned on one after the other in the pattern:
//  3, 2, 1, 3, 2, 1, etc. Each will be lit for about 0.25 second. In other
//  words, the pattern will be twice as fast and in reverse order compared
//  to State 1.
////////////////////////////////////////////////////////////////////////////////

void main()
{
    unsigned int r; 
    //Initialize the sharedValue global variable and
    //and set the local variable to be same value
    sharedValue = 0;
    //Set up the UART serial port
    uart_init();
    //Set up inputs and so that they triggers
    //an interrupt when a rising edge is detected
    init_GPIO23_input();
    init_GPIO22_input();

    // Set up GPIO pin #4 #22and #27 to output
    init_GPIO4_output();
    init_GPIO17_output();
    init_GPIO27_output();

    //Enable IRQ Exceptions
    enableIRQ();    
    
    //Loop forever, waiting for interrupts to change the shared value
    while (1) {

        //When sharedValue equal to zero stay in state 1
        if(sharedValue == 0){
            //4 light on
            // Turn on the LED the LED
            set_GPIO4();
            
            // Print a message to the console
            uart_puts("first ON\n");

            // Delay using a busy loop
            r = 0x003FFFFF/2;
            while (r--) {
                asm volatile("nop");
            }

            // Turn the LED off
            clear_GPIO4();

            // Print a message to the console
            uart_puts("first OFF\n");

            //Turn on the LED
            set_GPIO17();

            //Delay 0.5s using a busy loop
            r = 0x003FFFFF/2;
            while (r--) {asm volatile("nop");
            }

            //Turn the LED off
            clear_GPIO17();
            
            // Turn on the LED 
            set_GPIO27();

            //Delay 0.5s using a busy loop
            r = 0x003FFFFF/2;

            while (r--) {asm volatile("nop");
            }

            //Turn the LED off
            clear_GPIO27();
        }

        //When sharedValue equal to 1 in state 2
        if (sharedValue == 1 ){

            //Turn on the LED
            set_GPIO27();
            //Delay 0.25s using a busy loop

            r = 0x003FFFFF/4;
            while (r--) {asm volatile("nop");
            }

            //Turn the LED off
            clear_GPIO27();

            //Turn on the LED 
            set_GPIO17();

            //Delay 0.25s using a busy loop
            r = 0x003FFFFF/4;
            while (r--) {asm volatile("nop");
            }

            //Turn the LED off
            clear_GPIO17();

            //Turn on the LED
            set_GPIO4();

            //Delay 0.25s using a busy loop
            r = 0x003FFFFF/4;
            while (r--) {asm volatile("nop");
            }

            //Turn the LED off
            clear_GPIO4();
        }

    }
}

////////////////////////////////////////////////////////////////////////////////
//
//  Function:       init_GPIO22_input
//
//  Arguments:      none
//
//  Returns:        void
//
//  Description:    This function sets GPIO pin 22 to an input pin without
//                  any internal pull-up or pull-down resistors. Note that
//                  a pull-down (or pull-up) resistor must be used externally
//                  on the bread board circuit connected to the pin. Be sure
//                  that the pin high level is 5V (definitely NOT 5V).
//                  GPIO pin 22 is also set to trigger an interrupt on a
//                  rising edge, and GPIO interrupts are enabled on the
//                  interrupt controller.
//
////////////////////////////////////////////////////////////////////////////////

void init_GPIO22_input()
{
    register unsigned int r;
    
    // Get the current contents of the GPIO Function Select Register 1
    r = *GPFSEL2;

    // Clear bits 6 - 8. This is the field FSEL22, which maps to GPIO pin 22.
    // We clear the bits by ANDing with a 000 bit pattern in the field. This
    // sets the pin to be an input pin
    r &= ~(0x7 << 6);

    // Write the modified bit pattern back to the
    // GPIO Function Select Register 1
    *GPFSEL2 = r;

    // Disable the pull-up/pull-down control line for GPIO pin 22. We follow the
    // procedure outlined on page 101 of the BCM2837 ARM Peripherals manual. We
    // will pull down the pin using an external resistor connected to ground.

    // Disable internal pull-up/pull-down by setting bits 0:1
    // to 00 in the GPIO Pull-Up/Down Register 
    *GPPUD = 0x0;

    // Wait 150 cycles to provide the required set-up time 
    // for the control signal
    r = 150;
    while (r--) {
        asm volatile("nop");
    }

    // Write to the GPIO Pull-Up/Down Clock Register 0, using a 1 on bit 22 to
    // clock in the control signal for GPIO pin 22. Note that all other pins
    // will retain their previous state.
    *GPPUDCLK0 = (0x1 << 22);

    // Wait 150 cycles to provide the required hold time
    // for the control signal
    r = 150;
    while (r--) {
        asm volatile("nop");
    }

    // Clear all bits in the GPIO Pull-Up/Down Clock Register 0
    // in order to remove the clock
    *GPPUDCLK0 = 0;
    
    // Set pin 22 to so that it generates an interrupt on a rising edge.
    // We do so by setting bit 22 in the GPIO Rising Edge Detect Enable
    // Register 0 to a 1 value (p. 97 in the Broadcom manual).
    *GPREN0 |= (0x1 << 22);
    
    // Enable the GPIO IRQS for ALL the GPIO pins by setting IRQ 52
    // GPIO_int[3] in the Interrupt Enable Register 2 to a 1 value.
    // See p. 117 in the Broadcom Peripherals Manual.
    *IRQ_ENABLE_IRQS_2 = (0x1 << 20);
}

////////////////////////////////////////////////////////////////////////////////
//
//  Function:       init_GPIO23_input
//
//  Arguments:      none
//
//  Returns:        void
//
//  Description:    This function sets GPIO pin 23 to an input pin without
//                  any internal pull-up or pull-down resistors. Note that
//                  a pull-down (or pull-up) resistor must be used externally
//                  on the bread board circuit connected to the pin. Be sure
//                  that the pin high level is 5V (definitely NOT 5V).
//                  GPIO pin 23 is also set to trigger an interrupt on a
//                  rising edge, and GPIO interrupts are enabled on the
//                  interrupt controller.
//
////////////////////////////////////////////////////////////////////////////////

void init_GPIO23_input()
{
    register unsigned int r;
    
    
    // Get the current contents of the GPIO Function Select Register 2
    r = *GPFSEL2;

    // Clear bits 9 - 11. This is the field FSEL23, which maps to GPIO pin 23.
    // We clear the bits by ANDing with a 000 bit pattern in the field. This
    // sets the pin to be an input pin
    r &= ~(0x7 << 9);

    // Write the modified bit pattern back to the
    // GPIO Function Select Register 2
    *GPFSEL2 = r;

    // Disable the pull-up/pull-down control line for GPIO pin 23. We follow the
    // procedure outlined on page 101 of the BCM2837 ARM Peripherals manual. We
    // will pull down the pin using an external resistor connected to ground.

    // Disable internal pull-up/pull-down by setting bits 0:1
    // to 00 in the GPIO Pull-Up/Down Register 
    *GPPUD = 0x0;

    // Wait 150 cycles to provide the required set-up time 
    // for the control signal
    r = 150;
    while (r--) {
        asm volatile("nop");
    }

    // Write to the GPIO Pull-Up/Down Clock Register 0, using a 1 on bit 23 to
    // clock in the control signal for GPIO pin 23. Note that all other pins
    // will retain their previous state.
    *GPPUDCLK0 = (0x1 << 23);

    // Wait 150 cycles to provide the required hold time
    // for the control signal
    r = 150;
    while (r--) {
        asm volatile("nop");
    }

    // Clear all bits in the GPIO Pull-Up/Down Clock Register 0
    // in order to remove the clock
    *GPPUDCLK0 = 0;
    
    // Set pin 23 to so that it generates an interrupt on a rising edge.
    // We do so by setting bit 23 in the GPIO Rising Edge Detect Enable
    // Register 0 to a 1 value (p. 97 in the Broadcom manual).
    *GPREN0 |= (0x1 << 23);
    
    // Enable the GPIO IRQS for ALL the GPIO pins by setting IRQ 52
    // GPIO_int[3] in the Interrupt Enable Register 2 to a 1 value.
    // See p. 117 in the Broadcom Peripherals Manual.
    *IRQ_ENABLE_IRQS_2 = (0x1 << 20);
}



////////////////////////////////////////////////////////////////////////////////
//
//  Function:       init_GPIO4_output
//
//  Arguments:      none
//
//  Returns:        void
//
//  Description:    This function sets GPIO pin 4 to an output pin without
//                  any pull-up or pull-down resistors.
//
////////////////////////////////////////////////////////////////////////////////

void init_GPIO4_output()
{
    register unsigned int r;
    
    
    // Get the current contents of the GPIO Function Select Register 0
    r = *GPFSEL0;

    // Clear bits 12 - 14. This is the field FSEL4, which maps to GPIO pin 4.
    // We clear the bits by ANDing with a 000 bit pattern in the field.
    r &= ~(0x7 << 12);

    // Set the field FSEL4 to 001, which sets pin 4 to an output pin.
    // We do so by ORing the bit pattern 001 into the field.
    r |= (0x1 << 12);

    // Write the modified bit pattern back to the
    // GPIO Function Select Register 0
    *GPFSEL0 = r;

    // Disable the pull-up/pull-down control line for GPIO pin 4. We follow the
    // procedure outlined on page 101 of the BCM2837 ARM Peripherals manual. The
    // internal pull-up and pull-down resistor isn't needed for an output pin.

    // Disable pull-up/pull-down by setting bits 0:1
    // to 00 in the GPIO Pull-Up/Down Register 
    *GPPUD = 0x0;

    // Wait 150 cycles to provide the required set-up time 
    // for the control signal
    r = 150;
    while (r--) {
      asm volatile("nop");
    }

    // Write to the GPIO Pull-Up/Down Clock Register 0, using a 1 on bit 4 to
    // clock in the control signal for GPIO pin 4. Note that all other pins
    // will retain their previous state.
    *GPPUDCLK0 = (0x1 << 4);

    // Wait 150 cycles to provide the required hold time
    // for the control signal
    r = 150;
    while (r--) {
      asm volatile("nop");
    }

    // Clear all bits in the GPIO Pull-Up/Down Clock Register 0
    // in order to remove the clock
    *GPPUDCLK0 = 0;
}



////////////////////////////////////////////////////////////////////////////////
//
//  Function:       set_GPIO4
//
//  Arguments:      none
//
//  Returns:        void
//
//  Description:    This function sets the GPIO output pin 4
//                  to a 1 (high) level.
//
////////////////////////////////////////////////////////////////////////////////

void set_GPIO4()
{
	  register unsigned int r;
	  
	  // Put a 1 into the SET23 field of the GPIO Pin Output Set Register 0
	  r = (0x1 << 4);
	  *GPSET0 = r;
}



////////////////////////////////////////////////////////////////////////////////
//
//  Function:       clear_GPIO4
//
//  Arguments:      none
//
//  Returns:        void
//
//  Description:    This function clears the GPIO output pin 4
//                  to a 0 (low) level.
//
////////////////////////////////////////////////////////////////////////////////

void clear_GPIO4()
{
	  register unsigned int r;
	  
	  // Put a 1 into the CLR4 field of the GPIO Pin Output Clear Register 0
	  r = (0x1 << 4);
	  *GPCLR0 = r;
}


////////////////////////////////////////////////////////////////////////////////
//
//  Function:       init_GPIO17_output
//
//  Arguments:      none
//
//  Returns:        void
//
//  Description:    This function sets GPIO pin 17 to an output pin without
//                  any pull-up or pull-down resistors.
//
////////////////////////////////////////////////////////////////////////////////

void init_GPIO17_output()
{
    register unsigned int r;
    
    
    // Get the current contents of the GPIO Function Select Register 1
    r = *GPFSEL1;

    // Clear bits 21 - 23. This is the field FSEL17, which maps to GPIO pin 17.
    // We clear the bits by ANDing with a 000 bit pattern in the field.
    r &= ~(0x7 << 21);

    // Set the field FSEL17 to 001, which sets pin 17 to an output pin.
    // We do so by ORing the bit pattern 001 into the field.
    r |= (0x1 << 21);

    // Write the modified bit pattern back to the
    // GPIO Function Select Register 0
    *GPFSEL1 = r;

    // Disable the pull-up/pull-down control line for GPIO pin 17. We follow the
    // procedure outlined on page 101 of the BCM2837 ARM Peripherals manual. The
    // internal pull-up and pull-down resistor isn't needed for an output pin.

    // Disable pull-up/pull-down by setting bits 0:1
    // to 00 in the GPIO Pull-Up/Down Register 
    *GPPUD = 0x0;

    // Wait 150 cycles to provide the required set-up time 
    // for the control signal
    r = 150;
    while (r--) {
      asm volatile("nop");
    }

    // Write to the GPIO Pull-Up/Down Clock Register 0, using a 1 on bit 17 to
    // clock in the control signal for GPIO pin 17. Note that all other pins
    // will retain their previous state.
    *GPPUDCLK0 = (0x1 << 17);

    // Wait 150 cycles to provide the required hold time
    // for the control signal
    r = 150;
    while (r--) {
      asm volatile("nop");
    }

    // Clear all bits in the GPIO Pull-Up/Down Clock Register 0
    // in order to remove the clock
    *GPPUDCLK0 = 0;
}



////////////////////////////////////////////////////////////////////////////////
//
//  Function:       set_GPIO17
//
//  Arguments:      none
//
//  Returns:        void
//
//  Description:    This function sets the GPIO output pin 17
//                  to a 1 (high) level.
//
////////////////////////////////////////////////////////////////////////////////

void set_GPIO17()
{
	  register unsigned int r;
	  
	  // Put a 1 into the SET17 field of the GPIO Pin Output Set Register 0
	  r = (0x1 << 17);
	  *GPSET0 = r;
}



////////////////////////////////////////////////////////////////////////////////
//
//  Function:       clear_GPIO17
//
//  Arguments:      none
//
//  Returns:        void
//
//  Description:    This function clears the GPIO output pin 17
//                  to a 0 (low) level.
//
////////////////////////////////////////////////////////////////////////////////

void clear_GPIO17()
{
	  register unsigned int r;
	  
	  // Put a 1 into the CLR17 field of the GPIO Pin Output Clear Register 0
	  r = (0x1 << 17);
	  *GPCLR0 = r;
}



////////////////////////////////////////////////////////////////////////////////
//
//  Function:       init_GPIO27_output
//
//  Arguments:      none
//
//  Returns:        void
//
//  Description:    This function sets GPIO pin 27 to an output pin without
//                  any pull-up or pull-down resistors.
//
////////////////////////////////////////////////////////////////////////////////

void init_GPIO27_output()
{
    register unsigned int r;
    
    
    // Get the current contents of the GPIO Function Select Register 2
    r = *GPFSEL2;

    // Clear bits 21 - 23. This is the field FSEL27, which maps to GPIO pin 27.
    // We clear the bits by ANDing with a 000 bit pattern in the field.
    r &= ~(0x7 << 21);

    // Set the field FSEL27 to 001, which sets pin 27 to an output pin.
    // We do so by ORing the bit pattern 001 into the field.
    r |= (0x1 << 21);

    // Write the modified bit pattern back to the
    // GPIO Function Select Register 2
    *GPFSEL2 = r;

    // Disable the pull-up/pull-down control line for GPIO pin 27. We follow the
    // procedure outlined on page 101 of the BCM2837 ARM Peripherals manual. The
    // internal pull-up and pull-down resistor isn't needed for an output pin.

    // Disable pull-up/pull-down by setting bits 0:1
    // to 00 in the GPIO Pull-Up/Down Register 
    *GPPUD = 0x0;

    // Wait 150 cycles to provide the required set-up time 
    // for the control signal
    r = 150;
    while (r--) {
      asm volatile("nop");
    }

    // Write to the GPIO Pull-Up/Down Clock Register 0, using a 1 on bit 27 to
    // clock in the control signal for GPIO pin 27. Note that all other pins
    // will retain their previous state.
    *GPPUDCLK0 = (0x1 << 27);

    // Wait 150 cycles to provide the required hold time
    // for the control signal
    r = 150;
    while (r--) {
      asm volatile("nop");
    }

    // Clear all bits in the GPIO Pull-Up/Down Clock Register 0
    // in order to remove the clock
    *GPPUDCLK0 = 0;
}



////////////////////////////////////////////////////////////////////////////////
//
//  Function:       set_GPIO27
//
//  Arguments:      none
//
//  Returns:        void
//
//  Description:    This function sets the GPIO output pin 27
//                  to a 1 (high) level.
//
////////////////////////////////////////////////////////////////////////////////

void set_GPIO27()
{
	  register unsigned int r;
	  
	  // Put a 1 into the SET27 field of the GPIO Pin Output Set Register 0
	  r = (0x1 << 27);
	  *GPSET0 = r;
}



////////////////////////////////////////////////////////////////////////////////
//
//  Function:       clear_GPIO27
//
//  Arguments:      none
//
//  Returns:        void
//
//  Description:    This function clears the GPIO output pin 27
//                  to a 0 (low) level.
//
////////////////////////////////////////////////////////////////////////////////

void clear_GPIO27()
{
	  register unsigned int r;
	  
	  // Put a 1 into the CLR27 field of the GPIO Pin Output Clear Register 0
	  r = (0x1 << 27);
	  *GPCLR0 = r;
}
