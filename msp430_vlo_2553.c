#include "msp430g2553.h"

int pos = 0;   // Index to PWM's duty cycle table (= brightness)

int main(void)
{
  // Stop watchdog
  WDTCTL = WDTPW + WDTHOLD;

  // Set the clock divider DIVS_3 = /8
  // SMCLK = 1 MHz / 8 = 125 KHz (SLAU144E p.5-15)
  BCSCTL2 |= DIVS_3;

  // use the low freqency VLO clock
  BCSCTL3 |= LFXT1S_2;

  /* Timer A Capture/Compare 0 */
  // PWM period = 12 KHz / 1200 = 10 Hz
  //65535 max
  TACCR0 = 12000;

  // Source Timer A from ACLK (TASSEL_1), up mode (MC_1).
  // Up mode counts up to TACCR0. SLAU144E p.12-20
  TACTL = TASSEL_1 | MC_1;

  // OUTMOD_7 = Reset/set output when the timer counts to TACCR1/TACCR0
  // CCIE = Interrupt when timer counts to TACCR1
  //TACCTL1 = OUTMOD_7 | CCIE;
  TACCTL1 = OUTMOD_7;
  TACCTL0 = CCIE;
  TACCR1 = 0;

  // Make P1.6 (green led) an output. SLAU144E p.8-3
  P1DIR |= BIT6;

  // P1.6 = TA0.1 (timer A's output). SLAS694C p.41
  P1SEL |= BIT6;
  // LPM0 (shut down the CPU) with interrupts enabled
  __bis_SR_register(CPUOFF | GIE);

  return 0;
}

// This will be called when timer counts to TACCR1.
// Timer A0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A0 (void)
{
  if (pos < 12000) {
	pos += 1000;
	TACCR1 = pos;
  }
  else if (pos < 24000) {
	pos += 1000;
	TACCR1 = 24000 - pos;
  }
  else {
    pos = 0;
  }
  // Clear interrupt flag
  TACCTL1 &= ~CCIFG;
}
