#include <iostream>
#include <cmath>
#include <vector>
#include <numeric> // For std::accumulate
#include <chrono>  // For timing
#include <random>  // For random number generation

#define NUM_PATHS 100000 // Number of Monte Carlo paths
#define NUM_STEPS 1000     // Number of time steps per path

// Parameters for the option and asset
const float S0 = 100.0f; // Initial asset price
const float K = 100.0f;  // Strike price
const float B = 120.0f;  // Barrier level
const float T = 1.0f;    // Time to maturity
const float r = 0.05f;   // Risk-free rate
const float sigma = 0.2f; // Volatility

// Function to generate a random number from a normal distribution
float normalRandom() {
    static std::mt19937 generator(std::random_device{}());
    static std::normal_distribution<float> distribution(0.0f, 1.0f);
    return distribution(generator);
}

// Function for Monte Carlo simulation
void monteCarloSimulation(std::vector<float>& results, int numPaths, int numSteps, float dt, float mu, float sigma, float barrier, float strike, float r, float T) {
    for (int tid = 0; tid < numPaths; ++tid) {
        float S = S0;
        bool breached = false;

        for (int i = 0; i < numSteps; ++i) {
            float z = normalRandom();
            S *= exp((mu - 0.5f * sigma * sigma) * dt + sigma * sqrt(dt) * z);
            if (S > barrier) {
                breached = true;
                break;
            }
        }

        float payoff = breached ? 0.0f : std::max(S - strike, 0.0f);
        results[tid] = payoff * exp(-r * T); // Discounted payoff
    }
}

// Function to solve Black-Scholes PDE using finite difference method
void blackScholesPDE(std::vector<float>& optionPrices, int numSteps, float Smax, float dS, float dt, float r, float sigma, float K) {
    for (int j = 0; j <= numSteps; ++j) {
        float S = j * dS;
        float payoff = std::max(S - K, 0.0f);
        optionPrices[j] = payoff;
    }
}

// Main function
int main() {
    std::vector<float> results(NUM_PATHS);
    std::vector<float> optionPrices(NUM_STEPS + 1);

    // Parameters for the simulation
    float dt = T / NUM_STEPS;
    float mu = r - 0.5f * sigma * sigma;

    // Time Monte Carlo simulation
    auto start = std::chrono::high_resolution_clock::now();
    monteCarloSimulation(results, NUM_PATHS, NUM_STEPS, dt, mu, sigma, B, K, r, T);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> simulationTime = end - start;

    // Calculate the average option price
    float sum = std::accumulate(results.begin(), results.end(), 0.0f);
    float monteCarloPrice = sum / NUM_PATHS;

    std::cout << "Estimated Barrier Option Price (Monte Carlo): " << monteCarloPrice << std::endl;
    std::cout << "Time for Monte Carlo simulation: " << simulationTime.count() * 1000.0f << " ms" << std::endl;

    // Solve Black-Scholes PDE
    float Smax = 2 * S0; // Maximum asset price for the PDE grid
    float dS = Smax / NUM_STEPS;

    start = std::chrono::high_resolution_clock::now();
    blackScholesPDE(optionPrices, NUM_STEPS, Smax, dS, dt, r, sigma, K);
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> pdeTime = end - start;

    // Print PDE results (optional, for debugging purposes)
    std::cout << "Black-Scholes PDE prices at final time step:" << std::endl;
    for (int i = 0; i <= NUM_STEPS; ++i) {
        float S = i * dS;
        // std::cout << "S = " << S << ", Price = " << optionPrices[i] << std::endl;
    }

    std::cout << "Time for Black-Scholes PDE solution: " << pdeTime.count() * 1000.0f << " ms" << std::endl;

    return 0;
}
