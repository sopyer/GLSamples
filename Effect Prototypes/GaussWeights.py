#~ TODO: add code to evalute coefficients for bilinear assisted gaussian blur
#~ Comment from http://http://rastergrid.com/blog/2010/09/efficient-gaussian-blur-with-linear-sampling/:
#~ I, I know this is an old post, but I just found it and was doing something similar in my engine.
#~ However, the formula I was using for weight offsets is:
#~ offA + (wB / (wA+wB))
#~ You can derive this from your formula (assuming that offset B is offset A + 1):
#~ (offA*wA + offB*wB)/(wA+wB)
#~ = (offA*wA + (offA+1)*wB)/(wA+wB)
#~ = (offA*wA + offA*wB + wB)/(wA+wB)
#~ = (offA*(wA + wB) + wB)/(wA+wB)
#~ = (offA*(wA + wB)) / (wA+wB) + wB/(wA+wB)
#~ = offA + wB/(wA+wB)

import sys

def GenerateGaussianFilter(taps) :	
	weights = []
	sum = 0.0	
	w = 1.0

	width = (taps-1)*2	

	for i in range((width)/2+1) :	
		weights.append(w)
		sum = sum + w		
		w = w * (width-i) / (i+1)

	weights.reverse()

	sum =  weights[0] + (sum - weights[0])*2.0

	if sum > 0 :
		for i in range(len(weights)) :
			weights[i] = weights[i] / sum

	return weights

def GenerateGaussianFilterBilinear(weights):
	weightsBilinear = []
	offsetsBilinear = []
	count = len(weights)/2
	first = 0;
	if (len(weights)%2!=0):
		first=1
		weightsBilinear = [weights[0]]
		offsetsBilinear = [0.0]

	for i in range(count):
		idx = i*2+first
		wA = weights[idx]
		wB = weights[idx+1]
		weightsBilinear.append(wA+wB)
		offsetsBilinear.append(idx+wB/(wA+wB))
		
	return (offsetsBilinear, weightsBilinear)

weights = GenerateGaussianFilter(5)
print "Gaussian weights", weights
print "Bilinear gaussian weights and offsets", GenerateGaussianFilterBilinear(weights)
