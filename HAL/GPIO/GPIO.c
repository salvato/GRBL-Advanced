/** PORT Configuration
  * D2:	X_STEP_BIT:              PA10
  * D3: Y_STEP_BIT:              PB3
  * D4: Z_STEP_BIT:              PB5
  * D5: X_DIRECTION_BIT:         PB4
  * D6: Y_DIRECTION_BIT:         PB10
  * D7: Z_DIRECTION_BIT:         PA8
  * D8: STEPPERS_DISABLE_BIT:    PA9
  * D9: X_LIMIT_BIT:             PC7
  * D10: Y_LIMIT_BIT:            PB6
  * D11: SPINDLE_PWM:            PA7
  * D12: Z_LIMIT_BIT:            PA6
  * D13: SPINDLE_DIRECTION_BIT:  PA5
  * D14: SPINDLE_ENABLE_BIT:     PB9
  * D15: SAFETY_DOOR_ENABLE_BIT: PB8
  *
  * A0: CONTROL_RESET_BIT:       PA0
  * A1: CONTROL_FEED_HOLD_BIT:   PA1
  * A2: CONTROL_CYCLE_START_BIT: PA4
  * A3: COOLANT_FLOOD_BIT:       PB0
  * A4: COOLANT_MIST_BIT:        PC1
  * A5: PROBE_BIT:               PC0
  */
#include "GPIO.h"
#include "Platform.h"


static void GPIO_InitStepper(void);
static void GPIO_InitProbe(void);
static void GPIO_InitSpindle(void);
static void GPIO_InitLimit(void);
static void GPIO_InitCoolant(void);
static void GPIO_InitSystem(void);


void
GPIO_InitGPIO(char gpio) {
	// Enable peripheral clocks for all GPIOs
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC, ENABLE);

	switch(gpio)
	{
	case GPIO_STEPPER:
		GPIO_InitStepper();
		break;

	case GPIO_PROBE:
		GPIO_InitProbe();
		break;

	case GPIO_SPINDLE:
		GPIO_InitSpindle();
		break;

	case GPIO_LIMIT:
		GPIO_InitLimit();
		break;

	case GPIO_COOLANT:
		GPIO_InitCoolant();
		break;

	case GPIO_SYSTEM:
		GPIO_InitSystem();
		break;

	default:
		break;
	}
}


static void
GPIO_InitStepper(void) {
	GPIO_InitTypeDef GPIO_InitStructure;

    // GPIO Configuration (GPIO PortA):
    // Arduino pin#                     D7               D8                 D2
    GPIO_InitStructure.GPIO_Pin = GPIO_DIR_Z_PIN | GPIO_ENABLE_PIN | GPIO_STEP_X_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

    // GPIO Configuration (GPIO PortB):
    // Arduino pin#                     D3               D5                 D4              D6
    GPIO_InitStructure.GPIO_Pin = GPIO_STEP_Y_PIN | GPIO_DIR_X_PIN | GPIO_STEP_Z_PIN | GPIO_DIR_Y_PIN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}


static void
GPIO_InitProbe(void) {
	GPIO_InitTypeDef GPIO_InitStructure;

    // GPIO Configuration (GPIO PortC):
    // Arduino pin#                     A5
    GPIO_InitStructure.GPIO_Pin = GPIO_PROBE_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIO_PROBE_PORT, &GPIO_InitStructure);
}


static void
GPIO_InitSpindle(void) {
	GPIO_InitTypeDef GPIO_InitStructure;

    // GPIO Configuration (GPIO PortA):
    // Arduino pin#                       D13 (not present on grbl!)
    GPIO_InitStructure.GPIO_Pin = GPIO_SPINDLE_DIR_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIO_SPINDLE_DIR_PORT, &GPIO_InitStructure);

    // GPIO Configuration (GPIO PortB):
    // Arduino pin#                       D14 (not present on grbl!)
    GPIO_InitStructure.GPIO_Pin = GPIO_SPINDLE_ENA_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIO_SPINDLE_ENA_PORT, &GPIO_InitStructure);

    // GPIO Configuration (GPIO PortA):
    // Arduino pin#                       D11
    GPIO_InitStructure.GPIO_Pin = GPIO_SPINDLE_PWM_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIO_SPINDLE_PWM_PORT, &GPIO_InitStructure);

	// Connect timer to pin
    GPIO_PinAFConfig(GPIO_SPINDLE_PWM_PORT, GPIO_SPINDLE_PWM_SOURCE_PIN, GPIO_AF_TIM1);
}


static void
GPIO_InitLimit(void) {
	GPIO_InitTypeDef GPIO_InitStructure;

    // GPIO Configuration (GPIO PortB):
    // Arduino pin#                     D10
    GPIO_InitStructure.GPIO_Pin = GPIO_LIM_Y_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIO_LIM_Y_PORT, &GPIO_InitStructure);

    // GPIO Configuration (GPIO PortC):
    // Arduino pin#                   D9
    GPIO_InitStructure.GPIO_Pin = GPIO_LIM_X_PIN;
    GPIO_Init(GPIO_LIM_X_PORT, &GPIO_InitStructure);

    // GPIO Configuration (GPIO PortA):
    // Arduino pin#                   D12
    GPIO_InitStructure.GPIO_Pin = GPIO_LIM_Z_PIN;
    GPIO_Init(GPIO_LIM_Z_PORT, &GPIO_InitStructure);
}


static void
GPIO_InitCoolant(void) {
	GPIO_InitTypeDef GPIO_InitStructure;

    // GPIO Configuration (GPIO PortB):
    // Arduino pin#                      A3
    GPIO_InitStructure.GPIO_Pin = GPIO_COOL_FLOOD_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIO_COOL_FLOOD_PORT, &GPIO_InitStructure);

    // GPIO Configuration (GPIO PortB):
    // Arduino pin#                      A4
    GPIO_InitStructure.GPIO_Pin = GPIO_COOL_MIST_PIN;
    GPIO_Init(GPIO_COOL_MIST_PORT, &GPIO_InitStructure);
}


static void
GPIO_InitSystem(void) {
	GPIO_InitTypeDef GPIO_InitStructure;

#ifdef ETH_IF
    // W5500 Reset Pin
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_SetBits(GPIOA, GPIO_Pin_15);
#endif

    // GPIO Configuration (GPIO PortA):
    // Arduino pin#                      A0                   A1                   A4
    GPIO_InitStructure.GPIO_Pin = GPIO_CTRL_RST_PIN | GPIO_CTRL_FEED_PIN | GPIO_CTRL_START_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

    // GPIO Configuration (GPIO PortB):
    // Arduino pin#                    D15
    GPIO_InitStructure.GPIO_Pin = GPIO_DOOR_PIN;
    GPIO_Init(GPIO_DOOR_PORT, &GPIO_InitStructure);
}
