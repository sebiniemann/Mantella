global fitnessFunction;
fitnessFunction = 1;
iterations = [];
dimensions = 2;
startingpoint = 8 * rand(dimensions, 1) - 4;
stepsize = 2;
opts.MaxFunEvals = 20000;
% opts.TolX = '1e-12*max(insigma)';
% opts.TolUpX = Inf;
% opts.TolHistFun = 1e-12;
% opts.StopOnEqualFunctionValues = Inf;
% opts.CMA.cs = '(mueff+2)/(N+mueff+5)';
% opts.CMA.ccum = '((N+4 + 2*mueff/N) / (4 + mueff/N))^-1';
FHANDLES = benchmarks('handles');

for i=1:1
	disp(fitnessFunction);
	actFunc = FHANDLES{i};
    optimalObjectiveValue = feval(actFunc, 'init', dimensions, 0);
	opts.StopFitness = optimalObjectiveValue + 10^floor(log10(abs(optimalObjectiveValue))) * 1e-3;
	for j=1:1
		[xmin,fmin,counteval,stopflag,out,bestever] = cmaes('bbob',startingpoint,stepsize,opts);
		stopflag
		iterations(i,j) = counteval;
	end;
	fitnessFunction=fitnessFunction+1;
end;
iterations;
median(iterations,2)
