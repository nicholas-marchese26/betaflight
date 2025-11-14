#include "platform.h"
#include "io/gps.h"
#include "common/maths.h"

#ifdef USE_GPS

static uint32_t maxDistance = 0;

void gpsDistanceUpdate(void)
{
    if (!STATE(GPS_FIX)) {
        return;
    }

    static int32_t homeLat = 0;
    static int32_t homeLon = 0;

    if (ARMING_FLAG(ARMED) && homeLat == 0) {
        homeLat = gpsSol.llh.lat;
        homeLon = gpsSol.llh.lon;
    }

    uint32_t d = gpsDistance(homeLat, homeLon, gpsSol.llh.lat, gpsSol.llh.lon);

    if (d > maxDistance) {
        maxDistance = d;
    }
}

uint32_t getMaxDistance(void)
{
    return maxDistance;
}

#endif
