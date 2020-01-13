/*
*   ELEVATOR STATUS
*/
#define FREE 0
#define READY 1
#define MOVING 2
#define REACHING_USER 3
#define STOPPED 4
#define ARRIVED 5
#define USER_REACHED 6
#define MAINTENANCE 7
#define EMERGENCY 8

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
*   - 2 Hz = 0.5s => 500ms
*   - 5 Hz = 0.2s => 200ms
*   - 4 Hz : 0x005F5E10

*/
#define HZ_2 0x00BEBC20
#define HZ_5 0x004C4B40
#define HZ_4 0x005F5E10

/* 	
*		TIME CONSTS
*		- 3s = 3000ms
*/
#define SEC_3 0x047868C0 // 3 * 25 * 10^6
#define MIN_1 0x59682F00 // 1 minute alarm = 25Mhz * 60s * 10^6‬ =0x59682F00
//#define MS_7200 0x0ABA9500

/* 	
*		NOTE
*/
#define NOTE_1 1
#define NOTE_2 2

/*
*	BUTTONS
*/
#define KEY_1 0
#define KEY_2 1
#define INT_0 2

/*
* JOYSTICKS
*/
#define JOYSTICK_SELECT 0
#define JOYSTICK_MOVE 1

/*
* EMERGENCY MODE
*/
#define USER_DISABLE 1
#define RESCUE_DISABLE 2
#define ENABLE 3



/************************EOF*****************************/
