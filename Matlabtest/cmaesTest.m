global fitnessFunction;
fitnessFunction = 1;
iterations = [];
dimensions = 10;
startingpoint = 8 * rand(dimensions, 1) - 4;
stepsize = 2;
for i=1:24
	disp(fitnessFunction);
	for j=1:5
		[xmin,fmin,counteval,stopflag,out,bestever] = cmaes('bbob',startingpoint,stepsize);
		disp(stopflag);
		iterations(i,j) = counteval;
	end;
	fitnessFunction+=1;
end;
disp(iterations);
disp(median(iterations,2));
