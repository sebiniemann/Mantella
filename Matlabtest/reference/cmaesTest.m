global fitnessFunction;
fitnessFunction = 1;
iterations = [];
dimensions = 3;
startingpoint = 8 * rand(dimensions, 1) - 4;
stepsize = 2;
opts.DispFinal = "off";
opts.DispModulo = "Inf";
opts.SaveVariables = "off";
opts.MaxFunEvals = 20000;
opts.StopOnStagnation = 'off';
opts.StopOnWarnings = 'off';
opts.StopOnEqualFunctionValues = 0;
opts.MaxIter      = "100 + 50 * (N+3)^2 / sqrt(popsize)";
opts.TolX         = "1e-12*max(insigma)";
opts.TolUpX       = "Inf";
opts.TolFun       = "1e-12";
opts.TolHistFun   = "1e-12";
opts.CMA.ccum = "((N+4 + 2*mueff/N) / (4 + mueff/N))^-1 ";
opts.CMA.ccov1 = "2 / ((N+1.3)^2+mueff)";
opts.CMA.ccovmu = "2 * (mueff-2+1/mueff) / ((N+2)^2+mueff)";
FHANDLES = benchmarks('handles');
trials = 3;

stopflags = {};
targetFvalue = zeros(0);
bestFvalue = zeros(0);
bestX = {};

for i=1:24
	fitnessFunction = i;
	disp(['function: ' num2str(fitnessFunction)]);
	actFunc = FHANDLES{i};
    optimalObjectiveValue = feval(actFunc, 'init', dimensions, 0);
	opts.StopFitness = optimalObjectiveValue + 10^floor(log10(abs(optimalObjectiveValue))) * 1e-3;
	for j=1:trials
		[xmin,fmin,counteval,stopflag] = cmaes_orig('bbob',startingpoint,stepsize,opts);
		targetFvalue(i) = opts.StopFitness;
		bestFvalue(i,j) = fmin;
		bestX(end+1) = xmin;
		stopflag;
		stopflags(i,j) = stopflag(1);
		iterations(i,j) = counteval;
	end;
end;
iterations;
stopflags
disp('target vs best function value');
[targetFvalue' bestFvalue]
median(iterations,2)
