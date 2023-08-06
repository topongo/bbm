#include "mock.h"
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include "readsys.h"
#include "debug.h"

void create_files() {
  debug("Creating mock files...\n");
  int fd1 = creat(BBM_BATTERY_FILE, 0644);
  write(fd1, "100", 3);
  close(fd1);
  int fd2 = creat(BBM_CHARGE_FILE, 0644);
  write(fd2, "0", 1);
  close(fd2);
}

void mock_init() {
  debug("Debugging enabled!\n");
 
  debug("Checking for mock files...\n");
  DIR *dir = opendir(BBM_MOCK_DIR);
  if(dir == NULL) {
    debug("Creating mock directory\n");
    mkdir(BBM_MOCK_DIR, 0755);
    create_files();
  } else {
    closedir(dir);
    int fd1 = open(BBM_BATTERY_FILE, O_RDONLY);
    int fd2 = open(BBM_CHARGE_FILE, O_RDONLY);
    if(fd1 < 0 || fd2 < 0) {
      create_files();
    }
    close(fd1);
    close(fd2);
  }
}

