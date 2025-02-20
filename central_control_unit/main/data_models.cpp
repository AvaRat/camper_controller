#include <iostream>
#include <stdint.h>
#include <string.h>
#include <map>

using namespace std;


#define ID_SLAVE_DATA 0x010
class CcuDataModel{
    uint8_t data[8]; 

};

typedef struct {
    union {
        struct {
            //The order of these bits must match deprecated message flags for compatibility reasons
            uint32_t extd: 1;           /**< Extended Frame Format (29bit ID) */
            uint32_t rtr: 1;            /**< Message is a Remote Frame */
            uint32_t ss: 1;             /**< Transmit as a Single Shot Transmission. Unused for received. */
            uint32_t self: 1;           /**< Transmit as a Self Reception Request. Unused for received. */
            uint32_t dlc_non_comp: 1;   /**< Message's Data length code is larger than 8. This will break compliance with ISO 11898-1 */
            uint32_t reserved: 27;      /**< Reserved bits */
        };
        //Todo: Deprecate flags
        uint32_t flags;                 /**< Deprecated: Alternate way to set bits using message flags */
    };
    uint32_t identifier;                /**< 11 or 29 bit identifier */
    uint8_t data_length_code;           /**< Data length code */
    uint8_t data[8];    /**< Data bytes (not relevant in RTR frame) */
} twai_message_t;


//Data bytes of data message will be initialized in the transmit task
static twai_message_t data_message = {.identifier = ID_SLAVE_DATA, .data_length_code = 4,
    .data = {0, 0 , 0 , 0 ,0 ,0 ,0 ,0}};

struct can_data_field_t{
    uint32_t frame_id;
    uint32_t value;
};

class CamperState{
    public:
    void serialize_to_can_frame(){
    
        
    };
    
    float house_battery_lvl; // 0 - 32 V
    float house_battery_voltage; // 0 - 32 V
    float house_battery_current; // 0 - 256 V

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

    float water_tank_lvl; //0-100 %
    float temp_inside;
    float temp_outside;
};

void int32_2_frame(int32_t value, uint8_t *data_frame) {
    if (data_frame == NULL) {
        return; // Prevent dereferencing NULL pointer
    }
    
    // Store value in little-endian format
    data_frame[0] = (uint8_t)(value & 0xFF);
    data_frame[1] = (uint8_t)((value >> 8) & 0xFF);
    data_frame[2] = (uint8_t)((value >> 16) & 0xFF);
    data_frame[3] = (uint8_t)((value >> 24) & 0xFF);
    
    // Set remaining bytes to zero
    data_frame[4] = 0;
    data_frame[5] = 0;
    data_frame[6] = 0;
    data_frame[7] = 0;
}

int32_t frame_2_int32(const uint8_t *data_frame) {
    if (data_frame == NULL) {
        return -1; // Prevent dereferencing NULL pointer
    }
    
    // Reconstruct value from little-endian format
    int32_t value = (int32_t)data_frame[0] |
                    ((int32_t)data_frame[1] << 8) |
                    ((int32_t)data_frame[2] << 16) |
                    ((int32_t)data_frame[3] << 24);
    
    return value;
}

void float_2_frame(float value, uint8_t *data_frame) {
    if (data_frame == NULL) {
        return; // Prevent dereferencing NULL pointer
    }
    
    // Copy float bytes into the frame
    memcpy(data_frame, &value, sizeof(float));
    
    // Set remaining bytes to zero
    data_frame[4] = 0;
    data_frame[5] = 0;
    data_frame[6] = 0;
    data_frame[7] = 0;
}

float frame_2_float(const uint8_t *data_frame) {
    if (data_frame == NULL) {
        return 0.0f; // Handle NULL pointer case
    }
    
    // Reconstruct float from bytes
    float value;
    memcpy(&value, data_frame, sizeof(float));
    
    return value;
}

// int main(void){

//     uint8_t can_data_frame[8]; 

//     float x = 155.5;
//     int32_t i = 13670; // 13.670V
//     int32_2_frame(i, can_data_frame);
//     printf("int data : %d\n", frame_2_int32(can_data_frame));

//     float_2_frame(x, can_data_frame);
//     printf("float data received : %f\n", frame_2_float(can_data_frame));
//     return 0;
// }