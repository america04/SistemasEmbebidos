library IEEE;
use IEEE.std_logic_1164.all;

-- Entity declaration

entity LEDtest is

    port
    (
			A : in std_logic;
			Y : out std_logic
    );

end LEDtest;

-- Architecture definition

architecture led_test of LEDtest is

begin

	Y <= A;

end led_test;