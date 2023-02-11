#ifndef BARRIER_RESILIENCE_CONFIG_HPP
#define BARRIER_RESILIENCE_CONFIG_HPP

#include "data_structure/data_structure.hpp"
#include "data_structure/trivial.hpp"
#include "functional"

template<class T>
struct Config {
    // Constructor function for data structure.
    std::function<DataStructure<T> *()> data_structure_constructor;

    static Config<T> with_trivial_datastructure() {
        return Config<T>{
                []() -> DataStructure<T> * {
                    return new Trivial<T>();
                }
        };
    }
};


#endif //BARRIER_RESILIENCE_CONFIG_HPP
