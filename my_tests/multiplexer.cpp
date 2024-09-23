#include<iostream>
#include <bitset>

using namespace std;

class AnalogMultiplexer{
/*
Analog channel based on CD74 multiplexer
*/
    public:
    AnalogMultiplexer(){};

    void select_channel(int channel){
        if(channel < 1 | channel > 15){
            return; //throw some exception or use enum to check for correctness during compilation
        }
        uint32_t __builtin_bswap32 (uint32_t x);
        cout << "channel " << channel << " selected" << endl;
        cout << "pins: " << S0_pin << " " << S1_pin << " " << S2_pin << " " << S3_pin << endl;
        bitset<4> y(channel);
        cout << "binary: " << y << endl;
        cout << "set pin s3_pin: " << "to " << y[3] << endl;
        cout << "set pin s2_pin: " << "to " << y[2] << endl;
        cout << "set pin s1_pin: " << "to " << y[1] << endl;
        cout << "set pin s0_pin: " << "to " << y[0] << endl;
    };

    private:
    int S0_pin=0, S1_pin=0, S2_pin=0, S3_pin=0;
   // int gpios[] = {s3_pin, s2_pin, s1_pin, s0_pin};
    int available_channels[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

};

int main(void){
    AnalogMultiplexer am;

    am.select_channel(5);
    return 0;
}