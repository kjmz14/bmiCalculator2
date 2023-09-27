/*
 * MIT License
 *
 * Copyright (c) 2023 Karol Zimmer
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "bmi.h"
#include <math.h>

UNITS_TYPE BMIUnits = BMIMetric;

double BMICalculate(BMIHeightStruct height, unsigned int mass)
{

	double dInches;

	if (BMIUnits == BMIMetric)
	{
		if (height.cmFt == 0)
			return 0;

		double m = (double)height.cmFt / 100;
		return (double)mass / pow(m, 2);
	}

	height.in += 12 * height.cmFt;

	if (height.in == 0)
		return 0;

	dInches = (double)height.in;
	return 703 * (double)mass / pow(dInches, 2);

}