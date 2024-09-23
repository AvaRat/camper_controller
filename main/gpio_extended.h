#include "esp_log.h"
#include "esp_io_expander_tca95xx_16bit.h"
#include "driver/i2c.h"
#include "config.h"
#include "gpio_cxx.hpp"
#include <vector>
#include <map>
#include <bitset>
#include <variant>

using namespace std;
using namespace idf;

/*
    explicit I2CMaster(I2CNumber i2c_number,
              SCL_GPIO scl_gpio,
              SDA_GPIO sda_gpio,
              Frequency clock_speed,
              bool scl_pullup = true,
              bool sda_pullup = true);
*/



class ExpanderTCA{
    public:
    ExpanderTCA(i2c_port_t i2c_port, esp_io_expander_tca_95xx_16bit_address i2c_address){
        ESP_ERROR_CHECK(esp_io_expander_new_i2c_tca95xx_16bit(i2c_port, i2c_address, &io_expander)); 
    };

    int set_as_output(uint32_t external_num, uint32_t internal_num){
        if(internal_num > 16){
            return -1;
        }else{
            esp_io_expander_set_dir(io_expander,  expander_pin_arr[internal_num], IO_EXPANDER_OUTPUT);
            external_pin_map[external_num] = expander_pin_arr[internal_num];            
        }
        return 0;
    }
    // <external_num, internal_num>
    int configure_output_GPIOs(std::map<uint32_t, uint32_t> gpios_map){
        if(gpios_map.size() > 16){
            // TODO throw some exception
            return -1; 
        }
        for(auto const& [external_num, internal_num]: gpios_map){
            esp_io_expander_set_dir(io_expander,  expander_pin_arr[internal_num], IO_EXPANDER_OUTPUT);
            external_pin_map[external_num] = expander_pin_arr[internal_num];
        }
        return 0;
    };

    int set_high(uint32_t num){
        if(num)
        esp_io_expander_set_level(io_expander, external_pin_map[num], 1);

        return 0;
    };

    int set_low(uint32_t num){
        esp_io_expander_set_level(io_expander, external_pin_map[num], 0);

        return 0;
    };

    private:    
    esp_io_expander_handle_t io_expander;
    esp_io_expander_pin_num_t expander_pin_arr[16] = {IO_EXPANDER_PIN_NUM_0, IO_EXPANDER_PIN_NUM_1, IO_EXPANDER_PIN_NUM_2, 
        IO_EXPANDER_PIN_NUM_3, IO_EXPANDER_PIN_NUM_4, IO_EXPANDER_PIN_NUM_5, IO_EXPANDER_PIN_NUM_6, IO_EXPANDER_PIN_NUM_7,
        IO_EXPANDER_PIN_NUM_8,IO_EXPANDER_PIN_NUM_9,IO_EXPANDER_PIN_NUM_10,IO_EXPANDER_PIN_NUM_11,IO_EXPANDER_PIN_NUM_12,
        IO_EXPANDER_PIN_NUM_13,IO_EXPANDER_PIN_NUM_14,IO_EXPANDER_PIN_NUM_15
    };
    std::map<uint32_t, esp_io_expander_pin_num_t> external_pin_map;
    
};



class ExtendedOutputGPIOSpace{
    public:
    ExtendedOutputGPIOSpace(vector<uint32_t> nums, ExpanderTCA *expander): gpio_expander(expander){
        uint32_t internal_pin_num = 0;
        for(uint32_t num :nums){
            if(num < first_extended_gpio) {
                output_ios.push_back(GPIO_Output(GPIONum(num)));
                gpio_map[num] = output_ios.size() - 1;
                cout << "setting normal PIN " << num << " as OUTPUT" << endl;
            } else{
                gpio_expander->set_as_output(last_extended_gpio, internal_pin_num);
                last_extended_gpio++;
                internal_pin_num++;
            }
        }
        //initialize GPIOs as OUTPUT

        //initialize GPIOExpander
    };
    void set_output(uint32_t num, bool output_state){
        output_state == 1 ? set_high(num) : set_low(num);
    }

    void set_high(uint32_t num){
        if(num < first_extended_gpio) {
            output_ios[gpio_map[num]].set_high();
        }else  
            gpio_expander->set_high(num);
    };

    void set_high(vector<uint32_t> nums){
        for(auto num: nums){
            this->set_high(num);
        }
    }

    void set_low(uint32_t num){
        if(num < first_extended_gpio) {
            output_ios[gpio_map[num]].set_low();
        }else  
            gpio_expander->set_low(num);
    };

    void set_low(vector<uint32_t> nums){
        for(auto num: nums){
            this->set_low(num);
        }
    }

    private:
    uint32_t first_extended_gpio = 50;
    uint32_t last_extended_gpio = first_extended_gpio;
    //list of extender gpio nums 50, 51, 51, 53, 54, 55, 56, 57
    std::vector<GPIO_Output> output_ios;
    std::map<uint32_t, uint32_t> gpio_map;
    std::vector<GPIOInput> input_ios;
    ExpanderTCA *gpio_expander;
};



#define MULTIPLEXER_CHANNELS 16

class AnalogMultiplexer{
/*
Analog channel based on CD74 multiplexer
*/
    public:
    AnalogMultiplexer(ExtendedOutputGPIOSpace *gpio_handler, uint8_t s0_pin, uint8_t s1_pin, uint8_t s2_pin, uint8_t s3_pin):
        gpio_handler(gpio_handler),S0_pin(s0_pin), S1_pin(s1_pin), S2_pin(s2_pin), S3_pin(s3_pin){};

    uint8_t enableChannel(uint32_t channel){
        if((channel < 0) || (channel >= MULTIPLEXER_CHANNELS)){
            return -1; //throw some exception or use enum to check for correctness during compilation
        }
        cout << "channel " << channel << " enabled" << endl;
        bitset<4> bits(channel);
        gpio_handler->set_output(S3_pin, bits[0]);
        gpio_handler->set_output(S2_pin, bits[1]);
        gpio_handler->set_output(S1_pin, bits[2]);
        gpio_handler->set_output(S0_pin, bits[3]);
        return 0;
    };

    private:
    ExtendedOutputGPIOSpace *gpio_handler;
    uint8_t S0_pin, S1_pin, S2_pin, S3_pin;
};



/*
class AbstractGPIOOutput{
    public: 
    AbstractGPIOOutput(uint32_t num): num(num){
        
        //setup as output
    };
    void set_high(){
        if(is_extended){
            extended_gpio->set_high(num);
        }else
            normal_gpio->set_high();
    }
    void set_low(){
        if(is_extended){
            extended_gpio->set_low(num);
        }else
            normal_gpio->set_low();

    }

    private:
    bool is_extended;
    uint32_t num;
    GPIO_Output *normal_gpio;
};
*/