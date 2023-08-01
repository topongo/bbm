#include "notifications.h"

#include "debug.h"
#include <stdarg.h>
#include <stdio.h>
#include <libnotify/notify.h>

void ntf_init() {
  notify_init("BBM");
}

void ntf_send(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  char body[256];
  vsprintf(body, fmt, args);
  va_end(args);

  debug("Sending notification: %s\n", body);
  NotifyNotification *n = notify_notification_new("BBM", body, NULL);
  notify_notification_set_timeout(n, 5000);
  notify_notification_show(n, NULL);
}

