#include "../platforms.h"

#ifdef TARGET_HC32F46x

#include "../../inc/MarlinConfig.h"

#if HAS_SERVOS

#include <Servo.h>

/**
 * return last written value in servo.read instead of calculated value
 */
#define OPTIMISTIC_SERVO_READ

/**
 * @brief servo lib wrapper for marlin
 */
class MarlinServo
{
public:
    MarlinServo();

    /**
     * @brief attach the pin to the servo, set pin mode, return channel number
     * @param pin pin to attach to
     * @return channel number, -1 if failed
     */
    int8_t attach(const pin_t apin);

    /**
     * @brief detach servo
     */
    void detach();

    /**
     * @brief is servo attached?
     */
    bool attached();

    /**
     * @brief set servo angle
     * @param angle new angle
     */
    void write(servo_angle_t angle);

    /**
     * @brief attach servo, move to angle, delay then detach
     * @param angle angle to move to
     */
    void move(servo_angle_t angle);

    /**
     * @brief read current angle
     * @return current angle betwwne 0 and 180 degrees
     */
    servo_angle_t read();

private:
    /**
     * @brief internal servo object, provided by arduino core
     */
    Servo servo;

    /**
     * @brief virtual servo channel
     */
    uint8_t channel;

    /**
     * @brief pin the servo attached to last
     */
    pin_t pin;

    /**
     * @brief last known servo angle
     */
    servo_angle_t angle;
};

// alias for marlin HAL
typedef MarlinServo hal_servo_t;

#endif // HAS_SERVOS
#endif // TARGET_HC32F46x