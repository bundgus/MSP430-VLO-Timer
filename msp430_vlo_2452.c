#include "msp430g2452.h"

int main(void)
{
  // Stop watchdog
  WDTCTL = WDTPW + WDTHOLD;

  // Set the clock divider DIVS_3 = /8
  // SMCLK = 12 KHz / 8 = 1.5 KHz
  //BCSCTL2 |= DIVS_3;

  // use the low frequency VLO clock
  BCSCTL3 |= LFXT1S_2;

  /* Timer A Capture/Compare 0 */
  // PWM period = 12 KHz / 1200 = 10 Hz
  //65535 max
  TACCR0 = 48000;

  // Source Timer A from ACLK (TASSEL_1), up mode (MC_1).
  // Up mode counts up to TACCR0. SLAU144E p.12-20
  TACTL = TASSEL_1 | MC_1;

  // OUTMOD_7 = Reset/set output when the timer counts to TACCR1/TACCR0
  // CCIE = Interrupt when timer counts to TACCR1
  //TACCTL1 = OUTMOD_7 | CCIE;
  TACCTL1 = OUTMOD_7;
  TACCTL0 = CCIE;
  TACCR1 = 100;

  // Make P1.6 (green led) an output.
  P1DIR |= BIT6;

  // P1.6 = TA0.1 (timer A's output).
  P1SEL |= BIT6;

  // LPM0 (shut down the CPU) with interrupts enabled
  __bis_SR_register(CPUOFF);

  return 0;
}
