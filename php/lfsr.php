<?php

$num_bits = 60;
$taps = [0, 2, 3, 5];

//$num_bits = 7;
//$taps = [0, 6];

// Seed the register.
$lfsr = 1 << $num_bits - 1;

function shift($lfsr, $taps, $num_bits) {

  // Taps (2 rr 4) exclusive ORed together.
  if (count($taps) == 2) {
    $bit = (
      ($lfsr >> $taps[0]) ^ ($lfsr >> $taps[1])
    ) & 1;
  }
  else {
    $bit = (
      ($lfsr >> $taps[0]) ^ ($lfsr >> $taps[1]) ^ ($lfsr >> $taps[2]) ^ ($lfsr >> $taps[3])
    ) & 1;
  }

  //$bit = ($lfsr >> 0) & 1;

  // Shift right, removing the least significant bit.
  $lfsr = $lfsr >> 1;

  // Add the new bit to the start of the register.
  if ($bit) {
    $lfsr |= 1 << ($num_bits - 1);
  }
  else {
    $lfsr &= ~(1 << ($num_bits - 1));
  }

  return $lfsr;
}

while (1) {
  // Get the last 8 bits
  $num = 0xFF & $lfsr;
  printf("%'0" . $num_bits . "b : %d\n", $lfsr, $num);
  $lfsr = shift($lfsr, $taps, $num_bits);
  usleep(100000);
}
