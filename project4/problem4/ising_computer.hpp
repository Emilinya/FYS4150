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
    int32_t E = 0;
    int32_t M = 0;
};

typedef std::map<int8_t, double> ExpMap;

enum class InitialState
{
    ORDERED,
    UNORDERED,
};

// create initial state from temperature and initial state type
template <uint32_t L>
std::pair<IsingState, ExpMap> createState(double T, std::mt19937 &generator, InitialState initialState = InitialState::UNORDERED)
{
    ExpMap expMap;
    for (int dE = -8; dE <= 8; dE += 4)
    {
        expMap.insert({dE, exp(-dE / T)});
    }

    std::uniform_int_distribution<int> uniform(0, 1);

    IsingState state;
    state.spins.resize(L * L);

    for (size_t i = 0; i < L; i++)
    {
        for (size_t j = 0; j < L; j++)
        {
            if (initialState == InitialState::UNORDERED)
            {
                // v is 0 or 1, so we must transform to -1 or 1
                int v = uniform(generator);
                state.spins[i * L + j] = -(1 - v) + v;
            }
            else if (initialState == InitialState::ORDERED)
            {
                state.spins[i * L + j] = 1;
            }

            // assertion just in case
            assert(state.spins[i * L + j] == 1 || state.spins[i * L + j] == -1);
        }
    }

    if (L == 1)
    {
        // this case does not make much sense, return somewhat reasonable values
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
            // sum of spin below and spin to the right
            int8_t dr_spins = state.spins[i * L + (j + 1) % L] + state.spins[((i + 1) % L) * L + j];

            state.E += -state.spins[i * L + j] * dr_spins;
            state.M += state.spins[i * L + j];
        }
    }

    return {state, expMap};
}

// modify state using a Monte Carlo Markov chain approach
template <uint32_t L>
void modifyStateMCMC(IsingState &state, const ExpMap &expMap, std::mt19937 &generator)
{
    std::uniform_int_distribution<int> idxDist(0, L - 1);
    int xidx = idxDist(generator);
    int yidx = idxDist(generator);

    int8_t flippedSpin = state.spins[yidx * L + xidx];

    int8_t dM = -2 * flippedSpin;
    int8_t dE = 0;
    if (L > 2)
    {
        // must do (L + idx - 1) % L instead of (idx - 1) % L, because the mod
        // operator handles negative numbers poorly
        dE = 2 * flippedSpin * (state.spins[yidx * L + ((xidx + 1) % L)] + state.spins[yidx * L + ((L + xidx - 1) % L)] + state.spins[((yidx + 1) % L) * L + xidx] + state.spins[((L + yidx - 1) % L) * L + xidx]);
    }
    else if (L == 2)
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
        state.spins[yidx * L + xidx] = -state.spins[yidx * L + xidx];
        state.E += dE;
        state.M += dM;
    }
}

// get epsilon and |m| after one Monte Carlo cycle
template <uint32_t L>
std::pair<double, double> sampleDistribution(IsingState &state, const ExpMap &expMap, std::mt19937 &generator)
{
    size_t n = L * L;

    // do one Monte Carlo cycle
    for (size_t i = 0; i < n; i++)
    {
        modifyStateMCMC<L>(state, expMap, generator);
    }

    double eps = state.E / static_cast<double>(n);
    double absm = fabs(state.M / static_cast<double>(n));

    return {eps, absm};
}

// save epsilon and |m| values to a file
template <uint32_t L>
void sampleToFile(double T, uint32_t nCycles, std::ofstream &file, std::mt19937 &generator, InitialState initialState = InitialState::UNORDERED, uint32_t burnSamples = 0)
{
    auto [state, expMap] = createState<L>(T, generator, initialState);
    for (size_t i = 0; i < burnSamples; i++)
    {
        sampleDistribution<L>(state, expMap, generator);
    }

    for (size_t i = 0; i < nCycles - burnSamples; i++)
    {
        auto [eps, absm] = sampleDistribution<L>(state, expMap, generator);
        file << eps << " " << absm << "\n";
    }
}

// calculate statistical quantities <epslilon>, <|m|>, Cv, and X
template <uint32_t L>
std::tuple<double, double, double, double> calcQtys(
    double T, uint32_t nCycles, std::mt19937 &generator, InitialState initialState = InitialState::UNORDERED, uint32_t burnSamples = 0)
{
    auto [state, expMap] = createState<L>(T, generator, initialState);

    for (size_t i = 0; i < burnSamples; i++)
    {
        sampleDistribution<L>(state, expMap, generator);
    }

    double totEps = 0;
    double totEps2 = 0;

    double totAbsm = 0;
    double totAbsm2 = 0;

    for (size_t i = 0; i < nCycles - burnSamples; i++)
    {
        auto [eps, absm] = sampleDistribution<L>(state, expMap, generator);
        totEps += eps;
        totEps2 += eps * eps;

        totAbsm += absm;
        totAbsm2 += absm * absm;
    }

    double avgEps = totEps / static_cast<double>(nCycles - burnSamples);
    double avgEps2 = totEps2 / static_cast<double>(nCycles - burnSamples);

    double avgAbsm = totAbsm / static_cast<double>(nCycles - burnSamples);
    double avgAbsm2 = totAbsm2 / static_cast<double>(nCycles - burnSamples);

    double Cv = (L*L) * (avgEps2 - avgEps*avgEps) / (T*T);
    double X = (L*L) * (avgAbsm2 - avgAbsm*avgAbsm) / T;

    return {avgEps, avgAbsm, Cv, X};
}
