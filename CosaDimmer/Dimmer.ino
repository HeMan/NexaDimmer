#include "Cosa/Rotary.hh"
#include "Cosa/Driver/NEXA.hh"
#include "Cosa/Trace.hh"
#include "Cosa/PWMPin.hh"
#include "Cosa/IOStream/Driver/UART.hh"
#include "Cosa/Periodic.hh"

#define ID 0x34457A3L
#define UNIT 1



#if defined(BOARD_ATTINY)
#define CLK Board::PCI1
#define DT Board::PCI2
#define OUT Board::D3
#else
#define CLK Board::PCI6
#define DT Board::PCI7
#define OUT Board::D9
#endif

class Transmit : public Periodic {
private:
    NEXA::Transmitter transmitter;
    int8_t oldlevel;
    int8_t level;
public:
    Transmit(Board::DigitalPin pin, int32_t id, uint16_t ms=500) : Periodic(ms), transmitter(pin, id), oldlevel(0), level(0) {}
    virtual void run() {
        //trace << "Run" << endl;
        if (level != oldlevel) {
            trace << "Changing " << level << endl;
            transmitter.send(UNIT, level);
            oldlevel = level;
        }
    }   
    void setlevel(int8_t _level) {
        level=_level;
        //trace << "Setting level to " << level << endl;
    }
};
    
Transmit transmitter(OUT, ID);
PWMPin ledPin(Board::PWM0);
Rotary::Dial<int8_t> dial(CLK, DT, Rotary::Encoder::FULL_CYCLE, 0, 0, 15, 1);

void setup()
{
    Watchdog::begin(16, Watchdog::push_timeout_events);
    uart.begin(9600);
    trace.begin(&uart, PSTR("CosaRotaryEncoder: started"));


    PinChangeInterrupt::begin();
    RTC::begin();
    
    transmitter.begin();

}

void loop()
{
    int8_t level = 0;
    Event event;
    Event::queue.await(&event);
    
    event.dispatch();
    //trace << "dispatched" << endl;
    
    level = -dial.get_value();
    transmitter.setlevel(level);
    
/*    transmitter.send(0, level); */
    ledPin.set(-level*16);
    //trace << level << endl;
}
