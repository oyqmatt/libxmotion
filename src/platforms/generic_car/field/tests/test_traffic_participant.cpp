#include "field/traffic_participant.hpp"
#include "field/threat_distribution.hpp"
#include "fastplot/field_plot.hpp"

#include "stopwatch/stopwatch.h"

using namespace librav;

int main()
{
    TrafficParticipant traff_part(100, 100);

    stopwatch::StopWatch timer;

    timer.tic();

    GaussianPositionVelocityThreat gau(50, 50, 1, 1);
    traff_part.SetThreatDistribution(gau);
    ScalarFieldMatrix mat = traff_part.GenerateFieldMatrix(0, 1, 0, 1, true);

    std::cout << "time elapsed: " << timer.toc() << std::endl;

    // plot surface
    FastPlot::ShowFieldSurface(mat.x, mat.y, mat.z, true);

    return 0;
}