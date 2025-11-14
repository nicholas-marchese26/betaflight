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
    // Update current altitude
    calculateEstimatedAltitude();

    // Convert to meters
    float currentAltitudeM = zeroedAltitudeCm / 100.0f;

    // Static variable to hold the max altitude across calls
    static float maxAltitudeM = 0.0f;

    // Update max if current altitude is higher
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
    // Get horizontal distance in meters
    uint32_t horizontalM = getMaxDistance();

    // Get max altitude in meters
    float maxAltM = getMaxAltitudeMeters();

    // 3D distance
    float max3DDistance = sqrtf(horizontalM * horizontalM + maxAltM * maxAltM);

    return max3DDistance;
}

#endif
