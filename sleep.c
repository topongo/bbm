#include "sleep.h"
#include "notifications.h"
#include "debug.h"
#include <time.h>

time_t last_sleep = 0;

void start_sleep() {
  last_sleep = time(NULL);
}

void detect_suspend(int blevel, int level) {
  if(last_sleep == 0) return;
  time_t now = time(NULL);
  if(now - last_sleep > 60 && blevel != level) {
    debug("Suspend detected\n");
    if(blevel != level) {
      const char *act = blevel < level ? "dropped" : "increased";
      ntf_send("Battery level has %s by %d%%", act, level - blevel);
    }
  }
}

