#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>
#include "readsys.h"
#include "notifications.h"
#include "strings.h"
#include "debug.h"
#include "mock.h"
#include "sleep.h"


#define DELAY 1

const int NORMAL_WARNINGS[] = { 20, 15, 10, 5 };
const int CRITICAL_WARNINGS[] = { 5, 3, 2, 1 };
const int CHARGED_WARNINGS[] = { 80, 100 };


void send_low_warnings(int level, int *threshold) {
  int lowest = 101;
  for(int i = 0; i < sizeof(NORMAL_WARNINGS) / sizeof(int); i++) {
    debug("checking: level=%d; threshold=%d; NORMAL_WARNINGS[i]=%d\n", level, *threshold, NORMAL_WARNINGS[i]);
    if(level <= NORMAL_WARNINGS[i]) {
      if (*threshold > NORMAL_WARNINGS[i])
        lowest = NORMAL_WARNINGS[i];
    } else if(lowest == 101) {
        return;
    }
  }
  for(int i = 0; i < sizeof(CRITICAL_WARNINGS) / sizeof(int); i++) {
    debug("checking: level=%d; threshold=%d; CRITICAL_WARNINGS[i]=%d\n", level, *threshold, CRITICAL_WARNINGS[i]);
    if(level <= CRITICAL_WARNINGS[i]) {
      if (*threshold > CRITICAL_WARNINGS[i]) {
        lowest = -CRITICAL_WARNINGS[i];
      }
    } 
  }
  debug("Battery warning: %d%%\n", level);

  if (lowest == 101) return;

  if(lowest < 0) {
    ntf_send("Critical: battery below %d%%!", -lowest);
    *threshold = -lowest;
  } else {
    ntf_send("Warning: battery below %d%%!", lowest);
    *threshold = lowest;
  }
}

void send_high_warnings(int level, int *threshold) {
  for(int i = sizeof(CHARGED_WARNINGS) / sizeof(int) -1; i >= 0; i++) {
    debug("checking: level=%d; threshold=%d; CHARGED_WARNINGS[i]=%d\n", level, *threshold, CHARGED_WARNINGS[i]);
    if(level >= CHARGED_WARNINGS[i]) {
      if (*threshold < CHARGED_WARNINGS[i]) {
        debug("Battery warning: %d%%\n", level);
        ntf_send("Battery charged: %d%%! Unplug charger for improving battery life.", CHARGED_WARNINGS[i]);
        *threshold = CHARGED_WARNINGS[i];
        break;
      }
    } else break;
  }
}


int main(int argc, char *argv[]) {
#if defined(BBM_MOCK)
  mock_init();
#endif

  int blv, lv = battery_level();
  bool bch, ch = charging();
  int low_warning = 101;
  int high_warning = -1;
  
  ntf_init();
  ntf_send("BBM started!\nPercentage: %d%%\nStatus: %s", lv, ch ? ST_CHARGING : ST_DISCHARGING);

  if(!ch)
    // will always notify on startup if in warning range and not charging
    send_low_warnings(lv, &low_warning);
  else
    send_high_warnings(lv, &high_warning);

  sleep(DELAY);
  while(1) {
    blv = lv;
    bch = ch;
    lv = battery_level();
    ch = charging();

    detect_suspend(blv, lv);

    if(blv > lv) {
      // discharging
      debug("Battery discharging: %d => %d%%\n", blv, lv);
 
      // check for warnings
      send_low_warnings(lv, &low_warning);
      high_warning = lv;
    } else if(blv < lv) {
      //    charging
      debug("Battery charging: %d => %d%%\n", blv, lv);

      // chech for warnings
      send_high_warnings(lv, &high_warning);
      low_warning = lv;
    }

    if(ch != bch) {
      if(ch) {
        // connected
        debug("Charger connected\n");
        ntf_send("Charger connected!");
      } else {
        // disconnected
        ntf_send("Charger disconnected!");
        debug("Charger disconnected\n");
      }
    }
    start_sleep();
    sleep(DELAY);
  }
}
