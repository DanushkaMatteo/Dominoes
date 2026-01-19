#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <vector>
#include <chrono>   // Required for timing
#include <iomanip>  // Required for formatting output

#include "dominolinebuilder.h"

// Helper struct to manage dataset configuration
struct Dataset {
    unsigned long int size;
    std::string filePath;
};

int main()
{
    // Define the datasets you want to measure.
    // IMPORTANT: Ensure these file paths match your actual directory structure.
    // The sizes should match the files you have available (e.g., 10, 100, 1000, etc.)
    std::vector<Dataset> datasets = {
        {10, "../dominoes-test_data/10/10-input.txt"},
        {20, "../dominoes-test_data/20/20-input.txt"},
        {30, "../dominoes-test_data/30/30-input.txt"},
        {40, "../dominoes-test_data/40/40-input.txt"},
        {50, "../dominoes-test_data/50/50-input.txt"},
        {100, "../dominoes-test_data/100/100-input.txt"},
        {200, "../dominoes-test_data/200/200-input.txt"},
        {300, "../dominoes-test_data/300/300-input.txt"},
        {400, "../dominoes-test_data/400/400-input.txt"},
        {500, "../dominoes-test_data/500/500-input.txt"},
        {1000, "../dominoes-test_data/1000/1000-input.txt"},
        {2000, "../dominoes-test_data/2000/2000-input.txt"},
        {3000, "../dominoes-test_data/3000/3000-input.txt"},
        {4000, "../dominoes-test_data/4000/4000-input.txt"},
        {5000, "../dominoes-test_data/5000/5000-input.txt"},
        {10000, "../dominoes-test_data/10000/10000-input.txt"},
        {20000, "../dominoes-test_data/20000/20000-input.txt"},
        {30000, "../dominoes-test_data/30000/30000-input.txt"},
    };

    // Open a CSV file to store the results
    std::ofstream csvFile("performance_results.csv");
    if (!csvFile.is_open()) {
        std::cerr << "Error: Could not create CSV file." << std::endl;
        return 1;
    }

    // Write CSV Header
    csvFile << "DatasetSize,Constructor(us),NextRightLoop(us),DisplayLine(us),TotalTime(us)\n";

    // Setup console output formatting
    std::cout << std::left << std::setw(15) << "Size"
              << std::setw(20) << "Constructor (us)"
              << std::setw(20) << "Build Loop (us)"
              << std::setw(20) << "Display (us)"
              << std::setw(20) << "Total (us)" << std::endl;
    std::cout << std::string(95, '-') << std::endl;

    for (const auto& data : datasets) {
        std::ifstream theInputStream(data.filePath);
        if (!theInputStream.good()) {
            std::cerr << "Error: Cannot read from file: " << data.filePath << std::endl;
            continue; // Skip this dataset if file not found
        }

        // --- 1. Measure Constructor ---
        auto startConstructor = std::chrono::high_resolution_clock::now();

        DominoLineBuilder theDominoLine(data.size, theInputStream);

        auto endConstructor = std::chrono::high_resolution_clock::now();


        // --- 2. Measure nextRight() Loop (Building the Line) ---
        auto startBuild = std::chrono::high_resolution_clock::now();

        while (theDominoLine.nextRight());

        auto endBuild = std::chrono::high_resolution_clock::now();


        // --- 3. Measure displayLine() ---
        // Note: Writing to std::cout can be slow.
        // We capture the output to a dummy stream to measure pure logic speed,
        // or keep std::cout if I/O is part of the required measurement.
        // Below measures writing to a 'null' stream to avoid console clutter during tests.
        // If you must print to console, change 'nullStream' to 'std::cout'.
        std::ofstream nullStream("/dev/null"); // Linux specific (use "nul" for Windows)
        auto startDisplay = std::chrono::high_resolution_clock::now();

        if(nullStream.good()) {
             theDominoLine.displayLine(nullStream);
        } else {
             // Fallback if /dev/null doesn't exist (e.g. simple test)
             // theDominoLine.displayLine(std::cout);
        }

        auto endDisplay = std::chrono::high_resolution_clock::now();


        // --- Calculate Durations in Microseconds ---
        auto durationConstructor = std::chrono::duration_cast<std::chrono::microseconds>(endConstructor - startConstructor).count();
        auto durationBuild = std::chrono::duration_cast<std::chrono::microseconds>(endBuild - startBuild).count();
        auto durationDisplay = std::chrono::duration_cast<std::chrono::microseconds>(endDisplay - startDisplay).count();
        auto totalTime = durationConstructor + durationBuild + durationDisplay;

        // --- Output to Console ---
        std::cout << std::left << std::setw(15) << data.size
                  << std::setw(20) << durationConstructor
                  << std::setw(20) << durationBuild
                  << std::setw(20) << durationDisplay
                  << std::setw(20) << totalTime << std::endl;

        // --- Output to CSV File ---
        csvFile << data.size << ","
                << durationConstructor << ","
                << durationBuild << ","
                << durationDisplay << ","
                << totalTime << "\n";
    }

    std::cout << "\nPerformance testing complete. Results saved to 'performance_results.csv'." << std::endl;
    csvFile.close();

    return 0;
}