#include "led_indicators.h"

static const char *TAG = "led_indicators";

//using namespace std::placeholders; 


LedStripDriver::LedStripDriver(gpio_num_t gpio_num, uint16_t n_leds): strip_length(n_leds)
{
    // LED strip general initialization, according to your led board design
    strip_config = {
        .strip_gpio_num = gpio_num, // The GPIO that connected to the LED strip's data line
        .max_leds = n_leds,      // The number of LEDs in the strip,
        .led_model = LED_MODEL_WS2812,        // LED strip model
        .color_component_format = LED_STRIP_COLOR_COMPONENT_FMT_GRB, // The color order of the strip: GRB
        .flags = {
            .invert_out = false, // don't invert the output signal
        }
    };

    // LED strip backend configuration: RMT
    rmt_config = {
        .clk_src = RMT_CLK_SRC_DEFAULT,        // different clock source can lead to different power consumption
        .resolution_hz = RMT_LED_STRIP_RESOLUTION_HZ, // RMT counter clock frequency
        .mem_block_symbols = 64,               // the memory size of each RMT channel, in words (4 bytes)
        .flags = {
            .with_dma = false, // DMA feature is available on chips like ESP32-S3/P4
        }
    };

    for(auto led_index=0; led_index<strip_length; led_index++)
    {
       // LedIndicator::led_driver_call_t fun_call1 = std::bind(set_pixel, _1, _2, _3, _4);
        LedIndicator::led_driver_call_t fun_call = [&] (uint8_t index, uint8_t r, uint8_t g, uint8_t b)
        {   
            set_pixel(index, r, g, b);
        };
        leds.push_back(new LedIndicator(led_index, 100, fun_call));
    }
}

vector<LedIndicator*> LedStripDriver::get_all_indicators(){
    return leds;
}

void LedStripDriver::init_hardware(){
    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &strip_handle));
    ESP_LOGI(TAG, "Created LED strip object with RMT backend");

    // leds = vector<LedIndicator*>();
    // //led_strip_set_pixel(led_strip_handle_t strip, uint32_t index, uint32_t red, uint32_t green, uint32_t blue);
    // // store a lambda
    // std::function<void()> f_display_42 = []() { 
    //     cout << 42; 
    //     //led_strip_set_pixel(led_strip_handle_t strip, uint32_t index, uint32_t red, uint32_t green, uint32_t blue)
    //     };
    // f_display_42();
}

void LedStripDriver::set_pixel(uint32_t index, uint32_t r, uint32_t g, uint32_t b)
{   
    led_strip_set_pixel(strip_handle, index, r, g, b);
}


void LedStripDriver::update_all_leds()
{
    //ESP_LOGI(TAG, "Led strip updated");
    led_strip_refresh(strip_handle);
}


LedStripDriver::~LedStripDriver(){
    for(auto led : leds)
    {
        delete led;
    }
    led_strip_del(strip_handle);
}

LedIndicator::LedIndicator( uint32_t index, uint16_t default_brightness, led_driver_call_t driver_call_):   
    pixel_index(index), 
    color(Color::black()), 
    brightness(default_brightness),
    led_driver_call(driver_call_)
{
    flash_timer = new idf::esp_timer::ESPTimer([&]() {toggle();});
}

LedIndicator::LedIndicator(led_driver_call_t driver_call): 
    pixel_index(0),
    color(Color::black()), 
    brightness(0), 
    led_driver_call(driver_call)
{
    function<void()> toggle = [&]() {this->toggle();};
    flash_timer = new idf::esp_timer::ESPTimer(toggle);
}

LedIndicator::~LedIndicator() {
    delete flash_timer;
}


//TODO handle brightness
void LedIndicator::on()
{
    //ESP_LOGI(TAG, "led-%d ON", (int) this->pixel_index);
    //led_strip_set_pixel()

    uint8_t r = (uint8_t) (brightness * color.r / 100);
    uint8_t g = (uint8_t) (brightness * color.g / 100);
    uint8_t b = (uint8_t) (brightness * color.b / 100);

    led_driver_call(pixel_index, r, g, b);
    is_on = 1;
}

void LedIndicator::off()
{
    //ESP_LOGI(TAG, "led-%d OFF", (int) this->pixel_index);
    //led_strip_set_pixel()

    led_driver_call(pixel_index, 0, 0, 0);
    is_on = 0;
}

void LedIndicator::toggle(){
    is_on ? off() : on();
}

void LedIndicator::set_color(Color new_color){
    color = new_color;
}

void LedIndicator::set_brightness(uint16_t brightness_){
    if(0 > brightness_ || brightness_ > 100){
        ESP_LOGW(TAG, "brightness too big");
        return;
    }
    brightness = brightness_;
}

void LedIndicator::flash(uint16_t period_ms){
    ESP_LOGI(TAG, "Setting up timer for led_%d   period: %ds\n", (int)pixel_index, (int)(period_ms));
    flash_timer->start_periodic(chrono::microseconds(period_ms*1000));
}