#include "esp_common.h"


static const char *TAG_COMMON = "esp_common";

extern lv_subject_t disp_brightness_subject;


void screen_brightness_observer_cb(lv_observer_t *observer, lv_subject_t *subject){
    int brightness = lv_subject_get_int(subject);
    //ledc_set_fade_with_time(LEDC_MODE, LEDC_CHANNEL, brightness, 10);
    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, brightness);
    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
    //ledc_set_duty_and_update(LEDC_MODE, LEDC_CHANNEL, brightness, 1023);
}

void init_observers()
{
    lv_subject_add_observer(&disp_brightness_subject, screen_brightness_observer_cb, NULL);
}


esp_err_t s_example_write_file(const char *path, char *data)
{
    ESP_LOGI(TAG_COMMON, "Opening file %s", path);
    FILE *f = fopen(path, "w");
    if (f == NULL) {
        ESP_LOGE(TAG_COMMON, "Failed to open file for writing");
        return ESP_FAIL;
    }
    fprintf(f, data);
    fclose(f);
    ESP_LOGI(TAG_COMMON, "File written");

    return ESP_OK;
}

esp_err_t s_example_read_file(const char *path)
{
    ESP_LOGI(TAG_COMMON, "Reading file %s", path);
    FILE *f = fopen(path, "r");
    if (f == NULL) {
        ESP_LOGE(TAG_COMMON, "Failed to open file for reading");
        return ESP_FAIL;
    }
    char line[MAX_BTN_IMG_PATH];
    fgets(line, sizeof(line), f);
    fclose(f);

    // strip newline
    char *pos = strchr(line, '\n');
    if (pos) {
        *pos = '\0';
    }
    ESP_LOGI(TAG_COMMON, "Read from file: '%s'", line);

    return ESP_OK;
}

esp_err_t init_sd_card(){
    esp_err_t ret;

    // Options for mounting the filesystem.
    // If format_if_mount_failed is set to true, SD card will be partitioned and
    // formatted in case when mounting fails.
    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
#ifdef CONFIG_EXAMPLE_FORMAT_IF_MOUNT_FAILED
        .format_if_mount_failed = true,
#else
        .format_if_mount_failed = false,
#endif // EXAMPLE_FORMAT_IF_MOUNT_FAILED
        .max_files = 5,
        .allocation_unit_size = 16 * 1024
    };
    sdmmc_card_t *card;
    const char mount_point[] = MOUNT_POINT;
    ESP_LOGI(TAG_COMMON, "Initializing SD card");

    // Use settings defined above to initialize SD card and mount FAT filesystem.
    // Note: esp_vfs_fat_sdmmc/sdspi_mount is all-in-one convenience functions.
    // Please check its source code and implement error recovery when developing
    // production applications.
    ESP_LOGI(TAG_COMMON, "Using SPI peripheral");

    // By default, SD card frequency is initialized to SDMMC_FREQ_DEFAULT (20MHz)
    // For setting a specific frequency, use host.max_freq_khz (range 400kHz - 20MHz for SDSPI)
    // Example: for fixed frequency of 10MHz, use host.max_freq_khz = 10000;
    sdmmc_host_t host = SDSPI_HOST_DEFAULT();

    spi_bus_config_t bus_cfg = {
        .mosi_io_num = PIN_NUM_MOSI,
        .miso_io_num = PIN_NUM_MISO,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 4000,
    };
    ret = spi_bus_initialize(host.slot, &bus_cfg, SDSPI_DEFAULT_DMA);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG_COMMON, "Failed to initialize bus.");
        return ESP_FAIL;
    }

    // This initializes the slot without card detect (CD) and write protect (WP) signals.
    // Modify slot_config.gpio_cd and slot_config.gpio_wp if your board has these signals.
    sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
    slot_config.gpio_cs = PIN_NUM_CS;
    slot_config.host_id = host.slot;

    ESP_LOGI(TAG_COMMON, "Mounting filesystem");
    ret = esp_vfs_fat_sdspi_mount(mount_point, &host, &slot_config, &mount_config, &card);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG_COMMON, "Failed to mount filesystem. "
                     "If you want the card to be formatted, set the CONFIG_EXAMPLE_FORMAT_IF_MOUNT_FAILED menuconfig option.");
        } else {
            ESP_LOGE(TAG_COMMON, "Failed to initialize the card (%s). "
                     "Make sure SD card lines have pull-up resistors in place.", esp_err_to_name(ret));
        }
        return ESP_FAIL;
    }
    ESP_LOGI(TAG_COMMON, "Filesystem mounted");

    // Card has been initialized, print its properties
    sdmmc_card_print_info(stdout, card);

    // Format FATFS
#ifdef CONFIG_EXAMPLE_FORMAT_SD_CARD
    ret = esp_vfs_fat_sdcard_format(mount_point, card);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG_COMMON, "Failed to format FATFS (%s)", esp_err_to_name(ret));
        return ESP_FAIL;
    }

    if (stat(file_foo, &st) == 0) {
        ESP_LOGI(TAG_COMMON, "file still exists");
        return ESP_FAIL;
    } else {
        ESP_LOGI(TAG_COMMON, "file doesnt exist, format done");
    }
#endif // CONFIG_EXAMPLE_FORMAT_SD_CARD


/*
    // All done, unmount partition and disable SPI peripheral
    esp_vfs_fat_sdcard_unmount(mount_point, card);
    ESP_LOGI(TAG_COMMON, "Card unmounted");

    //deinitialize the bus after all devices are removed
    spi_bus_free(host.slot);
*/
    ESP_LOGI(TAG_COMMON, "uSD-Card mounted, ready to use");
    return ESP_OK;
}


esp_err_t init_spiffs(){
    ESP_LOGI(TAG_COMMON, "Initializing SPIFFS");

    esp_vfs_spiffs_conf_t conf = {
      .base_path = "/spiffs",
      .partition_label = "lvgl_assets",
      .max_files = 30,
      .format_if_mount_failed = false
    };

    // Use settings defined above to initialize and mount SPIFFS filesystem.
    // Note: esp_vfs_spiffs_register is an all-in-one convenience function.
    esp_err_t ret = esp_vfs_spiffs_register(&conf);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG_COMMON, "Failed to mount or format filesystem");
        } else if (ret == ESP_ERR_NOT_FOUND) {
            ESP_LOGE(TAG_COMMON, "Failed to find SPIFFS partition");
        } else {
            ESP_LOGE(TAG_COMMON, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
        }
        return ret;
    }
    #ifdef CONFIG_EXAMPLE_SPIFFS_CHECK_ON_START
        ESP_LOGI(TAG_COMMON, "Performing SPIFFS_check().");
        ret = esp_spiffs_check(conf.partition_label);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG_COMMON, "SPIFFS_check() failed (%s)", esp_err_to_name(ret));
            return ret;
        } else {
            ESP_LOGI(TAG_COMMON, "SPIFFS_check() successful");
        }
    #endif

    size_t total = 0, used = 0;
    ret = esp_spiffs_info(conf.partition_label, &total, &used);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG_COMMON, "Failed to get SPIFFS partition information (%s). NOT Formatting...", esp_err_to_name(ret));
       // esp_spiffs_format(conf.partition_label);
        return ret;
    } else {
        ESP_LOGI(TAG_COMMON, "Partition size: total: %d, used: %d", total, used);
    }

    // Check consistency of reported partition size info.
    if (used > total) {
        ESP_LOGW(TAG_COMMON, "Number of used bytes cannot be larger than total. Performing SPIFFS_check().");
        ret = esp_spiffs_check(conf.partition_label);
        // Could be also used to mend broken files, to clean unreferenced pages, etc.
        // More info at https://github.com/pellepl/spiffs/wiki/FAQ#powerlosses-contd-when-should-i-run-spiffs_check
        if (ret != ESP_OK) {
            ESP_LOGE(TAG_COMMON, "SPIFFS_check() failed (%s)", esp_err_to_name(ret));
            return ret;
        } else {
            ESP_LOGI(TAG_COMMON, "SPIFFS_check() successful");
        }
    }

    // Check if destination file exists before renaming
    // struct stat st;
    // if (stat("/spiffs/"FILENAME, &st) == 0) {
    //     ESP_LOGI(TAG_COMMON, FILENAME" file size: %d", (int)st.st_size);
    // }else{
    //     ESP_LOGE(TAG_COMMON, FILENAME" not found");
    // }

    // // All done, unmount partition and disable SPIFFS
    // esp_vfs_spiffs_unregister(conf.partition_label);
    // ESP_LOGI(TAG_COMMON, "SPIFFS unmounted");
    ESP_LOGI(TAG_COMMON, "SPIFFS mounted, ready to use");
    return ESP_OK;
}

