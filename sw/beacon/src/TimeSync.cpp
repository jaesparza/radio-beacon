#include "TimeSync.h"

const char *weekdays[] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
const char *months[] = {"Dummy", "Jan", "Feb", "Mar", "Apr", "May", "Jun",
                        "Jul",   "Aug", "Sep", "Oct", "Nov", "Dec"};

char s[128];

#define _DEBUG_OUT

TimeSync::TimeSync(RTClock *rt, TinyGPSPlus *gps) {
    _rt = rt;
    _gps = gps;
}

uint8_t TimeSync::syncRTC() {
    bool timeUpdatePending = true;
    int i = 0;
    while (timeUpdatePending) {
        i++;
        while (GPS_SERIAL.available() > 0) {
            _gps->encode(GPS_SERIAL.read());
            if (_gps->time.isUpdated()) {
                _mtt.month = _gps->date.month();
                _mtt.day = _gps->date.day();
                _mtt.year = _gps->date.year() - 1970;
                _mtt.hour = _gps->time.hour();
                _mtt.minute = _gps->time.minute();
                _mtt.second = _gps->time.second();
                _rt->setTime(_rt->makeTime(_mtt));

                timeUpdatePending = false;
            }
        }
    }

#ifdef _DEBUG_OUT
    BEACON_SERIAL.print("YEAR ");
    BEACON_SERIAL.print(_gps->date.year());
    BEACON_SERIAL.print(" MONTH ");
    BEACON_SERIAL.print(_gps->date.month());
    BEACON_SERIAL.print(" DAY ");
    BEACON_SERIAL.print(_gps->date.day());
    BEACON_SERIAL.print("UTC time: ");
    BEACON_SERIAL.print(_gps->time.hour()); // Hour (0-23) (u8)
    BEACON_SERIAL.print(":");
    BEACON_SERIAL.print(_gps->time.minute()); // Minute (0-59) (u8)
    BEACON_SERIAL.print(":");
    BEACON_SERIAL.print(_gps->time.second()); // Second (0-59) (u8)
    BEACON_SERIAL.println();
    BEACON_SERIAL.print("Attempts until time read: ");
    BEACON_SERIAL.println(i);
#endif //_DEBUG_OUT
}

void TimeSync::getGPStime() {
    while (1) {
        while (GPS_SERIAL.available() > 0) {
            _gps->encode(GPS_SERIAL.read());
            if (_gps->time.isUpdated()) {
                BEACON_SERIAL.print("UTC time: ");
                BEACON_SERIAL.print(_gps->time.hour()); // Hour (0-23) (u8)
                BEACON_SERIAL.print(":");
                BEACON_SERIAL.print(_gps->time.minute()); // Minute (0-59) (u8)
                BEACON_SERIAL.print(":");
                BEACON_SERIAL.print(_gps->time.second()); // Second (0-59) (u8)
                BEACON_SERIAL.println();
            }
        }
    }
}

void TimeSync::monitorRTC() {
    static uint8_t lastSecond;

    _rt->breakTime(_rt->now(), _mtt);

    if (_mtt.second != lastSecond) {
        lastSecond = _mtt.second;
        sprintf(s, "RTC timestamp: %s %u %u, %s, %02u:%02u:%02u\n",
                months[_mtt.month], _mtt.day, _mtt.year + 1970,
                weekdays[_mtt.weekday], _mtt.hour, _mtt.minute, _mtt.second);
        BEACON_SERIAL.print(s);
    }
}

void TimeSync::NMEAstringGrabber() {
    while (1) {
        while (GPS_SERIAL.available() > 0) {
            BEACON_SERIAL.write(GPS_SERIAL.read());
        }
    }
}

void TimeSync::registerAlarmFuntion(voidFuncPtr triggeredFunction,
                                    uint8_t secondsAhead) {
    _rt->attachAlarmInterrupt(triggeredFunction, _rt->getTime() + secondsAhead);
}

void TimeSync::scheduleNextWSPRTX(voidFuncPtr triggeredFunction,
                                  uint16_t minutes) {
    // Get the current time
    _rt->breakTime(_rt->now(), _mtt);
    _mtt.minute = (((_mtt.minute + minutes) % 2) == 0)
                      ? _mtt.minute + minutes + 2
                      : _mtt.minute + minutes + 1;
    _mtt.second = 0;
    // Set the alarm for the next even minute
    _rt->createAlarm(triggeredFunction, _mtt);
    BEACON_SERIAL.println("Next TX will be in: ");
    printTime(_mtt);
}

void TimeSync::printTime(tm_t mtt) {
    sprintf(s, "RTC time is: %s %u %u, %s, %02u:%02u:%02u\n", months[mtt.month],
            mtt.day, mtt.year + 1970, weekdays[mtt.weekday], mtt.hour,
            mtt.minute, mtt.second);
    BEACON_SERIAL.print(s);
}