#include <iostream>
#include <thread>
#include <chrono>
#include <math.h>
#include "hx711.hpp"

using namespace std;

int main (int argc, char *argv[])
{
    std::cout << "RX711 Based Load Cell" << endl << endl;

/*    Gpio pin("6");

    pin.Enable();
    pin.Mode(OUT);
    pin.Write(1);

    cout << flush;

    std::this_thread::sleep_for(std::chrono::milliseconds(3000));

    pin.Write(0);

    pin.Disable();*/

    HX711 scale("203","6", 128);

    std::cout << "Raw reading" << endl;
    std::cout << scale.read_average(10) << endl;
    std::cout << scale.read_average(10) << endl;
    std::cout << scale.read_average(10) << endl;
    std::cout << scale.read_average(10) << endl;
    std::cout << "Finished Read" << endl;

    scale.tare();

    scale.set_scale(12611.75);

    for (int i = 0; i < 500; i++)
    {
        float val = scale.get_units(5);
        val = floor(val*100.00 + 0.5) / 100.00;
        cout << val << " oz." << endl;
        //std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    std::cout << endl << endl;

    return 0;
}
