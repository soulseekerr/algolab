#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <chrono>
#include <mutex>

class BenchmarkLogger {
public:
    static BenchmarkLogger& get() {
        static BenchmarkLogger instance;
        return instance;
    }

    void log(const std::string& label, long long ns) {
        std::scoped_lock lock(mutex_);
        entries.emplace_back(label, ns);
    }

    void writeCSV(const std::string& filename = "benchmark_report.csv") {
        std::ofstream out(filename);
        out << "Label,Time_ns,Time_us,Time_ms\n";
        for (const auto& [label, ns] : entries) {
            out << label << "," << ns << ","
                << ns / 1000.0 << "," << ns / 1'000'000.0 << "\n";
        }
    }

    void prettyPrint() {
        for (const auto& [label, ns] : entries) {
            if (ns >= 1'000'000'000) {
                std::cout << label << ": " << ns / 1'000'000'000.0 << " s\n";
            } else if (ns >= 1'000'000) {
                std::cout << label << ": " << ns / 1'000'000.0 << " ms\n";
            } else if (ns >= 1'000) {
                std::cout << label << ": " << ns / 1000.0 << " µs\n";
            } else if (ns >= 1) {
                std::cout << label << ": " << ns << " ns \n";
            } else {
                std::cout << label << ": " << ns << " ns \n";
            }
        }
    }

private:
    BenchmarkLogger() = default;

    std::vector<std::pair<std::string, long long>> entries;
    std::mutex mutex_;
};
