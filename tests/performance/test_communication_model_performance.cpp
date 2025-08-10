#include <gtest/gtest.h>
#include "CommunicationModelAPI.h"
#include "MathConstants.h"
#include <memory>
#include <chrono>
#include <vector>
#include <random>
#include <thread>

/**
 * @brief 通信模型性能测试类
 */
class CommunicationModelPerformanceTest : public ::testing::Test {
protected:
    void SetUp() override {
        api = std::make_unique<CommunicationModelAPI>();
        
        // 设置标准测试环境
        CommunicationEnvironment env;
        env.frequency = 2400.0;
        env.bandwidth = 20.0;
        env.transmitPower = 30.0;
        env.noisePower = -90.0;
        env.distance = 5.0;
        env.environmentType = EnvironmentType::OPEN_FIELD;
        api->setEnvironment(env);
    }

    void TearDown() override {
        api.reset();
    }

    std::unique_ptr<CommunicationModelAPI> api;
    
    // 性能测试辅助方法
    template<typename Func>
    double measureExecutionTime(Func&& func, int iterations = 1000) {
        auto start = std::chrono::high_resolution_clock::now();
        
        for (int i = 0; i < iterations; ++i) {
            func();
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        return static_cast<double>(duration.count()) / iterations; // 平均执行时间（微秒）
    }
    
    // 生成随机环境参数
    CommunicationEnvironment generateRandomEnvironment() {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_real_distribution<> freqDist(1000.0, 6000.0);
        static std::uniform_real_distribution<> bwDist(5.0, 100.0);
        static std::uniform_real_distribution<> powerDist(10.0, 50.0);
        static std::uniform_real_distribution<> distDist(0.1, 50.0);
        static std::uniform_int_distribution<> envDist(0, 3);
        
        CommunicationEnvironment env;
        env.frequency = freqDist(gen);
        env.bandwidth = bwDist(gen);
        env.transmitPower = powerDist(gen);
        env.noisePower = -90.0;
        env.distance = distDist(gen);
        env.environmentType = static_cast<EnvironmentType>(envDist(gen));
        env.temperature = 20.0;
        env.humidity = 50.0;
        env.atmosphericPressure = 1013.25;
        
        return env;
    }
};

/**
 * @brief 测试基本计算性能
 */
TEST_F(CommunicationModelPerformanceTest, BasicCalculationPerformance) {
    const int iterations = 1000;
    
    // 测试链路状态计算性能
    double linkStatusTime = measureExecutionTime([this]() {
        api->calculateLinkStatus();
    }, iterations);
    
    EXPECT_LT(linkStatusTime, 1000.0); // 期望每次计算少于1毫秒
    std::cout << "链路状态计算平均时间: " << linkStatusTime << " 微秒" << std::endl;
    
    // 测试性能计算性能
    double performanceTime = measureExecutionTime([this]() {
        api->calculatePerformance();
    }, iterations);
    
    EXPECT_LT(performanceTime, 1500.0); // 期望每次计算少于1.5毫秒
    std::cout << "性能计算平均时间: " << performanceTime << " 微秒" << std::endl;
    
    // 测试通信距离计算性能
    double rangeTime = measureExecutionTime([this]() {
        api->calculateCommunicationRange();
    }, iterations);
    
    EXPECT_LT(rangeTime, 500.0); // 期望每次计算少于0.5毫秒
    std::cout << "通信距离计算平均时间: " << rangeTime << " 微秒" << std::endl;
}

/**
 * @brief 测试参数设置性能
 */
TEST_F(CommunicationModelPerformanceTest, ParameterSettingPerformance) {
    const int iterations = 10000;
    
    // 测试频率设置性能
    double freqSetTime = measureExecutionTime([this]() {
        api->setFrequency(2400.0 + (rand() % 1000));
    }, iterations);
    
    EXPECT_LT(freqSetTime, 100.0); // 期望每次设置少于0.1毫秒
    std::cout << "频率设置平均时间: " << freqSetTime << " 微秒" << std::endl;
    
    // 测试环境设置性能
    std::vector<CommunicationEnvironment> environments;
    for (int i = 0; i < 100; ++i) {
        environments.push_back(generateRandomEnvironment());
    }
    
    double envSetTime = measureExecutionTime([this, &environments]() {
        static int index = 0;
        api->setEnvironment(environments[index % environments.size()]);
        index++;
    }, iterations);
    
    EXPECT_LT(envSetTime, 200.0); // 期望每次设置少于0.2毫秒
    std::cout << "环境设置平均时间: " << envSetTime << " 微秒" << std::endl;
}

/**
 * @brief 测试缓存性能
 */
TEST_F(CommunicationModelPerformanceTest, CachingPerformance) {
    const int iterations = 1000;
    
    // 第一次计算（无缓存）
    double firstCalculationTime = measureExecutionTime([this]() {
        api->calculateLinkStatus();
    }, 1);
    
    // 后续计算（使用缓存）
    double cachedCalculationTime = measureExecutionTime([this]() {
        api->calculateLinkStatus();
    }, iterations);
    
    // 缓存计算应该显著更快
    EXPECT_LT(cachedCalculationTime, firstCalculationTime * 0.1);
    std::cout << "首次计算时间: " << firstCalculationTime << " 微秒" << std::endl;
    std::cout << "缓存计算平均时间: " << cachedCalculationTime << " 微秒" << std::endl;
    
    // 测试缓存失效后的性能
    api->setFrequency(2500.0); // 使缓存失效
    
    double invalidatedCacheTime = measureExecutionTime([this]() {
        api->calculateLinkStatus();
    }, 1);
    
    // 缓存失效后应该接近首次计算时间
    EXPECT_GT(invalidatedCacheTime, cachedCalculationTime * 5);
    std::cout << "缓存失效后计算时间: " << invalidatedCacheTime << " 微秒" << std::endl;
}

/**
 * @brief 测试批量分析性能
 */
TEST_F(CommunicationModelPerformanceTest, BatchAnalysisPerformance) {
    // 测试多场景分析性能
    std::vector<CommunicationScenario> scenarios = {
        CommunicationScenario::NORMAL_COMMUNICATION,
        CommunicationScenario::JAMMED_COMMUNICATION,
        CommunicationScenario::ANTI_JAM_COMMUNICATION,
        CommunicationScenario::MULTI_USER_COMMUNICATION,
        CommunicationScenario::RELAY_COMMUNICATION
    };
    
    double multiScenarioTime = measureExecutionTime([this, &scenarios]() {
        api->analyzeMultipleScenarios(scenarios);
    }, 100);
    
    EXPECT_LT(multiScenarioTime, 5000.0); // 期望少于5毫秒
    std::cout << "多场景分析平均时间: " << multiScenarioTime << " 微秒" << std::endl;
    
    // 测试频率范围分析性能
    double freqRangeTime = measureExecutionTime([this]() {
        api->analyzeFrequencyRange(2000.0, 3000.0, 100.0);
    }, 10);
    
    EXPECT_LT(freqRangeTime, 20000.0); // 期望少于20毫秒
    std::cout << "频率范围分析平均时间: " << freqRangeTime << " 微秒" << std::endl;
    
    // 测试功率范围分析性能
    double powerRangeTime = measureExecutionTime([this]() {
        api->analyzePowerRange(10.0, 50.0, 5.0);
    }, 10);
    
    EXPECT_LT(powerRangeTime, 15000.0); // 期望少于15毫秒
    std::cout << "功率范围分析平均时间: " << powerRangeTime << " 微秒" << std::endl;
}

/**
 * @brief 测试网络分析性能
 */
TEST_F(CommunicationModelPerformanceTest, NetworkAnalysisPerformance) {
    // 创建不同规模的网络节点
    std::vector<int> networkSizes = {5, 10, 20, 50};
    
    for (int size : networkSizes) {
        std::vector<std::pair<double, double>> nodes;
        for (int i = 0; i < size; ++i) {
            nodes.emplace_back(i * 1.0, (i % 5) * 1.0);
        }
        
        double linkMatrixTime = measureExecutionTime([this, &nodes]() {
            api->calculateLinkMatrix(nodes);
        }, 10);
        
        std::cout << "网络规模 " << size << " 链路矩阵计算时间: " 
                  << linkMatrixTime << " 微秒" << std::endl;
        
        // 验证计算时间随网络规模的增长是合理的
        EXPECT_LT(linkMatrixTime, size * size * 100.0); // 期望复杂度不超过O(n²)
        
        double connectivityTime = measureExecutionTime([this, &nodes]() {
            api->calculateNetworkConnectivity(nodes);
        }, 10);
        
        std::cout << "网络规模 " << size << " 连通性计算时间: " 
                  << connectivityTime << " 微秒" << std::endl;
        
        EXPECT_LT(connectivityTime, size * 200.0); // 期望线性复杂度
    }
}

/**
 * @brief 测试内存使用性能
 */
TEST_F(CommunicationModelPerformanceTest, MemoryUsagePerformance) {
    const int iterations = 1000;
    
    // 测试大量API实例创建和销毁
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < iterations; ++i) {
        auto tempApi = std::make_unique<CommunicationModelAPI>();
        tempApi->setFrequency(2400.0);
        tempApi->calculateLinkStatus();
        // tempApi 自动销毁
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    double avgCreationTime = static_cast<double>(duration.count()) / iterations;
    
    EXPECT_LT(avgCreationTime, 1000.0); // 期望每次创建少于1毫秒
    std::cout << "API实例创建销毁平均时间: " << avgCreationTime << " 微秒" << std::endl;
}

/**
 * @brief 测试并发性能
 */
TEST_F(CommunicationModelPerformanceTest, ConcurrencyPerformance) {
    const int numThreads = 4;
    const int iterationsPerThread = 250;
    
    auto threadFunction = [this](int threadId) {
        auto localApi = std::make_unique<CommunicationModelAPI>();
        
        // 每个线程使用不同的参数
        localApi->setFrequency(2400.0 + threadId * 100.0);
        localApi->setTransmitPower(30.0 + threadId * 5.0);
        
        for (int i = 0; i < iterationsPerThread; ++i) {
            localApi->setDistance(1.0 + (i % 10));
            auto status = localApi->calculateLinkStatus();
            EXPECT_TRUE(status.signalStrength > -200.0);
        }
    };
    
    auto start = std::chrono::high_resolution_clock::now();
    
    std::vector<std::thread> threads;
    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back(threadFunction, i);
    }
    
    for (auto& thread : threads) {
        thread.join();
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    std::cout << "并发测试总时间: " << duration.count() << " 毫秒" << std::endl;
    std::cout << "总计算次数: " << numThreads * iterationsPerThread << std::endl;
    
    // 验证并发性能合理
    EXPECT_LT(duration.count(), 5000); // 期望总时间少于5秒
}

/**
 * @brief 测试优化算法性能
 */
TEST_F(CommunicationModelPerformanceTest, OptimizationPerformance) {
    const int iterations = 50;
    
    // 测试距离优化性能
    double rangeOptTime = measureExecutionTime([this]() {
        api->optimizeForRange(10.0);
    }, iterations);
    
    EXPECT_LT(rangeOptTime, 10000.0); // 期望少于10毫秒
    std::cout << "距离优化平均时间: " << rangeOptTime << " 微秒" << std::endl;
    
    // 测试数据速率优化性能
    double dataRateOptTime = measureExecutionTime([this]() {
        api->optimizeForDataRate(50.0);
    }, iterations);
    
    EXPECT_LT(dataRateOptTime, 10000.0); // 期望少于10毫秒
    std::cout << "数据速率优化平均时间: " << dataRateOptTime << " 微秒" << std::endl;
    
    // 测试功率效率优化性能
    double powerEffOptTime = measureExecutionTime([this]() {
        api->optimizeForPowerEfficiency();
    }, iterations);
    
    EXPECT_LT(powerEffOptTime, 10000.0); // 期望少于10毫秒
    std::cout << "功率效率优化平均时间: " << powerEffOptTime << " 微秒" << std::endl;
    
    // 测试抗干扰优化性能
    double jammerResOptTime = measureExecutionTime([this]() {
        api->optimizeForJammerResistance();
    }, iterations);
    
    EXPECT_LT(jammerResOptTime, 10000.0); // 期望少于10毫秒
    std::cout << "抗干扰优化平均时间: " << jammerResOptTime << " 微秒" << std::endl;
}

/**
 * @brief 测试大数据量处理性能
 */
TEST_F(CommunicationModelPerformanceTest, LargeDataProcessingPerformance) {
    // 测试大范围频率分析
    auto start = std::chrono::high_resolution_clock::now();
    
    auto results = api->analyzeFrequencyRange(1000.0, 6000.0, 10.0);
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    std::cout << "大范围频率分析时间: " << duration.count() << " 毫秒" << std::endl;
    std::cout << "分析点数: " << results.size() << std::endl;
    
    EXPECT_LT(duration.count(), 10000); // 期望少于10秒
    EXPECT_GT(results.size(), 400); // 期望有足够的数据点
    
    // 测试大规模网络分析
    std::vector<std::pair<double, double>> largeNetwork;
    for (int i = 0; i < 100; ++i) {
        for (int j = 0; j < 10; ++j) {
            largeNetwork.emplace_back(i * 0.5, j * 0.5);
        }
    }
    
    start = std::chrono::high_resolution_clock::now();
    
    auto connectivity = api->calculateNetworkConnectivity(largeNetwork);
    
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    std::cout << "大规模网络连通性分析时间: " << duration.count() << " 毫秒" << std::endl;
    std::cout << "网络节点数: " << largeNetwork.size() << std::endl;
    
    EXPECT_LT(duration.count(), 30000); // 期望少于30秒
    EXPECT_GE(connectivity, 0.0);
    EXPECT_LE(connectivity, 1.0);
}

/**
 * @brief 测试稳定性和可靠性
 */
TEST_F(CommunicationModelPerformanceTest, StabilityAndReliability) {
    const int longRunIterations = 10000;
    
    // 长时间运行稳定性测试
    auto start = std::chrono::high_resolution_clock::now();
    
    int successCount = 0;
    for (int i = 0; i < longRunIterations; ++i) {
        try {
            // 随机改变参数
            auto env = generateRandomEnvironment();
            api->setEnvironment(env);
            
            auto status = api->calculateLinkStatus();
            
            // 验证结果的合理性
            if (status.signalStrength > -200.0 && status.signalStrength < 100.0 &&
                status.bitErrorRate >= 0.0 && status.bitErrorRate <= 1.0 &&
                status.throughput >= 0.0) {
                successCount++;
            }
        } catch (const std::exception& e) {
            // 记录异常但继续测试
            std::cout << "异常在迭代 " << i << ": " << e.what() << std::endl;
        }
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    
    double successRate = static_cast<double>(successCount) / longRunIterations;
    
    std::cout << "长时间运行测试:" << std::endl;
    std::cout << "总迭代次数: " << longRunIterations << std::endl;
    std::cout << "成功次数: " << successCount << std::endl;
    std::cout << "成功率: " << successRate * 100.0 << "%" << std::endl;
    std::cout << "总运行时间: " << duration.count() << " 秒" << std::endl;
    
    // 期望成功率至少95%
    EXPECT_GE(successRate, 0.95);
    
    // 期望平均每次计算时间合理
    double avgTimePerIteration = static_cast<double>(duration.count() * 1000) / longRunIterations;
    EXPECT_LT(avgTimePerIteration, 5.0); // 期望每次少于5毫秒
}