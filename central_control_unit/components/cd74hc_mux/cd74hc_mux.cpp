
#include "cd74hc_mux.hpp"

using namespace std;

static const char *TAG = "cd74hc_mux";

void int_to_bin_digit(uint8_t in, uint8_t* out)
{
    uint8_t count = 4;
    /* assert: count <= sizeof(int)*CHAR_BIT */
    unsigned int mask = 1U << (count-1);
    int i;
    for (i = 0; i < count; i++) {
        out[i] = (in & mask) ? 1 : 0;
        in <<= 1;
    }
}


Cd74HCMux::Cd74HCMux(esp_io_expander_handle_t _expander_handle, esp_io_expander_pin_num_t s0, esp_io_expander_pin_num_t s1, esp_io_expander_pin_num_t s2, esp_io_expander_pin_num_t s3)   
: expander_handle(_expander_handle), n_channels(16), ch_select_pin_0(s0), ch_select_pin_1(s1), ch_select_pin_2(s2), ch_select_pin_3(s3)
{
    ESP_ERROR_CHECK(esp_io_expander_set_dir(expander_handle, s0 | s1 | s2 | s3, IO_EXPANDER_OUTPUT));
    // enable channel 0 at start
    this_thread::sleep_for(chrono::milliseconds(5));
    ESP_ERROR_CHECK(esp_io_expander_set_level(expander_handle, s0 | s1 | s2 | s3, 0));
}

Cd74HCMux::~Cd74HCMux()
{}

void Cd74HCMux::enable()
{

}
void Cd74HCMux::enable_channel(uint8_t channel)
{

    if((channel > n_channels) | (channel < 0))
    {
        ESP_LOGW(TAG, "NO such channel");
    }

    uint8_t channels[4] = {0,0,0,0}; // ch0
    int_to_bin_digit(channel, channels);
    ESP_LOGI(TAG, "num: %d  array: %d %d %d %d", channel, channels[0], channels[1], channels[2], channels[3]);

    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_io_expander_set_level(expander_handle, ch_select_pin_0, channels[3]));
    this_thread::sleep_for(chrono::milliseconds(5));
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_io_expander_set_level(expander_handle, ch_select_pin_1, channels[2]));
    this_thread::sleep_for(chrono::milliseconds(5));
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_io_expander_set_level(expander_handle, ch_select_pin_2, channels[1]));
    this_thread::sleep_for(chrono::milliseconds(5));
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_io_expander_set_level(expander_handle, ch_select_pin_3, channels[0]));

    ESP_LOGI(TAG, "channel %d enabled", channel);

}