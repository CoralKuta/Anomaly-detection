/*
 * HybridAnomalyDetector.h
 *
 * Author: Noa Eitan 316222777, Coral Kuta 208649186
 */

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector:public SimpleAnomalyDetector {
public:
	HybridAnomalyDetector(float threshold);
    virtual ~HybridAnomalyDetector();
    virtual void addCorByMec(string feature1, string feature2, float *x, float *y, int size, float correlation) override;
};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
