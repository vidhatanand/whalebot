#pragma once

#include <gtest/gtest.h>
#include <mappers.h>

static const unsigned int kDefualtVectorSize    =   10000;

namespace korm {
namespace test {

template<class T>
bool testTMapping(const T& testValue)
{
    T                                       result;
    typename CTraits<T>::CDefaultMapper     mapper(testValue);
    typename CTraits<T>::CDefaultUnMapper   unMapper(result);

    unMapper.unMap(mapper.dataPointer(), mapper.dataSize());

    EXPECT_TRUE(testValue == result);
}

template<class T>
bool testTVector()
{
    std::vector<T>  testValue(kDefualtVectorSize, static_cast<T>(0));

    for (unsigned int i = 0; i != kDefualtVectorSize; ++i) {
        testValue.push_back(static_cast<T>(i));
    }
    return testTMapping(testValue);
}

TEST(MappingTest, CharTest)
{
    testTMapping<int>(24);
}

TEST(MappingTest, IntTest)
{
    testTMapping<int>(-32);    
}

TEST(MappingTest, UIntTest)
{
    testTMapping<unsigned int>(8);
}

TEST(MappingTest, FloatTest)
{
    testTMapping<float>(4.4);
}

TEST(MappingTest, DoubleTest)
{
    testTMapping<double>(0.5456677);
}

TEST(MappingTest, StringTest)
{
    std::string testValue("test_test_test_test_test_test_test_test");
    testTMapping(testValue);
}

TEST(MappingTest, WStringTest)
{
    std::wstring testValue(L"test_test_test_test_test_test_test_test");
    testTMapping(testValue);
}

TEST(MappingTest, VectorCharTest)
{
    testTVector<char>();
}

TEST(MappingTest, VectorIntTest)
{
    testTVector<int>();
}

TEST(MappingTest, VectorDoubleTest)
{
    testTVector<double>();
}

TEST(MappingTest, VectorFloatTest)
{
    testTVector<float>();
}
    
}//test
}// korm 
