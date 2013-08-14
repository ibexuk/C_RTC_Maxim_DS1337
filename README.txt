The Dallas Maxim DS1337 is a I2C serial bus real time clock. It provides low power time keeping for embedded designs with two alarm output pins.


This driver provides the following functions:


SET REAL TIME CLOCK TIME
BYTE rtc_set_time (BYTE alarms_enabled, BYTE hours, BYTE minutes, BYTE seconds, BYTE day, BYTE date, BYTE month, BYTE year)
The fields are converted from binary to BCD before being sent to the RTC
alarms_enabled 0 = alarms disabled, bit0 = alarm 1 enable, bit1 = alarm 2 enable, bit2 = alarms trigger INTA & INTB (0=both trigger INTA)
hours 0 – 23
minutes 0 – 59
seconds 0 – 59
day 1 – 7
date 1 – 31
month 1 – 12
year 0 – 99


GET REAL TIME CLOCK TIME
BYTE rtc_get_time (BYTE *hours, BYTE *minutes, BYTE *seconds, BYTE *day, BYTE *date, BYTE *month, BYTE *year)
All fields are converted to binary values
hours 0 – 23
minutes 0 – 59
seconds 0 – 59
day 1 – 7
date 1 – 31
month 1 – 12
year 0 – 99


SET REAL TIME CLOCK ALARM
BYTE rtc_set_alarm (BYTE alarm_id, BYTE alarms_enabled, BYTE hours, BYTE minutes, BYTE seconds, BYTE date_day)
The fields are converted from binary to BCD before being sent to the RTC
alarm_id 1 – 2
alarms_enabled 0 = alarms disabled, bit0 = alarm 1 enable, bit1 = alarm 2 enable, bit2 = alarms trigger INTA & INTB (0=both trigger INTA)
hours 0 – 23
minutes 0 – 59
seconds 0 – 59 (not available for alarm 2)
date_day 1 – 31 (dates) or 101 – 107 (days)
Set any value to 0xff to be ignored in the alarm comparison
Note: After an alarm is activated the INT pin will remain driven low until rtc_clear_alarms() is called


CLEAR REAL TIME CLOCK ALARMS
BYTE rtc_clear_alarms (void)
When an enabled alarm trips the alarm flags must be cleared using this function to reset the INT pin(s).


Please view the sample source code files for full information regarding the driver.