#include "readsys.h"

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

char *readsys(const char *path, int len) {
  int fd = open(path, O_RDONLY);
  if (fd < 0) {
    perror("open");
    return NULL;
  } else {
    char *buf = malloc(len + 1);
    read(fd, buf, len);
    buf[len] = '\0';
    close(fd);
    return buf;
  }
}

int battery_level() {
  char *buf = readsys(BBM_BATTERY_FILE, BBM_BATTERY_LEN);
  if(buf == NULL) return -1;
  int level = atoi(buf);
  free(buf);
  return level;
}

bool charging() {
  char *buf = readsys(BBM_CHARGE_FILE, BBM_CHARGE_LEN);
  if(buf == NULL) return false;
  bool charging = buf[0] == '1';
  free(buf);
  return charging;
}

