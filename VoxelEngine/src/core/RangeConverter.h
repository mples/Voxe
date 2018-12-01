#pragma once
#include "input_const.h"

class RangeConverter {
public:

	
	RangeConverter(double min_in, double max_in, double min_out, double max_out, double sens);

	template <typename RangeType>
	RangeType convert(RangeType value) const {
		double val = static_cast<float>(value);

		if (val < minInput_)
			val = minInput_;
		else if (val > maxInput_)
			val = maxInput_;

		double interpol = (val - minInput_) / (maxInput_ - minInput_);
		return static_cast<RangeType>(sensitivity_ * ((interpol * (maxOutput_ - minOutput_)) + minOutput_));
	}
private:
	double minInput_;
	double maxInput_;

	double minOutput_;
	double maxOutput_;

	double sensitivity_;
};

