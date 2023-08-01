#ifndef __READSYS_H__
#define __READSYS_H__

#if defined(BBM_MOCK)
  #define BBM_MOCK_DIR "mock/bbm"
  #define BBM_BATTERY_FILE "mock/capacity"
  #define BBM_CHARGE_FILE "mock/charging"
#else
  #define BBM_BATTERY_FILE "/sys/class/power_supply/BAT0/capacity"
  #define BBM_CHARGE_FILE "/sys/class/power_supply/ACAD/online"
#endif

#define BBM_BATTERY_LEN 3
#define BBM_CHARGE_LEN 1 

typedef enum bool {true, false} bool;

char *readsys(const char *path, int len);

int battery_level();

bool charging();

#endif
