#include <Debounce.h>

// �����������
Debounce::Debounce(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint32_t filterTime) {

	_GPIOx = GPIOx;
	_GPIO_Pin = GPIO_Pin;
	_filterTime = filterTime;
	_filterTimeCount = _filterTime / 2;
	flagLow = 0;
	flagRising = 0;
	flagFalling = 0;

	// ���������� ������������ �����
	if(_GPIOx == GPIOA) RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	else if(_GPIOx == GPIOB) RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
	else if(_GPIOx == GPIOC) RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	else if(_GPIOx == GPIOD) RCC->APB2ENR |= RCC_APB2ENR_IOPDEN;
	else if(_GPIOx == GPIOE) RCC->APB2ENR |= RCC_APB2ENR_IOPEEN;

	// ������������� ������������ ������ �� ���� � ������������� ����������
	uint32_t i = 0;
	uint16_t d = _GPIO_Pin;
	while (i < 16) {
		d = d >> 1;
		if (d == 0) break;
		i++;
	}

	if (i < 8) {
		_GPIOx->CRL |= 0b1000 << (i * 4);
		_GPIOx->CRL &= ~(0b0111 << (i * 4));
	}
	else {
		_GPIOx->CRH |= 0b1000 << ((i - 8) * 4);
		_GPIOx->CRH &= ~(0b0111 << ((i - 8) * 4));
	}

	_GPIOx->ODR |= 1 << i;
}
//----------------- ������ ������ Debounce

// ����� �������� ����������� ��������� �������
// flagLow; // ������� ������ � ������ ������
// flagRising; // ������� ��� ������������� �����
// flagFalling; // ������� ��� ������������� �����
void Debounce::scanStability(void) {

	// ���� ��������� ������ �� ����������, �� ������� = 0
	if((flagLow == 0) == ((_GPIOx->IDR & _GPIO_Pin) != 0)) _filterTimeCount = 0;

	// ��������� ������ ����������
	else {
		_filterTimeCount++;

		if (_filterTimeCount >= _filterTime) {
			// ��������� ������ �� �������� � ������� ��������� �������
		    // ��������� ������ ����� ����������
			flagLow = (~flagLow) & 1;   // �������� �������� ���������
			_filterTimeCount = 0;    // ����� �������� �������

	        if(flagLow != 0) flagFalling = 1;   // ������� ������ ������ �� �������
	        else flagRising = 1;   // ������� ������ ������ �� �������
		}
	}
}

// ����� ���������� ������� �� �������� ��������
// flagLow; // ������� ������ � ������ ������
// flagRising; // ������� ��� ������������� �����
// flagFalling; // ������� ��� ������������� �����
void Debounce::scanAverage(void) {

	if ((flagLow == 0) == ((_GPIOx->IDR & _GPIO_Pin) != 0)) {
		// ��������� ������� �������� �������
		if(_filterTimeCount != 0) _filterTimeCount--;   // ������� ������� - 1 � ������������ �� 0
	}

	else {
		// ��������� ������� ����������
		_filterTimeCount++;     // +1 � �������� �������

		if(_filterTimeCount >= _filterTime) {
			// �����
			flagLow = (~flagLow) & 1;   // �������� �������� ���������
			_filterTimeCount = 0;    // ����� �������� �������

	        if(flagLow != 0) flagFalling = 1;   // ������� ������ ������ �� �������
	        else flagRising = 1;   // ������� ������ ������ �� �������
		}
	}
}

// ����� ��������� ������� ����������
void Debounce::setTime(uint32_t filterTime) {
	_filterTime = filterTime;
}

// ������ �������� ������ � ������ ������
uint8_t Debounce::readFlagLow(void) {
	return (flagLow);
}

// ������ �������� ��� ������������� �����
uint8_t Debounce::readFlagRising(void) {
	if (flagRising != 0) {
		flagRising = 0;
		return (1);
	}
	else return (0);
}

// ������ �������� ��� ������������� �����
uint8_t Debounce::readFlagFalling(void) {
	if (flagFalling != 0) {
		flagFalling = 0;
		return (1);
	}
	else return (0);
}

