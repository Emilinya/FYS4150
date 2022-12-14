#pragma once

#include "../../cpp_utils/utils.hpp"
#include "crank_nicolson_ititializer.hpp"
#include "schrodinger_intiializer.hpp"
#include <string>

// Do one Crank-Nicolson step
arma::cx_vec step(arma::sp_cx_mat &A, arma::sp_cx_mat &B, arma::cx_vec &u)
{
    arma::cx_vec b = B * u;
    return arma::spsolve(A, b);
}

// Simulate the Schrödinger equation using given parameters
void simulate(
    std::string dataPath, double h, double dt, double T, double v0,
    PacketData packetX, PacketData packetY, WallData wallData)
{
    size_t M = 1 + 1. / h;
    size_t N = 1 + T / dt;

    auto V = get_potential(M, v0, wallData);
    auto U0 = get_initial_state(M, packetX, packetY);
    auto [A, B] = initialize_matricies(M, dt, V);

    auto u = mat2vec(U0);
    auto us = arma::cx_mat(N, u.size());
    us.row(0) = u.t();

    auto ts = arma::vec(N).fill(0);
    for (size_t i = 1; i < N; i++)
    {
        uint p = (i * 100) / N;
        if (p != ((i - 1) * 100) / N)
        {
            rprint(p << "/" << 100 << " %");
        }
        u = step(A, B, u);
        us.row(i) = u.t();
        ts(i) = ts(i - 1) + dt;
    }
    print("\r100/100 %");

    if (dataPath.back() != '/')
    {
        dataPath += "_";
    }

    ts.save(string_format("%sts.bin", dataPath.c_str()));
    us.save(string_format("%sus.bin", dataPath.c_str()));
    V.save(string_format("%sV.bin", dataPath.c_str()));
}
