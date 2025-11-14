#include <math.h>
#include "platform.h"
#include "position.h"
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


float getMaxAltitudeMeters(void)
{
    calculateEstimatedAltitude();
    float currentAltitudeM = zeroedAltitudeCm / 100.0f;
    static float maxAltitudeM = 0.0f;

    if (currentAltitudeM > maxAltitudeM) {
        maxAltitudeM = currentAltitudeM;
    }

    return maxAltitudeM;
}



uint32_t getMaxDistance(void)
{
    return maxDistance;
}


float getMax3DDistanceMeters(void)
{
    uint32_t horizontalM = getMaxDistance();
    float maxAltM = getMaxAltitudeMeters();
    float max3DDistance = sqrtf(horizontalM * horizontalM + maxAltM * maxAltM);
    return max3DDistance;
}

#endif
