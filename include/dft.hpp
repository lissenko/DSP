#pragma once

#include <iostream>
#include <complex>
#include <numbers>
#include <vector>
#include <thread>
#include <chrono>
#include <mpi.h>
#include <cstdlib>
#include <sstream>  

using namespace std::chrono;

typedef std::complex<double> Complex;
typedef std::vector<std::complex<double>> ComplexVector;
#define d(x) (static_cast<double>(x))

constexpr std::string_view SEQUENTIAL = "sequential";
constexpr std::string_view PARALLEL = "parallel";
constexpr std::string_view MPI_SEQUENTIAL = "mpi_sequential";
constexpr std::string_view MPI_PARALLEL = "mpi_parallel";
constexpr std::string_view FFT = "fft";

void dft_operation(double* x, const long unsigned N, ComplexVector& X, std::size_t start, std::size_t end) {
    for (std::size_t k = start; k < end; ++k) {
        double real = 0.0;
        double imag = 0.0;
        for (std::size_t n = 0; n < N; ++n) {
            double angle = 2.0 * M_PI * d(k) * d(n) / d(N);
            real += x[n] * std::cos(angle);
            imag -= x[n] * std::sin(angle);
        }
        X[k] = std::complex<double>(real, imag);
    }
}

void dft_sequential(double* x, const long unsigned N, ComplexVector& X) {
        dft_operation(x, N, X, 0, N);
}

void dft_parallel(double* x, const long unsigned N, ComplexVector& X, std::size_t
        numThreads, std::size_t size = 0, std::size_t low = 0, std::size_t high = 0) {
    if (size == 0) { size = N; high = N; }
    std::vector<std::thread> threads;
    std::size_t range = size / numThreads;
    for (std::size_t i = 0; i < numThreads-1; ++i) {
        std::size_t start = i * range + low;
        std::size_t end = (i+1) * range + low;
        /* std::cout << "[ " << start << ", " << end << "]" << std::endl; */
        threads.emplace_back(dft_operation, x, N, std::ref(X), start, end);
    }
    // In case N is not a multiple of numThreads
    std::size_t start = (numThreads - 1) * range + low;
    std::size_t end = high;
    /* std::cout << "[ " << start << ", " << end << "]" << std::endl; */
    threads.emplace_back(dft_operation, x, N, std::ref(X), start, end);

    for (std::thread& t : threads) {
        t.join();
    }
}

void dft_MPI(double* x, const long unsigned N, ComplexVector& X, int myRank, int worldSize) {
    std::size_t range = N / static_cast<std::size_t>(worldSize);
    std::size_t low = static_cast<std::size_t>(myRank) * range;
    std::size_t high = (static_cast<std::size_t>(myRank) + 1) * range;
    dft_operation(x, N, X, low, high);

    // gather data from slaves processes into the master process.
    // &X[low]: the address of the local data to be sent from each slave process
    // X.data(): the address of the receive buffer in the master process
    MPI_Gather(&X[low], static_cast<int>(range), MPI_DOUBLE_COMPLEX, X.data(), static_cast<int>(range), MPI_DOUBLE_COMPLEX, 0, MPI_COMM_WORLD);
    // Perform additional computation for the DFT on the range not processed by dft_MPI
    if (myRank == 0) {
        std::size_t remaining_start = (static_cast<std::size_t>(worldSize) * (N / static_cast<std::size_t>(worldSize)));
        std::size_t remaining_end = N;
        dft_operation(x, N, X, remaining_start, remaining_end);
    }
}

void dft_MPI_parallel(double* x, const long unsigned N, ComplexVector& X, int myRank, int worldSize, std::size_t threadNumber) {
    std::size_t range = N / static_cast<std::size_t>(worldSize);
    std::size_t low = static_cast<std::size_t>(myRank) * range;
    std::size_t high = (static_cast<std::size_t>(myRank) + 1) * range;
    std::size_t size = high - low;
    dft_parallel(x, N, X, threadNumber, size, low, high);

    // gather data from slaves processes into the master process.
    // &X[low]: the address of the local data to be sent from each slave process
    // X.data(): the address of the receive buffer in the master process
    MPI_Gather(&X[low], static_cast<int>(range), MPI_DOUBLE_COMPLEX, X.data(), static_cast<int>(range), MPI_DOUBLE_COMPLEX, 0, MPI_COMM_WORLD);
    // Perform additional computation for the DFT on the range not processed by dft_MPI
    if (myRank == 0) {
        std::size_t remaining_start = (static_cast<std::size_t>(worldSize) * (N / static_cast<std::size_t>(worldSize)));
        std::size_t remaining_end = N;
        dft_operation(x, N, X, remaining_start, remaining_end);
    }
}


void fft(double* signal, long unsigned& N, ComplexVector& X) {
    std::vector<double> x(signal, signal + N); // convert to vector to make it easy to resize
    // zero pad the signal to have a power of 2
    double n = log2(d(N));
    int power = static_cast<int>(std::ceil(n));
    if (power != n) { // signal must be zero padded
        N = static_cast<long unsigned>(pow(2, power));
        x.resize(N);
        X.resize(N, 0.0);
    }
    long unsigned Ndiv2 = N/2;
    std::vector<double> x_even(Ndiv2); // f1
    std::vector<double> x_odd(Ndiv2); //f2
    for (size_t i = 0; i < Ndiv2; ++i) {
        x_even[i] = x[i * 2];
        x_odd[i] = x[i * 2 + 1];
    }
    ComplexVector X_even(Ndiv2); // F1
    ComplexVector X_odd(Ndiv2); // F2
    if (Ndiv2 <= 2) {
        dft_sequential(x_even.data(), Ndiv2, X_even);
        dft_sequential(x_odd.data(), Ndiv2, X_odd);
    } else {
        fft(x_even.data(), Ndiv2, X_even);
        fft(x_odd.data(), Ndiv2, X_odd);
    }
    // Combine
    for (size_t k = 0; k < Ndiv2; ++k) {
        Complex t = std::polar(1.0, -2.0 * M_PI * d(k) / d(N)) * X_odd[k];
        X[k] = X_even[k] + t;
        X[k + Ndiv2] = X_even[k] - t;
    }

}

double* getMagnitude(ComplexVector& X) {
    double* arr = new double[X.size()];
    for (std::size_t i = 0; i < X.size(); ++i) {
        arr[i] = std::abs(X[i]);
    }
    return arr;
}

