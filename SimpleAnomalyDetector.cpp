/*
 * SimpleAnomalyDetector.cpp
 *
 * Author: Maor Malekan 321202962
 */

#include "AnomalyDetector.h"
#include "anomaly_detection_util.h"
#include "SimpleAnomalyDetector.h"
#include "timeseries.h"
#include <math.h>
#include <vector>

#define THRESHOLD 0.9

float findMaxThreshold(const std::vector<float>& xVec, const std::vector<float>& yVec, size_t size, const Line& line) {
    float max = 0;
    for (size_t i = 0; i < size; i++) {
        Point p(xVec[i], yVec[i]);
        max = fmax(max, fabs(dev(p, line)));
    }
    return max;
}

void buildArrayPoints(Point** points, float* x, float* y, int size) {
    for (size_t i = 0; i < size; i++) {
        points[i] = new Point(x[i], y[i]);
    }
}

void deletePoints(Point** points, size_t size) {
    for (size_t i = 0; i < size; i++) {
        delete(points[i]);
    }
}

SimpleAnomalyDetector::SimpleAnomalyDetector() {}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {}

void SimpleAnomalyDetector:: learnNormal(const TimeSeries& ts) {
    std::vector<std::string> features = ts.getFeatures();
    size_t size = features.size();
    for (size_t i = 0; i < size - 1; i++) {
        float maxCorrelation = 0;
        int correlatedIndex = -1;
        float maxDistance = 0;
        std::vector<float> x = ts.getValFeature(features[i]);
        std::vector<float> y;
        size_t valuesSize = x.size();
        for (size_t j = i + 1; j < size; j++) {
            y = ts.getValFeature(features[j]);
            float pe = pearson(x.data(), y.data(), valuesSize);
            pe = fabs(pe);
            if (pe >= THRESHOLD && pe > maxCorrelation) {
                maxCorrelation = pe;
                correlatedIndex = j;
            }
        }
        if (maxCorrelation >= THRESHOLD) {
            std::vector<float> correlatedVal = ts.getValFeature(features[correlatedIndex]);
            Point* points[valuesSize];
            buildArrayPoints(points, x.data(), correlatedVal.data(), valuesSize);
            Line line = linear_reg(points, valuesSize);
            deletePoints(points, valuesSize);
            float threshold = 1.2 * findMaxThreshold(x, correlatedVal, valuesSize, line);
            correlatedFeatures c = {features[i], features[correlatedIndex], maxCorrelation, line, threshold};
            cf.push_back(c);
        }          
    }
}

std::vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts){
    std::vector<AnomalyReport> ar;
    for(auto const& value: cf) {
        std::vector<float> xValues = ts.getValFeature(value.feature1);
        std::vector<float> yValues = ts.getValFeature(value.feature2);
        size_t vecSize = xValues.size();
        for (size_t i = 0; i < vecSize; i++) {
            Point p(xValues[i], yValues[i]);
            float d = dev(p, value.lin_reg);
            if (d > value.threshold) {
                std::string desc = value.feature1 + "-" + value.feature2;
                AnomalyReport temp(desc, i + 1);
                ar.push_back(temp);
            }
        }
    }
    return ar;
}

