#include "Servo.h"

#ifdef TARGET_HC32F46x
#if HAS_SERVOS

static uint8_t servoCount = 0;
static MarlinServo *servos[NUM_SERVOS] = {0};

constexpr uint32_t servoDelays[] = SERVO_DELAY;
static_assert(COUNT(servoDelays) == NUM_SERVOS, "SERVO_DELAY must be an array NUM_SERVOS long.");

//
// MarlinServo impl
//
MarlinServo::MarlinServo()
{
    this->channel = servoCount++;
    servos[this->channel] = this;
}

int8_t MarlinServo::attach(const pin_t apin)
{
    // use last pin if pin not given
    if (apin >= 0)
    {
        this->pin = apin;
    }

    // if attached, do nothing but no fail
    if (this->servo.attached())
    {
        return 0;
    }

    // attach
    uint8_t rc = this->servo.attach(this->pin);
    return rc == INVALID_SERVO ? -1 : rc;
}

void MarlinServo::detach()
{
    this->servo.detach();
}

bool MarlinServo::attached()
{
    return this->servo.attached();
}

void MarlinServo::write(servo_angle_t angle)
{
    this->angle = angle;
    this->servo.write(angle);
}

void MarlinServo::move(servo_angle_t angle)
{
    // attach with pin=-1 to use last pin attach() was called with
    if (attach(-1) < 0)
    {
        // attach failed
        return;
    }

    write(angle);
    safe_delay(servoDelays[this->channel]);
    TERN_(DEACTIVATE_SERVOS_AFTER_MOVE, detach());
}

servo_angle_t MarlinServo::read()
{
    return TERN(OPTIMISTIC_SERVO_READ, this->angle, this->servo.read());
}

#endif // HAS_SERVOS
#endif // TARGET_HC32F46x