#include <assert.h>
#include <fstream>
#include <vector>
#include <random>
#include <chrono>
#include <tuple>
#include <map>

#include <iostream>

struct IsingState
{
    std::vector<int8_t> spins;
    uint32_t L;
    double T;
    int32_t E = 0;
    int32_t M = 0;
};

typedef std::map<int8_t, double> ExpMap;

enum class InitialState
{
    ORDERED,
    UNORDERED,
};

std::pair<IsingState, ExpMap> createState(uint32_t L, double T, InitialState initialState = InitialState::UNORDERED)
{
    ExpMap expMap;
    for (int dE = -8; dE <= 8; dE += 4)
    {
        expMap.insert({dE, exp(-dE / T)});
    }

    unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 generator;
    generator.seed(seed);

    std::uniform_int_distribution<int> uniform(0, 1);

    IsingState state{
        .spins = {},
        .L = L,
        .T = T,
    };
    state.spins.resize(L * L);

    for (size_t i = 0; i < L; i++)
    {
        for (size_t j = 0; j < L; j++)
        {
            if (initialState == InitialState::UNORDERED)
            {
                int v = uniform(generator);
                state.spins[i * L + j] = -(1 - v) + v;
            }
            else if (initialState == InitialState::ORDERED)
            {
                state.spins[i * L + j] = 1;
            }

            assert(state.spins[i * L + j] == 1 || state.spins[i * L + j] == -1);
        }
    }

    if (L == 1)
    {
        state.M = state.spins[0];
        state.E = 0;
        return {state, expMap};
    }
    if (L == 2)
    {
        state.E = -(state.spins[0] + state.spins[3]) * (state.spins[1] + state.spins[2]);
        state.M = state.spins[0] + state.spins[1] + state.spins[2] + state.spins[3];

        return {state, expMap};
    }

    // L > 2
    for (size_t i = 0; i < L; i++)
    {
        for (size_t j = 0; j < L; j++)
        {
            int8_t dr_spins = state.spins[i * L + (j + 1) % L] + state.spins[((i + 1) % L) * L + j];
            state.E += -state.spins[i * L + j] * dr_spins;
            state.M += state.spins[i * L + j];
        }
    }

    return {state, expMap};
}

void modifyStateMCMC(IsingState &state, const std::map<int8_t, double> &expMap)
{
    unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 generator;
    generator.seed(seed);

    std::uniform_int_distribution<int> idxDist(0, state.L - 1);
    int xidx = idxDist(generator);
    int yidx = idxDist(generator);

    int8_t flippedSpin = state.spins[yidx * state.L + xidx];

    int8_t dM = -2 * flippedSpin;
    int8_t dE = 0;
    if (state.L > 2)
    {
        dE = 2 * flippedSpin * (state.spins[yidx * state.L + ((xidx + 1) % state.L)] + state.spins[yidx * state.L + ((state.L + xidx - 1) % state.L)] + state.spins[((yidx + 1) % state.L) * state.L + xidx] + state.spins[((state.L + yidx - 1) % state.L) * state.L + xidx]);
    }
    else if (state.L == 2)
    {
        if ((xidx == 0 && yidx == 0) || (xidx == 1 && yidx == 1))
        {
            dE = 2 * flippedSpin * (state.spins[1] + state.spins[2]);
        }
        else
        {
            dE = 2 * flippedSpin * (state.spins[0] + state.spins[3]);
        }
    }
    else
    {
        dE = 0;
    }

    double stateChangeProb = fmin(1, expMap.at(dE));

    std::uniform_real_distribution<double> probDist(0, 1);
    if (probDist(generator) <= stateChangeProb)
    {
        state.spins[yidx * state.L + xidx] = -state.spins[yidx * state.L + xidx];
        state.E += dE;
        state.M += dM;
    }
}

std::pair<double, double> sampleDistribution(IsingState &state, const ExpMap expMap)
{
    size_t n = state.L * state.L;

    // do one Monte Carlo cycle
    for (size_t i = 0; i < n; i++)
    {
        modifyStateMCMC(state, expMap);
    }

    double eps = state.E / static_cast<double>(n);
    double absm = fabs(state.M / static_cast<double>(n));

    return {eps, absm};
}

void sampleToFile(uint32_t L, double T, uint32_t nCycles, std::ofstream &file, InitialState initialState = InitialState::UNORDERED, uint32_t burnSamples = 0)
{
    auto [state, expMap] = createState(L, T, initialState);
    for (size_t i = 0; i < burnSamples; i++)
    {
        sampleDistribution(state, expMap);
    }

    for (size_t i = 0; i < nCycles - burnSamples; i++)
    {
        auto [eps, absm] = sampleDistribution(state, expMap);
        file << eps << " " << absm << "\n";
    }
}

std::tuple<double, double, double, double> calcQtys(
    uint32_t L, double T, uint32_t nCycles, InitialState initialState = InitialState::UNORDERED, uint32_t burnSamples = 0)
{
    auto [state, expMap] = createState(L, T, initialState);
    for (size_t i = 0; i < burnSamples; i++)
    {
        sampleDistribution(state, expMap);
    }

    double totEps = 0;
    double totEps2 = 0;

    double totAbsm = 0;
    double totAbsm2 = 0;

    for (size_t i = 0; i < nCycles - burnSamples; i++)
    {
        auto [eps, absm] = sampleDistribution(state, expMap);
        totEps += eps;
        totEps2 += eps * eps;

        totAbsm += absm;
        totAbsm2 += absm*absm;
    }

    double avgEps = totEps / static_cast<double>(nCycles - burnSamples);
    double avgEps2 = totEps2 / static_cast<double>(nCycles - burnSamples);

    double avgAbsm = totAbsm / static_cast<double>(nCycles - burnSamples);
    double avgAbsm2 = totAbsm2 / static_cast<double>(nCycles - burnSamples);

    double Cv = (L*L) * (avgEps2 - avgEps*avgEps) / (T*T);
    double X = (L*L) * (avgAbsm2 - avgAbsm*avgAbsm) / T;

    return {avgEps, avgAbsm, Cv, X};
}
