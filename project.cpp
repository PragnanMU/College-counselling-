#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <functional>
#include <stdexcept>

namespace CollegeCounseling {
    // Abstract base class for allocation strategies
    class AllocationStrategy {
    public:
        virtual std::string allocateCollege(int userRank) const = 0;
    };

    // Derived class implementing an allocation strategy based on rank intervals
    class RankIntervalStrategy : public AllocationStrategy {
    private:
        // Structure to hold data about colleges and rank intervals
        struct CollegeData {
            int rankStart;
            int rankEnd;
            std::string college;
        };

        // Vector to store college data
        std::vector<CollegeData> collegesData;

        // Static member to track the total number of instances
        static int totalInstances;

    public:
        // Parameterized constructor, loads college data from a file
        RankIntervalStrategy(const std::string& dataFile) {
            loadCollegesData(dataFile);
            totalInstances++;
        }

        // Delegating constructor, uses a default data file
        RankIntervalStrategy() : RankIntervalStrategy("default_data.txt") {}

        // Override of the virtual function to allocate a college based on rank
        std::string allocateCollege(int userRank) const override {
            for (const CollegeData& data : collegesData) {
                if (userRank >= data.rankStart && userRank <= data.rankEnd) {
                    return data.college;
                }
            }
            return "No college allocated for your rank.";
        }

        // Static method to get the total number of instances
        static int getTotalInstances() {
            return totalInstances;
        }

    private:
        // Private method to load colleges data from a file
        void loadCollegesData(const std::string& dataFile) {
            std::ifstream file(dataFile);
            if (!file.is_open()) {
                throw std::runtime_error("Error: Cannot open data file.");
            }

            std::string line;
            while (getline(file, line)) {
                size_t colonPos = line.find(':');
                if (colonPos == std::string::npos) {
                    throw std::runtime_error("Error: Invalid data format in the data file.");
                }

                // Parsing rank range and college name
                std::string rankRange = line.substr(0, colonPos);
                std::string college = line.substr(colonPos + 1);

                size_t hyphenPos = rankRange.find('-');
                if (hyphenPos == std::string::npos) {
                    throw std::runtime_error("Error: Invalid rank range in the data file.");
                }

                // Extracting rank start and end values
                int rankStart = std::stoi(rankRange.substr(0, hyphenPos));
                int rankEnd = std::stoi(rankRange.substr(hyphenPos + 1));

                // Adding college data to the vector
                collegesData.push_back({ rankStart, rankEnd, college });
            }

            file.close();
        }
    };

    // Initializing the static member of RankIntervalStrategy
    int RankIntervalStrategy::totalInstances = 0;

    // Derived classes with alternative allocation strategies
    class AnotherStrategy : public AllocationStrategy {
    public:
        // Override of the virtual function with a different allocation logic
        std::string allocateCollege(int userRank) const override {
            // Implement your allocation logic here
            return "not eligible for round two";
        }
    };

    class YetAnotherStrategy : public AllocationStrategy {
    public:
        // Override of the virtual function with another allocation logic
        std::string allocateCollege(int userRank) const override {
            // Implement your allocation logic here
            return "not eligible for round three";
        }
    };

    // Class representing a college application
    class CollegeApplication {
    private:
        std::string applicantName;
        int applicantRank;

    public:
        // Parameterized constructor for creating a college application
        CollegeApplication(const std::string& name, int rank)
            : applicantName(name), applicantRank(rank) {}

        // Getter for the applicant's name
        std::string getApplicantName() const {
            return applicantName;
        }

        // Getter for the applicant's rank
        int getApplicantRank() const {
            return applicantRank;
        }
    };

    // Class providing a static method for college allocation
    class CollegeAdmissionSystem {
    public:
        // Static method to allocate a college based on a strategy and application
        static std::string allocateCollege(const AllocationStrategy& strategy, const CollegeApplication& application) {
            return strategy.allocateCollege(application.getApplicantRank());
        }
    };
}

// Forward declaration for the displayAllocationResult function
void displayAllocationResult(const std::string& result);

// Lambda function to get rank allocation using a strategy and application
auto getRankAllocation = [](const CollegeCounseling::AllocationStrategy& strategy, const CollegeCounseling::CollegeApplication& application) {
    return CollegeCounseling::CollegeAdmissionSystem::allocateCollege(strategy, application);
};


// Main function
int main() {
    try {
        std::ifstream pathFile("data.txt");
        if (!pathFile.is_open()) {
            throw std::runtime_error("Error: Cannot open data.txt");
        }

        std::string projectFilePath;
        std::getline(pathFile, projectFilePath); // Read the complete path from data.txt
        pathFile.close();

        // Open project.txt using the complete path from data.txt
        std::ifstream projectFile(projectFilePath);

        if (!projectFile.is_open()) {
            throw std::runtime_error("Error: Cannot open project.txt");
        }

       

        // Close project.txt
        projectFile.close();

        // Rest of the code remains the same

        // User input for name
        std::cout << "Enter your name: ";
        std::string userName;
        std::getline(std::cin >> std::ws, userName); // Allowing spaces in the name

        // User input for rank
        std::cout << "Enter your rank: ";
        int userRank;
        if (!(std::cin >> userRank)) {
        // If reading fails, throw an exception
        throw std::runtime_error("Error: Invalid input for rank. Please enter a valid integer.");
    }

        // Creating instances of different strategies and a college application
        CollegeCounseling::RankIntervalStrategy rankStrategy(projectFilePath);
        CollegeCounseling::AnotherStrategy anotherStrategy;
        CollegeCounseling::YetAnotherStrategy yetAnotherStrategy;
        CollegeCounseling::CollegeApplication application(userName, userRank);

        // Getting and displaying the result for each strategy
        std::string resultRank = getRankAllocation(rankStrategy, application);
        displayAllocationResult(resultRank);

        std::string resultAnother = getRankAllocation(anotherStrategy, application);
        displayAllocationResult(resultAnother);

        std::string resultYetAnother = getRankAllocation(yetAnotherStrategy, application);
        displayAllocationResult(resultYetAnother);

        // Displaying the total instances of RankIntervalStrategy
        std::cout << "Total instances of RankIntervalStrategy: " << CollegeCounseling::RankIntervalStrategy::getTotalInstances() << std::endl;
    } catch (const std::exception& e) {
        // Handling exceptions and displaying error messages
        std::cerr << e.what() << std::endl;
    }

    return 0;
}


// Definition of the displayAllocationResult function
void displayAllocationResult(const std::string& result) {
    std::cout << "Result: " << result << std::endl;
}
