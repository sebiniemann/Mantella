global fitnessFunction;
fitnessFunction = 1;
iterations = [];
dimensions = 5;
startingpoint = 8 * rand(dimensions, 1) - 4;
stepsize = 2;
opts.MaxFunEvals = 20000;
opts.StopOnStagnation = 'off';
opts.StopOnWarnings = 'off';
opts.StopOnEqualFunctionValues = 0;
FHANDLES = benchmarks('handles');

for i=1:5
	fitnessFunction = i;
	disp(['function: ' num2str(fitnessFunction)]);
	actFunc = FHANDLES{i};
    optimalObjectiveValue = feval(actFunc, 'init', dimensions, 0);
	opts.StopFitness = optimalObjectiveValue + 10^floor(log10(abs(optimalObjectiveValue))) * 1e-3;
	for j=1:20
		[xmin,fmin,counteval,stopflag] = cmaes('bbob',startingpoint,stepsize,opts);
		stopflag;
		iterations(i,j) = counteval;
	end;
end;
iterations;
median(iterations,2)






% cmaes with bipop from BBOB2013 settings:
% opts.TolX = '1e-12*max(insigma)';
% opts.TolUpX = Inf;
% opts.TolHistFun = 1e-12;
% opts.StopOnEqualFunctionValues = Inf;
% opts.CMA.cs = '(mueff+2)/(N+mueff+5)';
% opts.CMA.ccum = '((N+4 + 2*mueff/N) / (4 + mueff/N))^-1';
