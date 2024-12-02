#include "stm32g0xx.h"

extern uint16_t REGS [20];
#define volt_set						1	//установка напряжения 4096
#define current_set					2	//установка тока 
#define volt_measured				3	//измеренное напряжения
#define current_measured		4 //измеренный ток
#define battery_charge			5	//заряд батаери 
#define	On_Off							6	// 0 - преоюразователь выключен, 1 - преобразователь включён

void Send_Messange(void);
void Read_Messange(void);











