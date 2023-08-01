#ifndef __MOCK_H__
#define __MOCK_H__

#if defined(BBM_MOCK)
#define BBM_MOCK_DIR "mock"
#define BBM_BATTERY_FILE BBM_MOCK_DIR "/battery"
#define BBM_CHARGE_FILE BBM_MOCK_DIR "/charging"

void mock_init();

#endif

#endif
