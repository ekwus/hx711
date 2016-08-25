
#include <iostream>
#include "hx711.hpp"
#include "gpio.hpp"

using namespace std;

HX711::HX711(string dout, string pd_sck, uint8_t gain)
    : pinDOUT(dout), pinSCK(pd_sck)
{
    cout << "HX711 Driver" << endl;
    //pinSCK.Enable();
    pinSCK.Mode(OUT);
    pinSCK.Write(LOW);

    //pinDOUT.Enable();
    pinDOUT.Mode(IN);

    set_gain(gain);
}

HX711::~HX711()
{
    pinSCK.Write(HIGH);
    pinSCK.Disable();
    pinDOUT.Disable();
}

bool HX711::is_ready()
{
    return pinDOUT.Read() == LOW;
}

void HX711::set_gain(uint8_t gain)
{
    switch (gain) {
        case 128:       // channel A, gain factor 128
            GAIN = 1;
            break;
        case 64:        // channel A, gain factor 64
            GAIN = 3;
            break;
        case 32:        // channel B, gain factor 32
            GAIN = 2;
            break;
    }

    pinSCK.Write(LOW);
    read();
}

long HX711::read() {
    // wait for the chip to become ready
    while (!is_ready());

    unsigned long value = 0;
    uint32_t data = 0;
    uint32_t filler = 0x00;

    // pulse the clock pin 24 times to read the data
    for (int i = 0; i < 24; i++)
    {
        pinSCK.Write(HIGH);
        pinSCK.Write(LOW);
        int read = pinDOUT.Read();
        data = data << 1;
        //std::cout << data << endl;
        if (read  == 1)
        {
            data++;
        }
    }

    // set the channel and the gain factor for the next reading using the clock pin
    for (unsigned int i = 0; i < GAIN; i++) {
        pinSCK.Write(HIGH);
        pinSCK.Write(LOW);
    }

    // Replicate the most significant bit to pad out a 32-bit signed integer
    if ( data & 0x800000 ) {
        filler = 0xFF000000;
    } else {
        filler = 0x00000000;
    }

//    std::cout << hex << filler << endl;
//    std::cout << hex << data << endl;
    // Construct a 32-bit signed integer
    //data = ~data;
    value = data | filler;

    // ... and add 1
    return static_cast<long>(++value);
}

long HX711::read_average(uint8_t times) {
    long sum = 0;
    for (uint8_t i = 0; i < times; i++) {
        sum += read();
    }
    return sum / times;
}

double HX711::get_value(uint8_t times) {
    return read_average(times) - OFFSET;
}

float HX711::get_units(uint8_t times) {
    return get_value(times) / SCALE;
}

void HX711::tare(uint8_t times) {
    double sum = read_average(times);
    set_offset(sum);
}

void HX711::set_scale(float scale) {
    SCALE = scale;
}

float HX711::get_scale() {
    return SCALE;
}

void HX711::set_offset(long offset) {
    OFFSET = offset;
}

long HX711::get_offset() {
    return OFFSET;
}

void HX711::power_down() {
    pinSCK.Write(LOW);
    pinSCK.Write(HIGH);
}

void HX711::power_up() {
    pinSCK.Write(LOW);
}
