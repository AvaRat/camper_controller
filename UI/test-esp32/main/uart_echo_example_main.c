/* UART Echo Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "esp_log.h"


/**
 * This is an example which echos any data it receives on configured UART back to the sender,
 * with hardware flow control turned off. It does not use UART driver event queue.
 *
 * - Port: configured UART
 * - Receive (Rx) buffer: on
 * - Transmit (Tx) buffer: off
 * - Flow control: off
 * - Event queue: off
 * - Pin assignment: see defines below (See Kconfig)
 */

#define ECHO_TEST_TXD 17
#define ECHO_TEST_RXD 16
#define ECHO_TEST_RTS (UART_PIN_NO_CHANGE)
#define ECHO_TEST_CTS (UART_PIN_NO_CHANGE)

#define ECHO_UART_PORT_NUM      2
#define ECHO_UART_BAUD_RATE     115200
#define ECHO_TASK_STACK_SIZE    2048

static const char *TAG = "UART TEST";
//esp_log_level_set(TAG, ESP_LOG_INFO);   

#define BUF_SIZE (1024)


struct CamperState_t{
    float house_battery_lvl;
    float house_battery_voltage;
    float house_battery_current;

    float start_battery_voltage;
    float start_battery_current;

    float pv_voltage;
    float pv_current;

    float alternator_voltage;
    float alternator_current;

    float dc_loads_voltage;
    float dc_loads_current;

    float inverter_voltage;
    float inverter_current;

    float grid_voltage;
    float grid_current;

    int32_t water_tank_lvl; //0-100 %
    int32_t temp_inside;
    int32_t temp_outside;
};

struct CamperState_t CamperState = {
    100, 12, 10, //house batt
    12.5, 0,    //start batt
    33, 5,      //pv
    0, 0,       //alternator
    13.1, 5,     //dc loads
    11.5, 10,    //inverter
    0, 0,  //grid
    50, 17, 10 //water, T_in, T_out
};

#define SLEEP_PIN 
#define GPIO_OUTPUT_IO_0    18
#define GPIO_OUTPUT_IO_1    19
#define GPIO_OUTPUT_PIN_SEL  ((1ULL<<GPIO_OUTPUT_IO_0) | (1ULL<<GPIO_OUTPUT_IO_1))


void led_indicator(){
            gpio_set_level(GPIO_OUTPUT_IO_0, 1);
            vTaskDelay(500 / portTICK_PERIOD_MS);
            gpio_set_level(GPIO_OUTPUT_IO_0, 0);
            vTaskDelay(100 / portTICK_PERIOD_MS);
            gpio_set_level(GPIO_OUTPUT_IO_0, 1);
            vTaskDelay(100 / portTICK_PERIOD_MS);
            gpio_set_level(GPIO_OUTPUT_IO_0, 0);
            vTaskDelay(100 / portTICK_PERIOD_MS);
            gpio_set_level(GPIO_OUTPUT_IO_0, 1);
            vTaskDelay(100 / portTICK_PERIOD_MS);
            gpio_set_level(GPIO_OUTPUT_IO_0, 0);
}


static void echo_task(void *arg)
{
    /* Configure parameters of an UART driver,
     * communication pins and install the driver */
    uart_config_t uart_config = {
        .baud_rate = ECHO_UART_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    int intr_alloc_flags = 0;

#if CONFIG_UART_ISR_IN_IRAM
    intr_alloc_flags = ESP_INTR_FLAG_IRAM;
#endif

    ESP_ERROR_CHECK(uart_driver_install(ECHO_UART_PORT_NUM, BUF_SIZE * 2, 0, 0, NULL, intr_alloc_flags));
    ESP_ERROR_CHECK(uart_param_config(ECHO_UART_PORT_NUM, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(ECHO_UART_PORT_NUM, ECHO_TEST_TXD, ECHO_TEST_RXD, ECHO_TEST_RTS, ECHO_TEST_CTS));

    // Configure a temporary buffer for the incoming data
    uint8_t *data = (uint8_t *) malloc(BUF_SIZE);

    ESP_LOGI(TAG, "UART ECHO CREATED\n");

    vTaskDelay(1000 / portTICK_PERIOD_MS);
    led_indicator();

    while (1) {
        // Read data from the UART
        int len = uart_read_bytes(ECHO_UART_PORT_NUM, data, (BUF_SIZE - 1), 20 / portTICK_PERIOD_MS);
        // Write data back to the UART
        //uart_write_bytes(ECHO_UART_PORT_NUM, (const char *) data, len);
        if (len) {
            data[len] = '\0';
            ESP_LOGI(TAG, "Recv str: %s", (char *) data);
            //led_indicator();
        }
    }
}



void gpio_setup(){
    //zero-initialize the config structure.
    gpio_config_t io_conf = {};
    //disable interrupt
    io_conf.intr_type = GPIO_INTR_DISABLE;
    //set as output mode
    io_conf.mode = GPIO_MODE_OUTPUT;
    //bit mask of the pins that you want to set,e.g.GPIO18/19
    io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
    //disable pull-down mode
    io_conf.pull_down_en = 1;
    //disable pull-up mode
    io_conf.pull_up_en = 0;
    //configure GPIO with the given settings
    gpio_config(&io_conf);
}


void app_main(void)
{
    gpio_setup();
    xTaskCreate(echo_task, "uart_echo_task", ECHO_TASK_STACK_SIZE, NULL, 10, NULL);


}
