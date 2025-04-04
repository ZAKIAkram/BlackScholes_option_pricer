#include <gtest/gtest.h>
#include "vbaInterface.hpp"

TEST(VbaInterfaceTest, CopyArray) {
    std::vector<double> data = { 1.0, 2.0, 3.0 };
    SAFEARRAY* outArray = SafeArrayCreateVector(VT_R8, 0, data.size());
    copyArray(data, &outArray);

    double* pData;
    SafeArrayAccessData(outArray, (void**)&pData);
    for (size_t i = 0; i < data.size(); ++i) {
        EXPECT_DOUBLE_EQ(pData[i], data[i]);
    }
    SafeArrayUnaccessData(outArray);
    SafeArrayDestroy(outArray);
}

TEST(VbaInterfaceTest, FlattenAndCopyArray) {
    std::vector<std::vector<double>> data = { {1.0, 2.0}, {3.0, 4.0} };
    SAFEARRAY* outArray = SafeArrayCreateVector(VT_R8, 0, data.size() * data[0].size());
    flattenAndCopyArray(data, &outArray);

    double* pData;
    SafeArrayAccessData(outArray, (void**)&pData);
    for (size_t i = 0; i < data.size(); ++i) {
        for (size_t j = 0; j < data[i].size(); ++j) {
            EXPECT_DOUBLE_EQ(pData[i * data[i].size() + j], data[i][j]);
        }
    }
    SafeArrayUnaccessData(outArray);
    SafeArrayDestroy(outArray);
}