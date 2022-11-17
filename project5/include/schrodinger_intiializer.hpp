#pragma once

#include <armadillo>
#include <stdexcept>

struct PacketData
{
    double center;
    double width;
    double momentum;
};

// create a normalized Gaussian wave packet
arma::cx_mat get_initial_state(size_t M, PacketData packetX, PacketData packetY)
{
    arma::cx_mat u = arma::cx_mat(M, M).fill(0.);
    double h = 1. / static_cast<double>(M - 1);

    arma::cx_double cx_i = arma::cx_double(0, 1);

    for (size_t iy = 1; iy < M-1; iy++)
    {
        double y = h * iy;
        double centered_y = y - packetY.center;
        arma::cx_double yTerm1 = -centered_y * centered_y / (2 * packetY.width * packetY.width);
        arma::cx_double yTerm2 = cx_i * packetY.momentum * centered_y;
        arma::cx_double yTerm = yTerm1 + yTerm2;

        for (size_t ix = 1; ix < M-1; ix++)
        {
            double x = h * ix;
            double centered_x = x - packetX.center;
            arma::cx_double xTerm1 = -centered_x * centered_x / (2 * packetX.width * packetX.width);
            arma::cx_double xTerm2 = cx_i * packetX.momentum * centered_x;
            arma::cx_double xTerm = xTerm1 + xTerm2;

            u(iy, ix) = exp(xTerm + yTerm);
        }
    }

    // we need the initial state to be normalized
    double magnitude = sqrt(arma::accu(arma::real(arma::conj(u) % u)));
    for (size_t i = 0; i < M; i++)
    {
        for (size_t j = 0; j < M; j++)
        {
            u(i, j) /= magnitude;
        }
    }

    return u;
}

struct WallData
{
    double position;
    double thickness;
    size_t nSlits;
    double slitApeture;
    double slitSeparation;
};

arma::mat get_potential(size_t M, double v0, WallData wallData)
{
    arma::mat V = arma::mat(M, M).fill(0);

    size_t pos_left = fmax(0, floor((wallData.position - wallData.thickness / 2) * M));
    size_t pos_right = pos_left + round(wallData.thickness * M);

    for (size_t yi = 0; yi < M; yi++)
    {
        for (size_t xi = pos_left; xi < pos_right; xi++)
        {
            V(yi, xi) = v0;
        }
    }

    size_t apetureIdxs = round(wallData.slitApeture * M);
    size_t separationIdxs = round(wallData.slitSeparation * M);
    size_t startIdx = round(M / 2.);

    if (wallData.nSlits == 0)
    {
        return V;
    }

    if (wallData.nSlits % 2 == 1)
    {
        // center is empty
        for (size_t yi = startIdx; yi > startIdx - apetureIdxs / 2; yi--)
        {
            for (size_t xi = pos_left; xi < pos_right; xi++)
            {
                V(yi, xi) = 0;
                V(M-(yi-1), xi) = 0;
            }
        }
        startIdx -= apetureIdxs / 2 + separationIdxs;
    } else {
        startIdx -= separationIdxs / 2;
    }

    for (size_t i = 0; i < wallData.nSlits / 2; i++)
    {
        for (size_t yi = startIdx; yi > startIdx - apetureIdxs; yi--)
        {
            for (size_t xi = pos_left; xi < pos_right; xi++)
            {
                V(yi, xi) = 0;
                V(M-(yi-1), xi) = 0;
            }
        }

        if (startIdx < apetureIdxs + separationIdxs) {
            break;
        }
        startIdx -= apetureIdxs + separationIdxs;
    }

    return V;
}
