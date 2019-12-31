/*
* Debounce.h - ���������� ��������� ���������� �������� STM32.
*
* ����� ���� ������������ ��� ���������� �������� ���������, �������� ���������� �������� �� �����.
*
* �������� � ������� ������.
*
* � ������������ �������� ��������� ������ ���������� ���� �� ������� ���������:
*
* void scanStability(void); // ����� �������� ����������� ��������� �������
* void scanAverage(void); // ����� ���������� ������� �� �������� ��������
*
* � ���������� ����������� �������� ��������� �������:
*
* uint8_t flagLow; // ������� ������ � ������ ������
* uint8_t flagRising; // ������� ��� ������������� �����
* uint8_t flagFalling; // ������� ��� ������������� �����
*
* �������� ����� ���� ��������� ���������:
*
* uint8_t readFlagLow(void); // ������ �������� ������ � ������ ������
* uint8_t readFlagRising(void); // ������ �������� ��� ������������� �����
* uint8_t readFlagFalling(void); // ������ �������� ��� ������������� �����
*
* ������ �������� �������:
* Debounce button(GPIOC, 1 << 11, 10); // ��������� ������ Debounce
*
* �������� ������� http://mypractic.ru/uroki-stm32 � ������ 12, 13, 14
*
* ����������� ��������� ��������.
*
* http://mypractic.ru
*/

#ifndef DEBOUNCE_H
#define DEBOUNCE_H

#include "stm32f103xb.h"

class Debounce {

public:
	Debounce(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint32_t filterTime);   // �����������
	void scanStability(void);   // ����� �������� ����������� ��������� �������
	void scanAverage(void);   // ����� ���������� ������� �� �������� ��������
	void setTime(uint32_t filterTime);   // ����� ��������� ������� ����������
	uint8_t readFlagLow(void);   // ������ �������� ������ � ������ ������
	uint8_t readFlagRising(void);   // ������ �������� ��� ������������� �����
	uint8_t readFlagFalling(void);   // ������ �������� ��� ������������� �����

	uint8_t flagLow;   // ������� ������ � ������ ������
	uint8_t flagRising;   // ������� ��� ������������� �����
	uint8_t flagFalling;   // ������� ��� ������������� �����

private :
	GPIO_TypeDef *_GPIOx;   // ����
	uint16_t _GPIO_Pin;   // ����� ������
	uint32_t _filterTime;   // ����� ����������
	uint32_t _filterTimeCount;   // ������� ������� ����������
};

#endif