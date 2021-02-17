/*
 * timeseries.h
 *
 * Author: Maor Malekan 321202962
 */

#ifndef TIMESERIES_H_
#define TIMESERIES_H_

#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>

class TimeSeries{

    std::vector<std::string> features;
    std::map<std::string, std::vector<float>> featuresMap;
    
    public:
        void initFeatures(std::string line) {            
            std::stringstream ss(line);
            std::string feature;
            size_t i = 0;
            while (getline(ss, feature, ',')) {
                features.push_back(feature);
                std::vector<float> v;
                featuresMap.insert({feature, v});
            }            
        }

        TimeSeries(std::string csv){
            std::ifstream my_file(csv);
            std::string line;
            bool first_line = true;
            if (my_file.is_open()) {
                while (getline (my_file, line)) {
                    if (!first_line) {
                        addData(line);
                    }
                    else {
                        initFeatures(line);
                    }
                    first_line = false;
                }
                my_file.close();
            }
        }

        // The function returns a vector with it's features
        const std::vector<std::string> getFeatures() const;

        // The function gets feature's name and returns it's values
        const std::vector<float> getValFeature(std::string feature) const;


        // The function gets feature's name and returns it's values
        const std::map<std::string, std::vector<float>> getMap() const;

        void addData(std::string values);
};

#endif /* TIMESERIES_H_ */

