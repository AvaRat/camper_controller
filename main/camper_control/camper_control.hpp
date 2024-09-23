

class CamperControl{
    public:
    CamperControl();



    private:



    float dc_loads_enabled;
    float temp_inside;
    float temp_outside;
    float hause_battery_level;
    float water_tank_level;


    float hause_battery_voltage;
    float start_battery_voltage;

};



class TemperatureSensor{
    public:
    TemperatureSensor();

    private:
    float get_current_reading();
};