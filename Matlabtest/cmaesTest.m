global fitnessFunction;
fitnessFunction = 1;
iterations = [];
for i=1:24
	disp(fitnessFunction);
	for j=1:5
		[xmin,fmin,counteval,stopflag,out,bestever] = cmaes('bbob',2*rand(10,1)-1,2);
		disp(stopflag);
		iterations(i,j) = counteval;
	end;
	fitnessFunction+=1;
end;
disp(iterations);
disp(median(iterations,2));
