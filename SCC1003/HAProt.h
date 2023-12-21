//#include "application.h"
//#include "x10.h"
#include "x10constants.h"

#define MAX_UNITS                16

typedef struct {
    uint8_t ID;
    char Name[8];
    uint8_t CurrentState = OFF;
    uint8_t LastState = OFF;
    uint8_t OnMonth;
    uint8_t OnDay;
    uint8_t OnHour;
    uint8_t OnMinute;
    uint8_t OffMonth;
    uint8_t OffDay;
    uint8_t OffHour;
    uint8_t OffMinute;
    uint8_t Weekday = 0;
    uint8_t RepeatMonth = 0;
    uint8_t LightSensorControl = false;
    uint8_t LightNightValue = OFF; // Valor ao escurecer
    uint8_t LightDayValue = OFF; // Valor ao anoitecer
} Unit;

extern Unit Units[MAX_UNITS-1];
extern uint8_t House;

void HAProcessCommand(uint8_t *HABuffer, uint8_t nBytesReceivedFromSocket);

