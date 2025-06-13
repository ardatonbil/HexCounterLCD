#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void init_display();
void init_timer0();

volatile uint8_t lcdptr=0, lcdcmd=0xC0, lcdoff=0;  

uint8_t count = 0;
uint8_t count2 = 0;

unsigned char LCDline[32]={"EE308 Board v3.0Count -------> 0"};
const char LookUp[16] ={"0123456789ABCDEF"};

int main(void)
{   
    //_delay_ms(500);
    init_display();
    init_timer0();  // initialize timer0
    init_INT7();
    
    sei();  // enable global interrupts
    // loop forever
    while(1)
    {
    }
}

void init_display()
{
    DDRA = 0xFF;
    
    PORTC &= 0xAF;
    DDRC  |= 0x50;
    
    PORTC &= 0xBF;

    PORTA = 0x38;
    PORTC |= 0x10;
    PORTC &= 0xEF;
    PORTA = 0x0C;
    PORTC |= 0x10;
    PORTC &= 0xEF;
    PORTA = 0x01;
    PORTC |= 0x10;;
    PORTC &= 0xEF;  
    _delay_ms(2);

    PORTA = 0x38;
    PORTC |= 0x10;
    PORTC &= 0xEF;
    _delay_ms(2);
    PORTA = 0x0C;
    PORTC |= 0x10;
    PORTC &= 0xEF;
    _delay_ms(2);
    PORTA = 0x01;
    PORTC |= 0x10;;
    PORTC &= 0xEF;  
    _delay_ms(2);
}

void init_timer0()
{
    TCCR0B |= (1<<CS02)|(1<<CS00);	// prescaler 1024
    TCNT0 = 0;			            // initialize counter
    TIMSK0 |= (1 << TOIE0);	        // enable overflow interrupt 
}

ISR(TIMER0_OVF_vect)
{
    if(lcdptr==16) {
        PORTC &= 0xBF;
        PORTA = lcdcmd;
        lcdptr = 0;
        lcdcmd ^= 0x40;
        lcdoff ^= 0x10;        
    }
    else {
        PORTC |= 0x40;
        PORTA = LCDline[lcdoff+lcdptr];
        PORTC |= 0x10;
        lcdptr++;
    }
    PORTC |= 0x10;
    PORTC &= 0xEF;
}

void init_INT7()
{
    EICRB |= (1<<ISC71)|(1<<ISC70); // INT7 on rising edge
    EIMSK |= (1<<INT7);             // enable INT7 - PORTE<7>
}

ISR(INT7_vect)
{
    LCDline[31] = LookUp[(count++)&0x0F];
    LCDline[30] = LookUp[(count2)&0x0F];
    if (count % 16 == 0) count2++;
}



