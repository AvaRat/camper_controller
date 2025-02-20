#include <stdint.h>

    /**
     * @brief Error codes
     * @{
     */
    enum Error_t
    {
        OK                   =  0,   /**< No error */
        INTF_ERROR           = -1,   /**< Interface error */
        CONF_ERROR           = -2,   /**< Configuration error */
        READ_ERROR           = -3,   /**< Read error */
        WRITE_ERROR          = -4,   /**< Write error */
        NULLPTR_ERROR        = -5,   /**< Null pointer error */
        INVALID_CH_ERROR     = -6,   /**< Invalid channel error */
        UNSUPPORTED_OP_ERROR = -7,   /**< Invalid channel error */
        INIT_ERROR           = -8    /**< Not initialized */
    };

class GPIOPAL
{
    public:

        /**
         * @brief   Interrupt event
         */
        enum IntEvent_t
        {
            INT_FALLING_EDGE   = 0,     /**< Interrupt on falling edge */
            INT_RISING_EDGE    = 1,     /**< Interrupt on rising edge */
        };

        /**
         * @brief   Voltage level
         */
        enum VLevel_t
        {
            GPIO_LOW   = 0,        /**< Level low */
            GPIO_HIGH  = 1         /**< Level high */
        };

        /**
         * @brief   Voltage logic
         */
        enum VLogic_t
        {
            NEGATIVE = 1,       /**< Negative logic. 0 on voltage level high, 1 on voltage low */
            POSITIVE = 2,       /**< Positive logic. 1 on voltage level high, 0 on voltage low */
        };

        /**
         * @brief       Initializes the GPIO
         * @return High-side switch error code 
         * @retval      OK          If success
         * @retval      INIT_ERROR  If initialization error
         */
        virtual Error_t        init        () = 0;

        /**
             * @brief       Deinitializes the GPIO
             * @return High-side switch error code 
             * @retval      OK          If success
             * @retval      INIT_ERROR  If deinitialization error
         */
        virtual Error_t       deinit      () = 0;

        /**
         * @brief       Reads the GPIO voltage level
         * @return      VLevel_t
         * @retval      GPIO_LOW    If voltage low
         * @retval      GPIO_HIGH   If voltage high
         */
        virtual VLevel_t       read        () = 0;

        /**
         * @brief       Writes the GPIO output voltage level
         * @param[in]   level  Voltage level
         * @return High-side switch error code 
         * @retval      OK              If success
         * @retval      WRITE_ERROR     If write error
         */
        virtual Error_t        write       (VLevel_t level) = 0;

        /**
         * @brief       Enables the GPIO output according to the GPIO logic
         *              - Low if negative
         *              - High if positive
         * @return High-side switch error code 
         * @retval      OK              If success
         * @retval      WRITE_ERROR     If write error
         */
        virtual Error_t        enable      () = 0;

        /**
         * @brief       Disables the GPIO output according to the GPIO logic
         *              - Low if positive
         *              - High if negative
         * @return High-side switch error code 
         * @retval      OK              If success
         * @retval      WRITE_ERROR     If write error
         */
        virtual Error_t        disable     () = 0;

};
