/*
IBEX UK LTD http://www.ibexuk.com
Electronic Product Design Specialists
RELEASED SOFTWARE

The MIT License (MIT)

Copyright (c) 2013, IBEX UK Ltd, http://ibexuk.com

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
//Project Name:		MAXIM DS1337 REAL TIME CLOCK DRIVER


#include "main.h"					//Global data type definitions (see https://github.com/ibexuk/C_Generic_Header_File )
#define RTC_DS1337_C
#include "rtc-ds1337.h"




//**********************************************
//**********************************************
//********** SET REAL TIME CLOCK TIME **********
//**********************************************
//**********************************************
//Returns:
//	1 if sucessful, 0 if not.
//The fields are converted from binary to BCD before being sent to the RTC
//	alarms_enabled	0 = alarms disabled, bit0 = alarm 1 enable, bit1 = alarm 2 enable, bit2 = alarms trigger INTA & INTB (0=both trigger INTA)
//	hours			0 - 23
//	minutes			0 - 59
//	seconds			0 - 59
//	day				1 - 7
//	date			1 - 31
//	month			1 - 12
//	year			0 - 99
BYTE rtc_set_time (BYTE alarms_enabled, BYTE hours, BYTE minutes, BYTE seconds, BYTE day, BYTE date, BYTE month, BYTE year)
{
	BYTE temp;
	BYTE temp1;


	//----- WRITE THE TIME -----
	//Send the start condition
	RTC_I2C_IDLE_I2C();
	RTC_I2C_START_I2C();
	while (RTC_I2C_START_IN_PROGRESS_BIT);

	//Send the address with the write bit set
	RTC_I2C_IDLE_I2C();
	RTC_I2C_WRITE_BYTE(DS1337_I2C_ADDRESS & 0xfe);	//Bit 0 low for write
	while (RTC_I2C_TX_IN_PROGRESS_BIT)
		;
	if (RTC_I2C_ACK_NOT_RECEIVED_BIT)
		goto rtc_set_time_fail;

	//Send the register address
	RTC_I2C_IDLE_I2C();
	RTC_I2C_WRITE_BYTE(0x00);
	while (RTC_I2C_TX_IN_PROGRESS_BIT)
		;
	if (RTC_I2C_ACK_NOT_RECEIVED_BIT)
		goto rtc_set_time_fail;

	//Write seconds
	if (seconds > 59)						//Ensure value is in range
		goto rtc_set_time_fail;
	temp1 = (seconds / 10);
	temp = (seconds - (temp1 * 10)) + (temp1 << 4);
	temp &= 0x7f;							//Bit7 = enable oscillator
	RTC_I2C_IDLE_I2C();
	RTC_I2C_WRITE_BYTE(temp);
	while (RTC_I2C_TX_IN_PROGRESS_BIT)
		;
	if (RTC_I2C_ACK_NOT_RECEIVED_BIT)
		goto rtc_set_time_fail;

	//Write minutes
	if (minutes > 59)						//Ensure value is in range
		goto rtc_set_time_fail;

	temp1 = (minutes / 10);
	temp = (minutes - (temp1 * 10)) + (temp1 << 4);
	RTC_I2C_IDLE_I2C();
	RTC_I2C_WRITE_BYTE(temp);
	while (RTC_I2C_TX_IN_PROGRESS_BIT)
		;
	if (RTC_I2C_ACK_NOT_RECEIVED_BIT)
		goto rtc_set_time_fail;

	//Write hours
	if (hours > 23)					//Ensure value is in range
		goto rtc_set_time_fail;

	temp1 = (hours / 10);
	temp = (hours - (temp1 * 10)) + (temp1 << 4);
	temp &= 0x3f;						//Bit6 low = set format to 24 hour
	RTC_I2C_IDLE_I2C();
	RTC_I2C_WRITE_BYTE(temp);
	while (RTC_I2C_TX_IN_PROGRESS_BIT)
		;
	if (RTC_I2C_ACK_NOT_RECEIVED_BIT)
		goto rtc_set_time_fail;

	//Write day
	if (day > 7)						//Ensure value is in range
		goto rtc_set_time_fail;
	if (day == 0)
		goto rtc_set_time_fail;

	temp1 = (day / 10);
	temp = (day - (temp1 * 10)) + (temp1 << 4);
	RTC_I2C_IDLE_I2C();
	RTC_I2C_WRITE_BYTE(temp);
	while (RTC_I2C_TX_IN_PROGRESS_BIT)
		;
	if (RTC_I2C_ACK_NOT_RECEIVED_BIT)
		goto rtc_set_time_fail;

	//Write date
	if (date > 31)						//Ensure value is in range
		goto rtc_set_time_fail;

	temp1 = (date / 10);
	temp = (date - (temp1 * 10)) + (temp1 << 4);
	RTC_I2C_IDLE_I2C();
	RTC_I2C_WRITE_BYTE(temp);
	while (RTC_I2C_TX_IN_PROGRESS_BIT)
		;
	if (RTC_I2C_ACK_NOT_RECEIVED_BIT)
		goto rtc_set_time_fail;

	//Write month
	if (month > 12)					//Ensure value is in range
		goto rtc_set_time_fail;

	temp1 = (month / 10);
	temp = (month - (temp1 * 10)) + (temp1 << 4);
	RTC_I2C_IDLE_I2C();
	RTC_I2C_WRITE_BYTE(temp);
	while (RTC_I2C_TX_IN_PROGRESS_BIT)
		;
	if (RTC_I2C_ACK_NOT_RECEIVED_BIT)
		goto rtc_set_time_fail;

	//Write year
	if (year > 99)						//Ensure value is in range
		goto rtc_set_time_fail;

	temp1 = (year / 10);
	temp = (year - (temp1 * 10)) + (temp1 << 4);
	RTC_I2C_IDLE_I2C();
	RTC_I2C_WRITE_BYTE(temp);
	while (RTC_I2C_TX_IN_PROGRESS_BIT)
		;
	if (RTC_I2C_ACK_NOT_RECEIVED_BIT)
		goto rtc_set_time_fail;




	//----- WRITE THE CONTROL BYTE -----
	//Send restart condition
	RTC_I2C_IDLE_I2C();
	RTC_I2C_RESTART_I2C();
	while (RTC_I2C_RESTART_IN_PROGRESS_BIT)
		;

	//Send the address with the write bit set
	RTC_I2C_IDLE_I2C();
	RTC_I2C_WRITE_BYTE(DS1337_I2C_ADDRESS & 0xfe);	//Bit 0 low for write
	while (RTC_I2C_TX_IN_PROGRESS_BIT)
		;
	if (RTC_I2C_ACK_NOT_RECEIVED_BIT)
		goto rtc_set_time_fail;

	//Send the register address
	RTC_I2C_IDLE_I2C();
	RTC_I2C_WRITE_BYTE(0x0e);
	while (RTC_I2C_TX_IN_PROGRESS_BIT)
		;
	if (RTC_I2C_ACK_NOT_RECEIVED_BIT)
		goto rtc_set_time_fail;

	//Write control
	RTC_I2C_IDLE_I2C();
	RTC_I2C_WRITE_BYTE(alarms_enabled & 0x07);
	while (RTC_I2C_TX_IN_PROGRESS_BIT)
		;
	if (RTC_I2C_ACK_NOT_RECEIVED_BIT)
		goto rtc_set_time_fail;

	//Send Stop
	RTC_I2C_IDLE_I2C();
	RTC_I2C_STOP_I2C();
	while (RTC_I2C_STOP_IN_PROGRESS_BIT)
		;
	
	return (1);

//----- I2C COMMS FAILED -----
rtc_set_time_fail:

	//Send Stop
	RTC_I2C_IDLE_I2C();
	RTC_I2C_STOP_I2C();
	while (RTC_I2C_STOP_IN_PROGRESS_BIT)
		;

	return (0);
}




//**********************************************
//**********************************************
//********** GET REAL TIME CLOCK TIME **********
//**********************************************
//**********************************************
//Returns:
//	1 if sucessful, 0 if not.
//All fields are converted to binary values
//	hours			0 - 23
//	minutes			0 - 59
//	seconds			0 - 59
//	day				1 - 7
//	date			1 - 31
//	month			1 - 12
//	year			0 - 99
BYTE rtc_get_time (BYTE *hours, BYTE *minutes, BYTE *seconds, BYTE *day, BYTE *date, BYTE *month, BYTE *year)
{
	BYTE temp;

	//Send the start condition
	RTC_I2C_IDLE_I2C();
	RTC_I2C_START_I2C();
	while (RTC_I2C_START_IN_PROGRESS_BIT)
		;

	//Send the address with the write bit set
	RTC_I2C_IDLE_I2C();
	RTC_I2C_WRITE_BYTE(DS1337_I2C_ADDRESS & 0xfe);	//Bit 0 low for write
	while (RTC_I2C_TX_IN_PROGRESS_BIT)
		;
	if (RTC_I2C_ACK_NOT_RECEIVED_BIT)
		goto rtc_get_time_fail;

	//Send the register address
	RTC_I2C_IDLE_I2C();
	RTC_I2C_WRITE_BYTE(0x00);
	while (RTC_I2C_TX_IN_PROGRESS_BIT)
		;
	if (RTC_I2C_ACK_NOT_RECEIVED_BIT)
		goto rtc_get_time_fail;

	//Send restart condition
	RTC_I2C_IDLE_I2C();
	RTC_I2C_RESTART_I2C();
	while (RTC_I2C_RESTART_IN_PROGRESS_BIT)
		;

	//Send the address with the read bit set
	RTC_I2C_IDLE_I2C();
	RTC_I2C_WRITE_BYTE(DS1337_I2C_ADDRESS | 0x01);	//Bit 1 high for read
	while (RTC_I2C_TX_IN_PROGRESS_BIT)
		;
	if (RTC_I2C_ACK_NOT_RECEIVED_BIT)
		goto rtc_get_time_fail;

	//Read seconds
	RTC_I2C_IDLE_I2C();
	#ifdef RTC_I2C_READ_BYTE_START
		RTC_I2C_READ_BYTE_START
	#endif
	temp = RTC_I2C_READ_BYTE;
	*seconds = (temp & 0x0f) + (((temp & 0x70) >> 4) * 10);		//(Bit 7 is osc flag bit - dump)
	RTC_I2C_ACK();					//Send Ack
	while (RTC_I2C_ACK_IN_PROGRESS_BIT)
		;

	//Read minutes
	RTC_I2C_IDLE_I2C();
	#ifdef RTC_I2C_READ_BYTE_START
		RTC_I2C_READ_BYTE_START
	#endif
	temp = RTC_I2C_READ_BYTE;
	*minutes = (temp & 0x0f) + (((temp & 0x70) >> 4) * 10);
	RTC_I2C_ACK();					//Send Ack
	while (RTC_I2C_ACK_IN_PROGRESS_BIT)
		;

	//Read hours
	RTC_I2C_IDLE_I2C();
	#ifdef RTC_I2C_READ_BYTE_START
		RTC_I2C_READ_BYTE_START
	#endif
	temp = RTC_I2C_READ_BYTE;
	*hours = (temp & 0x0f) + (((temp & 0x30) >> 4) * 10);
	RTC_I2C_ACK();					//Send Ack
	while (RTC_I2C_ACK_IN_PROGRESS_BIT)
		;

	//Read day
	RTC_I2C_IDLE_I2C();
	#ifdef RTC_I2C_READ_BYTE_START
		RTC_I2C_READ_BYTE_START
	#endif
	temp = RTC_I2C_READ_BYTE;
	*day = (temp & 0x07);
	RTC_I2C_ACK();					//Send Ack
	while (RTC_I2C_ACK_IN_PROGRESS_BIT)
		;

	//Read date
	RTC_I2C_IDLE_I2C();
	#ifdef RTC_I2C_READ_BYTE_START
		RTC_I2C_READ_BYTE_START
	#endif
	temp = RTC_I2C_READ_BYTE;
	*date = (temp & 0x0f) + (((temp & 0x30) >> 4) * 10);
	RTC_I2C_ACK();					//Send Ack
	while (RTC_I2C_ACK_IN_PROGRESS_BIT)
		;

	//Read month
	RTC_I2C_IDLE_I2C();
	#ifdef RTC_I2C_READ_BYTE_START
		RTC_I2C_READ_BYTE_START
	#endif
	temp = RTC_I2C_READ_BYTE;
	*month = (temp & 0x0f) + (((temp & 0x10) >> 4) * 10);
	RTC_I2C_ACK();					//Send Ack
	while (RTC_I2C_ACK_IN_PROGRESS_BIT)
		;

	//Read year
	RTC_I2C_IDLE_I2C();
	#ifdef RTC_I2C_READ_BYTE_START
		RTC_I2C_READ_BYTE_START
	#endif
	temp = RTC_I2C_READ_BYTE;
	*year = (temp & 0x0f) + ((temp >> 4) * 10);
	//RTC_I2C_ACK();					//Send Ack
	//while (RTC_I2C_ACK_IN_PROGRESS_BIT)
	//	;

	//Send NAK
	RTC_I2C_IDLE_I2C();
	RTC_I2C_NOT_ACK();
	while (RTC_I2C_ACK_IN_PROGRESS_BIT)
		;

	//Send Stop
	RTC_I2C_IDLE_I2C();
	RTC_I2C_STOP_I2C();
	while (RTC_I2C_STOP_IN_PROGRESS_BIT)
		;
	
	return (1);

//----- I2C COMMS FAILED -----
rtc_get_time_fail:

	*seconds = 0;
	*minutes = 0;
	*hours = 0;
	*day = 0;
	*date = 1;
	*month = 1;
	*year = 0;

	//Send Stop
	RTC_I2C_IDLE_I2C();
	RTC_I2C_STOP_I2C();
	while (RTC_I2C_STOP_IN_PROGRESS_BIT)
		;

	return (0);
}




//***********************************************
//***********************************************
//********** SET REAL TIME CLOCK ALARM **********
//***********************************************
//***********************************************
//Returns:
//	1 if sucessful, 0 if not.
//The fields are converted from binary to BCD before being sent to the RTC
//Fields:
//	alarm_id		1 - 2
//	alarms_enabled	0 = alarms disabled, bit0 = alarm 1 enable, bit1 = alarm 2 enable, bit2 = alarms trigger INTA & INTB (0=both trigger INTA)
//	hours			0 - 23 
//	minutes			0 - 59
//	seconds			0 - 59 (not available for alarm 2)
//	date_day		1 - 31 (dates) or 101 - 107 (days)
//Set any value to 0xff to be ignored in the alarm comparison
//Note: After an alarm is activated the INT pin will remain driven low until rtc_clear_alarms() is called
BYTE rtc_set_alarm (BYTE alarm_id, BYTE alarms_enabled, BYTE hours, BYTE minutes, BYTE seconds, BYTE date_day)
{
	BYTE temp;
	BYTE temp1;
	


	//----- WRITE THE ALARM -----
	//Send the start condition
	RTC_I2C_IDLE_I2C();
	RTC_I2C_START_I2C();
	while (RTC_I2C_START_IN_PROGRESS_BIT);

	//Send the address with the write bit set
	RTC_I2C_IDLE_I2C();
	RTC_I2C_WRITE_BYTE(DS1337_I2C_ADDRESS & 0xfe);	//Bit 0 low for write
	while (RTC_I2C_TX_IN_PROGRESS_BIT)
		;
	if (RTC_I2C_ACK_NOT_RECEIVED_BIT)
		goto rtc_set_alarm_fail;

	//Send the register address
	RTC_I2C_IDLE_I2C();
	if (alarm_id == 2)
		RTC_I2C_WRITE_BYTE(0x0b);
	else
		RTC_I2C_WRITE_BYTE(0x07);
	while (RTC_I2C_TX_IN_PROGRESS_BIT)
		;
	if (RTC_I2C_ACK_NOT_RECEIVED_BIT)
		goto rtc_set_alarm_fail;

	//----- WRITE SECONDS -----
	if (alarm_id != 2)
	{
		if (seconds > 59)
		{
			//Disabled
			temp = 0x80;
		}
		else
		{
			temp1 = (seconds / 10);
			temp = (seconds - (temp1 * 10)) + (temp1 << 4);
			temp &= 0x7f;							//Bit7 = enable oscillator
		}
		RTC_I2C_IDLE_I2C();
		RTC_I2C_WRITE_BYTE(temp);
		while (RTC_I2C_TX_IN_PROGRESS_BIT)
			;
		if (RTC_I2C_ACK_NOT_RECEIVED_BIT)
			goto rtc_set_alarm_fail;
	}

	//----- WRITE MINUTES -----
	if (minutes > 59)
	{
		//Disabled
		temp = 0x80;
	}
	else
	{
		temp1 = (minutes / 10);
		temp = (minutes - (temp1 * 10)) + (temp1 << 4);
	}
	RTC_I2C_IDLE_I2C();
	RTC_I2C_WRITE_BYTE(temp);
	while (RTC_I2C_TX_IN_PROGRESS_BIT)
		;
	if (RTC_I2C_ACK_NOT_RECEIVED_BIT)
		goto rtc_set_alarm_fail;

	//----- WRITE HOURS -----
	if (hours > 59)
	{
		//Disabled
		temp = 0x80;
	}
	else
	{
		temp1 = (hours / 10);
		temp = (hours - (temp1 * 10)) + (temp1 << 4);
		temp &= 0x3f;						//Bit6 low = set format to 24 hour
	}
	RTC_I2C_IDLE_I2C();
	RTC_I2C_WRITE_BYTE(temp);
	while (RTC_I2C_TX_IN_PROGRESS_BIT)
		;
	if (RTC_I2C_ACK_NOT_RECEIVED_BIT)
		goto rtc_set_alarm_fail;

	//----- WRITE DATE / DAY -----
	if ((date_day >= 1) && (date_day <= 31))
	{
		//Date
		temp1 = (date_day / 10);
		temp = (date_day - (temp1 * 10)) + (temp1 << 4);
		
		temp &= ~0x40;		//Flag date of month
	}
	else if ((date_day >= 101) && (date_day <= 107))
	{
		//Day
		date_day -= 100;
		temp1 = (date_day / 10);
		temp = (date_day - (temp1 * 10)) + (temp1 << 4);
		
		temp |= 0x40;		//Flag day of week
	}
	else
	{
		//Disabled
		temp = 0x80;
	}
	RTC_I2C_IDLE_I2C();
	RTC_I2C_WRITE_BYTE(temp);
	while (RTC_I2C_TX_IN_PROGRESS_BIT)
		;
	if (RTC_I2C_ACK_NOT_RECEIVED_BIT)
		goto rtc_set_alarm_fail;





	//----- WRITE THE CONTROL BYTE -----
	//Send restart condition
	RTC_I2C_IDLE_I2C();
	RTC_I2C_RESTART_I2C();
	while (RTC_I2C_RESTART_IN_PROGRESS_BIT)
		;

	//Send the address with the write bit set
	RTC_I2C_IDLE_I2C();
	RTC_I2C_WRITE_BYTE(DS1337_I2C_ADDRESS & 0xfe);	//Bit 0 low for write
	while (RTC_I2C_TX_IN_PROGRESS_BIT)
		;
	if (RTC_I2C_ACK_NOT_RECEIVED_BIT)
		goto rtc_set_alarm_fail;

	//Send the register address
	RTC_I2C_IDLE_I2C();
	RTC_I2C_WRITE_BYTE(0x0e);
	while (RTC_I2C_TX_IN_PROGRESS_BIT)
		;
	if (RTC_I2C_ACK_NOT_RECEIVED_BIT)
		goto rtc_set_alarm_fail;

	//Write control
	RTC_I2C_IDLE_I2C();
	RTC_I2C_WRITE_BYTE(alarms_enabled & 0x07);
	while (RTC_I2C_TX_IN_PROGRESS_BIT)
		;
	if (RTC_I2C_ACK_NOT_RECEIVED_BIT)
		goto rtc_set_alarm_fail;


	//Write status (to clear alarms)
	RTC_I2C_IDLE_I2C();
	RTC_I2C_WRITE_BYTE(0x00);
	while (RTC_I2C_TX_IN_PROGRESS_BIT)
		;
	if (RTC_I2C_ACK_NOT_RECEIVED_BIT)
		goto rtc_set_alarm_fail;




	//Send Stop
	RTC_I2C_IDLE_I2C();
	RTC_I2C_STOP_I2C();
	while (RTC_I2C_STOP_IN_PROGRESS_BIT)
		;
	
	return (1);

//----- I2C COMMS FAILED -----
rtc_set_alarm_fail:

	//Send Stop
	RTC_I2C_IDLE_I2C();
	RTC_I2C_STOP_I2C();
	while (RTC_I2C_STOP_IN_PROGRESS_BIT)
		;

	return (0);
}



//**************************************************
//**************************************************
//********** CLEAR REAL TIME CLOCK ALARMS **********
//**************************************************
//**************************************************
//When an enabled alarm trips the alarm flags must be cleared using this function to reset the INT pin(s).
//Returns:
//	1 if sucessful, 0 if not.
BYTE rtc_clear_alarms (void)
{

	//----- WRITE THE ALARM -----
	//Send the start condition
	RTC_I2C_IDLE_I2C();
	RTC_I2C_START_I2C();
	while (RTC_I2C_START_IN_PROGRESS_BIT);

	//Send the address with the write bit set
	RTC_I2C_IDLE_I2C();
	RTC_I2C_WRITE_BYTE(DS1337_I2C_ADDRESS & 0xfe);	//Bit 0 low for write
	while (RTC_I2C_TX_IN_PROGRESS_BIT)
		;
	if (RTC_I2C_ACK_NOT_RECEIVED_BIT)
		goto rtc_clear_alarm_fail;

	//Send the register address
	RTC_I2C_IDLE_I2C();
		RTC_I2C_WRITE_BYTE(0x0f);
	while (RTC_I2C_TX_IN_PROGRESS_BIT)
		;
	if (RTC_I2C_ACK_NOT_RECEIVED_BIT)
		goto rtc_clear_alarm_fail;

	//Write status
	RTC_I2C_IDLE_I2C();
	RTC_I2C_WRITE_BYTE(0x00);
	while (RTC_I2C_TX_IN_PROGRESS_BIT)
		;
	if (RTC_I2C_ACK_NOT_RECEIVED_BIT)
		goto rtc_clear_alarm_fail;

	//Send Stop
	RTC_I2C_IDLE_I2C();
	RTC_I2C_STOP_I2C();
	while (RTC_I2C_STOP_IN_PROGRESS_BIT)
		;
	
	return (1);

//----- I2C COMMS FAILED -----
rtc_clear_alarm_fail:

	//Send Stop
	RTC_I2C_IDLE_I2C();
	RTC_I2C_STOP_I2C();
	while (RTC_I2C_STOP_IN_PROGRESS_BIT)
		;

	return (0);
}



