/*
 * timeseries.h
 *
 * Author: Maor Malekan 321202962
 */

#include "timeseries.h"

// The function returns a vector with it's features
const std::vector<std::string> TimeSeries:: getFeatures() const {
    return features;
}

// The function gets feature's name and returns it's values
const std::vector<float> TimeSeries:: getValFeature(std::string feature) const {
    return featuresMap.find(feature)->second;
}


// The function gets feature's name and returns it's values
const std::map<std::string, std::vector<float>> TimeSeries:: getMap() const {
    return featuresMap;
}

void TimeSeries:: addData(std::string values) {
    std::stringstream ss(values);
    std::string value;
    size_t i = 0;
    while (getline(ss, value, ',')) {
        //featuresMap.find(features[i])->second.push_back(std::stof(value));
        featuresMap[features[i]].push_back(std::stof(value));
        i++;
    }
}