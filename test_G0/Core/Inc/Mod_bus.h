#include "stm32g0xx.h"

extern uint16_t REGS [20];
#define volt_set						1	//��������� ���������� 4096
#define current_set					2	//��������� ���� 
#define volt_measured				3	//���������� ����������
#define current_measured		4 //���������� ���
#define battery_charge			5	//����� ������� 
#define	On_Off							6	// 0 - ��������������� ��������, 1 - ��������������� �������

void Send_Messange(void);
void Read_Messange(void);











