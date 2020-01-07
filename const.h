/*
*   ELEVATOR STATUS
*/
#define FREE 0
#define READY 1
#define REACHING_USER 2
#define MOVING 3
#define STOPPED 4
#define ARRIVED 5

/*
* 	JOYSTICK STATUS
*/
#define DISABLED 0
#define ENABLED 1
#define SELECT_ENABLED 1
#define MOVE_ENABLED 2

/*
* ELEVATOR POSITION
* 4 km/h = 1.11111 m/s
* 8 m -> 8/1.11111 = 7.2 s = 7200 ms
* polling: 25 ms --> 7200/25 = 288
* elevator position [0, 288] = [GROUND_FLOOR, FIRST_FLOOR]
*/
#define GROUND_FLOOR 0
#define FIRST_FLOOR 288 // 0x120

/*
*		LEDs
*/
#define STATUS_LED 7
#define ALARM_LED_1 3
#define ALARM_LED_0 1
#define RESERVE_LED_1 2
#define RESERVE_LED_0 0

/*
* 	LED STATUS
*/
#define OFF 0
#define ON 1

/*
*   BLINKING CONSTS
*   - 2 Hz = 0.5s => 500ms; 25ms polling timer => HZ_2 = 20
*   - 5 Hz = 0.2s => 200ms; 25ms polling timer => HZ_5 = 8
*/
#define HZ_2 20 // 0x14
#define HZ_5 8

/* 	
*		TIME CONSTS
*		- 3s = 3000ms => 25ms polling timer => SEC_3 = 120
*/
#define SEC_3 120 // 0x78
#define MIN_1 0x017D7840    // 1 minute alarm = 25Mhz * 60s * 10^6‬ =0x59682F00
// 1 sec alarm = 25Mhz * 1s * 10^6‬ =0x017D7840 TESTING ONLY
