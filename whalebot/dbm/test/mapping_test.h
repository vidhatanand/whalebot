#pragma once

#include <gtest/gtest.h>
#include <mappers.h>

namespace korm {
namespace test {

template<class T>
bool testTMapping(const T& testValue)
{
    T                                       result(0);
    typename CTraits<T>::CDefaultMapper     mapper(testValue);
    typename CTraits<T>::CDefaultUnMapper   unMapper(result);

    unMapper.unMap(mapper.dataPointer(), mapper.dataSize());

    EXPECT_EQ(testValue, result) << " Mapping failed mapping from '"
                                 << testValue
                                 << "' mapped to '"
                                 << result << "'" << std::endl;    
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
    
}//test
}// korm 
