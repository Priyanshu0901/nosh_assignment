
# Assignment 1
Switch & LED Blinking with STM32G070RB.   

The code uses a timer to blink the LED and a switch interrupt to change the blinking frequency. It also utilizes a low-power sleep mode to reduce power consumption while waiting for switch presses.

# Pin Mapping
```shell
A5 -> LED 
A0 -> Switch # configured as an internal pull-up
```
# Explanation
**Blink Frequencies**: The LED blinks at 0.5Hz, 1Hz, or 2Hz, controlled by switch presses.  A 4th press turns the LED off. The cycle repeats after the 4th press.

**Timer (TIM17)**:  TIM17 generates periodic interrupts to toggle the LED. Update_Timer_Period configures the timer's period, thus controlling the blinking frequency.

**Switch Interrupt (EXTI)**: An external interrupt is configured on the switch pin (A0).  HAL_GPIO_EXTI_Falling_Callback is called when the switch is pressed (falling edge).

**Switch Press Handling**: Inside the switch interrupt handler, press_count tracks the number of presses (0-3). A switch statement sets the timer period based on press_count, effectively changing the blink frequency.  Case 0 turns the LED off and stops the timer.

**LED Blinking**: HAL_TIM_PeriodElapsedCallback is called when TIM17's period elapses. It toggles the LED pin, creating the blinking effect.

**Low Power Mode**: low_power_mode puts the microcontroller into SLEEP mode to save power.  HAL_SuspendTick and HAL_ResumeTick ensure the system tick (used for timekeeping) is properly handled during sleep.  PWR_MAINREGULATOR_ON keeps the main regulator on during sleep (important for fast wakeup). PWR_SLEEPENTRY_WFI enters sleep mode and waits for an interrupt (like the switch press) to wake up.

# Testing

Testing was done on a [STM32 Nucleo-G031k8 board](https://www.st.com/en/evaluation-tools/nucleo-g031k8.html).  

**Branch** : test/g031k8

## PinMapping for the test branch

```shell
LED -> C6
SWITCH -> A1
TEST_pin -> A0 # this is connected to the digital logic analyzer
```

