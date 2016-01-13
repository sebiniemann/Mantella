function [xmin, ...      % minimum search point of last iteration
	  fmin, ...      % function value of xmin
	  counteval, ... % number of function evaluations done
	  stopflag ...  % stop criterion reached
] = cmaes( ...
    fitfun, ...    % name of objective/fitness function
    xstart, ...    % objective variables initial point, determines N
    insigma, ...   % initial coordinate wise standard deviation(s)
    inopts, ...    % options struct, see defopts below
    varargin )     % arguments passed to objective function 

% Options defaults: Stopping criteria % (value of stop flag)
defopts.StopFitness  = '-Inf % stop if f(xmin) < stopfitness, minimization';
defopts.MaxFunEvals  = 'Inf  % maximal number of fevals';
defopts.MaxIter      = '1e3*(N+5)^2/sqrt(lambda) % maximal number of iterations';
defopts.StopFunEvals = 'Inf  % stop after resp. evaluation, possibly resume later';
defopts.StopIter     = 'Inf  % stop after resp. iteration, possibly resume later';
defopts.TolX         = '1e-11*max(insigma) % stop if x-change smaller TolX';
defopts.TolUpX       = '1e3*max(insigma) % stop if x-changes larger TolUpX';
defopts.TolFun       = '1e-12 % stop if fun-changes smaller TolFun';
defopts.TolHistFun   = '1e-13 % stop if back fun-changes smaller TolHistFun';
defopts.StopOnStagnation = 'on  % stop when fitness stagnates for a long time';
defopts.StopOnWarnings = 'yes  % ''no''==''off''==0, ''on''==''yes''==1 ';
defopts.StopOnEqualFunctionValues = '2 + N/3  % number of iterations';  

% Options defaults: Other
defopts.DiffMaxChange = 'Inf  % maximal variable change(s), can be Nx1-vector';
defopts.DiffMinChange = '0    % minimal variable change(s), can be Nx1-vector';
defopts.WarnOnEqualFunctionValues = ...
    'yes  % ''no''==''off''==0, ''on''==''yes''==1 ';
defopts.LBounds = '-Inf % lower bounds, scalar or Nx1-vector'; 
defopts.UBounds = 'Inf  % upper bounds, scalar or Nx1-vector'; 

defopts.PopSize      = '(4 + floor(3*log(N)))  % population size, lambda'; 
defopts.ParentNumber = 'floor(lambda/2)       % AKA mu, popsize equals lambda';
defopts.RecombinationWeights = 'superlinear decrease % or linear, or equal';
defopts.CMA.cs = '(mueff+2)/(N+mueff+3)  % cumulation constant for step-size'; 
defopts.CMA.damps = '1 + 2*max(0,sqrt((mueff-1)/(N+1))-1) + cs  % damping for step-size';
defopts.CMA.ccum = '4/(N+4)  % cumulation constant for covariance matrix'; 
defopts.CMA.ccov1 = '2 / ((N+1.3)^2+mueff)  % learning rate for rank-one update'; 
defopts.CMA.ccovmu = '2 * (mueff-2+1/mueff) / ((N+2)^2+mueff) % learning rate for rank-mu update'; 
defopts.CMA.on     = 'yes'; 
defopts.CMA.active = '0  % active CMA 1: neg. updates with pos. def. check, 2: neg. updates'; 

% ---------------------- Handling Input Parameters ----------------------
if isempty(fitfun)
  error('Objective function not determined');
end
if ~ischar(fitfun)
  error('first argument FUN must be a string');
end
if isempty(xstart)
  error('Initial search point, and problem dimension, not determined');
end
if isempty(insigma)
      error('Initial search volume is zero, choose SIGMA or X0 appropriate');
end

% Compose options opts
if nargin < 4 || isempty(inopts) % no input options available
  opts = defopts;
else
  opts = getoptions(inopts, defopts);
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  
counteval = 0;
% ------------------------ Initialization -------------------------------
% Handle resuming of old run
xmean = myeval(xstart)
  % Assign settings from input parameters and options for myeval...
  N = size(xmean, 1);
  lambda = myeval(opts.PopSize)
  insigma = myeval(insigma);
  if all(size(insigma) == [N 2]) 
    insigma = 0.5 * (insigma(:,2) - insigma(:,1));
  end
  
%--------------------------------------------------------------
% Evaluate options
stopFitness = myeval(opts.StopFitness); 
stopMaxFunEvals = myeval(opts.MaxFunEvals);  
stopMaxIter = myeval(opts.MaxIter);  
stopFunEvals = myeval(opts.StopFunEvals);  
stopIter = myeval(opts.StopIter);  
stopTolX = myeval(opts.TolX);
stopTolUpX = myeval(opts.TolUpX);
stopTolFun = myeval(opts.TolFun);
stopTolHistFun = myeval(opts.TolHistFun);
flgActiveCMA = myeval(opts.CMA.active); 

% Do more checking and initialization 
  maxdx = myeval(opts.DiffMaxChange); % maximal sensible variable change
  mindx = myeval(opts.DiffMinChange); % minimal sensible variable change 
				      % can both also be defined as Nx1 vectors
  lbounds = myeval(opts.LBounds);		     
  ubounds = myeval(opts.UBounds);
  if length(lbounds) == 1
    lbounds = repmat(lbounds, N, 1);
  end
  if length(ubounds) == 1
    ubounds = repmat(ubounds, N, 1);
  end

  sigma = max(insigma)              % overall standard deviation
  pc = zeros(N,1)
  ps = zeros(N,1)  % evolution paths for C and sigma

  diagD = ones(N,1)      % diagonal matrix D defines the scaling
  diagC = diagD.^2
    B = eye(N,N)                      % B defines the coordinate system
    BD = B.*repmat(diagD',N,1)        % B*D for speed up only
    C = diag(diagC)                   % covariance matrix == BD*(BD)'

  fitness.hist=NaN*ones(1,10+ceil(3*10*N/lambda)); % history of fitness values
  fitness.histsel=NaN*ones(1,10+ceil(3*10*N/lambda)); % history of fitness values
  fitness.histbest=[]; % history of fitness values
  fitness.histmedian=[]; % history of fitness values

    fitness.hist(1)=feval(fitfun, xmean, varargin{:}); 
    fitness.histsel(1)=fitness.hist(1);
    counteval = counteval + 1;
    
  % Initialize further constants
  chiN=N^0.5*(1-1/(4*N)+1/(21*N^2))
  countiter = 0
  
% -------------------- Generation Loop --------------------------------
stopflag = {};
while isempty(stopflag)
  % set internal parameters
  if countiter == 0 || lambda ~= lambda_last
    lambda_last = lambda
    % Strategy internal parameter setting: Selection  
    mu = myeval(opts.ParentNumber) % number of parents/points for recombination
      weights = log(mu+0.5)-log(1:mu)'
    mueff=sum(weights)^2/sum(weights.^2) % variance-effective size of mu
    weights = weights/sum(weights)     % normalize recombination weights array
    
    % Strategy internal parameter setting: Adaptation
    cc = myeval(opts.CMA.ccum) % time constant for cumulation for covariance matrix
    cs = myeval(opts.CMA.cs) 
      ccov1 = myeval(opts.CMA.ccov1)
      ccovmu = min(1-ccov1, myeval(opts.CMA.ccovmu))
    damps = myeval(opts.CMA.damps)
  end    
  
  countiter = countiter + 1

  % Generate and evaluate lambda offspring
  fitness.raw = repmat(NaN, 1, lambda);

  for k=find(isnan(fitness.raw)), 
    % fitness.raw(k) = NaN; 
    % Resample, until fitness is not NaN
    while isnan(fitness.raw(k))
        arz(:,k) = rand(N,1); % (re)sample
        %arz(:,k) = ones(N,1);
          arx(:,k) = xmean + sigma * (BD * arz(:,k));                % Eq. (1)
      
        arxvalid(:,k) = arx(:,k);
      % You may handle constraints here.  You may copy and alter
      % (columns of) arxvalid(:,k) only for the evaluation of the
      % fitness function. arx should not be changed.
      fitness.raw(k) = feval(fitfun, arxvalid(:,k), varargin{:});
    end
    counteval = counteval + 1; % retries due to NaN are not counted
  end
  disp('arz');
  disp(arz);
  disp('arx');
  disp(arx);

  fitness.sel = fitness.raw;
  
  % Sort by fitness 
  [fitness.raw, fitness.idx] = sort(fitness.raw); 
  [fitness.sel, fitness.idxsel] = sort(fitness.sel);  % minimization
  fitness.hist(2:end) = fitness.hist(1:end-1);    % record short history of
  fitness.hist(1) = fitness.raw(1);               % best fitness values
  if length(fitness.histbest) < 120+ceil(30*N/lambda) || ...
       (mod(countiter, 5) == 0  && length(fitness.histbest) < 2e4)  % 20 percent of 1e5 gen.
    fitness.histbest = [fitness.raw(1) fitness.histbest];          % best fitness values
    fitness.histmedian = [median(fitness.raw) fitness.histmedian]; % median fitness values
  else
    fitness.histbest(2:end) = fitness.histbest(1:end-1); 
    fitness.histmedian(2:end) = fitness.histmedian(1:end-1); 
    fitness.histbest(1) = fitness.raw(1);           % best fitness values
    fitness.histmedian(1) = median(fitness.raw);    % median fitness values
  end
  fitness.histsel(2:end) = fitness.histsel(1:end-1); % record short history of
  fitness.histsel(1) = fitness.sel(1);               % best sel fitness values

  % Calculate new xmean, this is selection and recombination 
  xold = xmean % for speed up of Eq. (2) and (3)
  xmean = arx(:,fitness.idxsel(1:mu))*weights 
  zmean = arz(:,fitness.idxsel(1:mu))*weights%==D^-1*B'*(xmean-xold)/sigma

  % Cumulation: update evolution paths
  ps = (1-cs)*ps + sqrt(cs*(2-cs)*mueff) * (B*zmean)          % Eq. (4)
  hsig = norm(ps)/sqrt(1-(1-cs)^(2*countiter))/chiN < 1.4 + 2/(N+1)

  pc = (1-cc)*pc ...
        + hsig*(sqrt(cc*(2-cc)*mueff)/sigma) * (xmean-xold)     % Eq. (2)

  % Adapt covariance matrix
  neg.ccov = 0;  % TODO: move parameter setting upwards at some point
  if ccov1 + ccovmu > 0                                                    % Eq. (3)
      arpos = (arx(:,fitness.idxsel(1:mu))-repmat(xold,1,mu)) / sigma
      % "active" CMA update: negative update, in case controlling pos. definiteness 
      if flgActiveCMA > 0
        % set parameters
        neg.mu = mu;  
        neg.mueff = mueff;
        if flgActiveCMA > 10  % flat weights with mu=lambda/2
          neg.mu = floor(lambda/2);  
          neg.mueff = neg.mu;
        end
        neg.ccov = (1 - ccovmu) * 0.25 * neg.mueff / ((N+2)^1.5 + 2*neg.mueff);
        neg.minresidualvariance = 0.66;  % keep at least 0.66 in all directions, small popsize are most critical
        neg.alphaold = 0.5;
        neg.ccovfinal = neg.ccov;

        % prepare vectors, compute negative updating matrix Cneg and checking matrix Ccheck
        arzneg = arz(:,fitness.idxsel(lambda:-1:lambda - neg.mu + 1));
        [arnorms idxnorms] = sort(sqrt(sum(arzneg.^2, 1))); 
        [~, idxnorms] = sort(idxnorms);  % inverse index 
        arnormfacs = arnorms(end:-1:1) ./ arnorms; 
        % arnormfacs = arnorms(randperm(neg.mu)) ./ arnorms;
        arnorms = arnorms(end:-1:1); % for the record
        if flgActiveCMA < 20
          arzneg = arzneg .* repmat(arnormfacs(idxnorms), N, 1);  % E x*x' is N
          % arzneg = sqrt(N) * arzneg ./ repmat(sqrt(sum(arzneg.^2, 1)), N, 1);  % E x*x' is N
        end
        if flgActiveCMA < 10 && neg.mu == mu  % weighted sum
          if mod(flgActiveCMA, 10) == 1 % TODO: prevent this with a less tight but more efficient check (see below) 
            Ccheck = arzneg * diag(weights) * arzneg';  % in order to check the largest EV
          end
          artmp = BD * arzneg;
          Cneg = artmp * diag(weights) * artmp';
        else  % simple sum
          if mod(flgActiveCMA, 10) == 1
            Ccheck = (1/neg.mu) * arzneg*arzneg';  % in order to check largest EV
          end
          artmp = BD * arzneg;
          Cneg = (1/neg.mu) * artmp*artmp';

        end
        if mod(flgActiveCMA, 10) == 1 && 1 - neg.ccov * arnorms(idxnorms).^2 * weights < neg.minresidualvariance
          maxeigenval = max(eig(Ccheck));  % norm is much slower, because (norm()==max(svd())
          neg.ccovfinal = min(neg.ccov, (1-ccovmu)*(1-neg.minresidualvariance)/maxeigenval); 
        end
        % update C
        C = (1-ccov1-ccovmu+neg.alphaold*neg.ccovfinal+(1-hsig)*ccov1*cc*(2-cc)) * C ... % regard old matrix 
            + ccov1 * pc*pc' ...     % plus rank one update
            + (ccovmu + (1-neg.alphaold)*neg.ccovfinal) ...  % plus rank mu update
              * arpos * (repmat(weights,1,N) .* arpos') ...
              - neg.ccovfinal * Cneg;                        % minus rank mu update
      else  % no active (negative) update
        C = (1-ccov1-ccovmu+(1-hsig)*ccov1*cc*(2-cc)) * C ... % regard old matrix 
            + ccov1 * pc*pc' ...     % plus rank one update
            + ccovmu ...             % plus rank mu update
              * arpos * (repmat(weights,1,N) .* arpos')
      end
      diagC = diag(C)
  end
  
  % Adapt sigma
    sigma = sigma * exp(min(1, (sqrt(sum(ps.^2))/chiN - 1) * cs/damps))            % Eq. (5)
  
  % Update B and D from C

  if (ccov1+ccovmu+neg.ccov) > 0 && mod(countiter, 1/(ccov1+ccovmu+neg.ccov)/N/10) < 1
    C=triu(C)+triu(C,1)' % enforce symmetry to prevent complex numbers
    C = C*10e12;
    C = round(C);
    C = C/10e12;
    printf('%.30f\n',C);
    [B,tmp] = eig(C)     % eigen decomposition, B==normalized eigenvectors
                          % effort: approx. 15*N matrix-vector multiplications
    diagD = diag(tmp)

    % limit condition of C to 1e14 + 1
    if min(diagD) <= 0
	  %warning(['Iteration ' num2str(countiter) ...
	%	   ': Eigenvalue (smaller) zero']);
	  %diagD(diagD<0) = 0;
	  %tmp = max(diagD)/1e14;
	  %C = C + tmp*eye(N,N); diagD = diagD + tmp*ones(N,1); 
    end
    if max(diagD) > 1e14*min(diagD) 
	  %warning(['Iteration ' num2str(countiter) ': condition of C ' ...
	%	   'at upper limit' ]);
	  %tmp = max(diagD)/1e14 - min(diagD);
	  %C = C + tmp*eye(N,N); diagD = diagD + tmp*ones(N,1); 
    end
    
    diagC = diag(C) 
    diagD = sqrt(diagD) % D contains standard deviations now
    BD = B.*repmat(diagD',N,1) % O(n^2)
  end % if mod

  % ----- numerical error management -----
  % Adjust maximal coordinate axis deviations
  if any(sigma*sqrt(diagC) > maxdx)
    %sigma = min(maxdx ./ sqrt(diagC));
  end
  % Adjust minimal coordinate axis deviations
  if any(sigma*sqrt(diagC) < mindx)
    %sigma = max(mindx ./ sqrt(diagC)) * exp(0.05+cs/damps); 
  end
  % Adjust too low coordinate axis deviations
  if any(xmean == xmean + 0.2*sigma*sqrt(diagC)) 
        %C = C + (ccov1+ccovmu) * diag(diagC .* ...
        %                              (xmean == xmean + 0.2*sigma*sqrt(diagC)));
      %sigma = sigma * exp(0.05+cs/damps); 
  end
  % Adjust step size in case of (numerical) precision problem 
    tmp = 0.1*sigma*BD(:,1+floor(mod(countiter,N)));
  if all(xmean == xmean + tmp)
	%sigma = sigma * exp(0.2+cs/damps); 
  end
  % Adjust step size in case of equal function values (flat fitness)
  if fitness.sel(1) == fitness.sel(1+ceil(0.1+lambda/4))
      %sigma = sigma * exp(0.2+cs/damps); 
  end
  % Adjust step size in case of equal function values
  if countiter > 2 && myrange([fitness.hist fitness.sel(1)]) == 0  
	%sigma = sigma * exp(0.2+cs/damps); 
  end
    
  % ----- end numerical error management -----

  % Set stop flag
  if fitness.raw(1) <= stopFitness, stopflag(end+1) = {'fitness'}; end
  if counteval >= stopMaxFunEvals, stopflag(end+1) = {'maxfunevals'}; end
  if countiter >= stopMaxIter, stopflag(end+1) = {'maxiter'}; end
  if all(sigma*(max(abs(pc), sqrt(diagC))) < stopTolX) 
    stopflag(end+1) = {'tolx'};
  end
  if any(sigma*sqrt(diagC) > stopTolUpX) 
    stopflag(end+1) = {'tolupx'};
  end
  if sigma*max(diagD) == 0  % should never happen
    stopflag(end+1) = {'bug'};
  end
  if countiter > 2 && myrange([fitness.sel fitness.hist]) <= stopTolFun 
    stopflag(end+1) = {'tolfun'};
  end
  if countiter >= length(fitness.hist) && myrange(fitness.hist) <= stopTolHistFun 
    stopflag(end+1) = {'tolhistfun'};
  end

  if counteval >= stopFunEvals || countiter >= stopIter
    stopflag(end+1) = {'stoptoresume'};
  end
end % while, end generation loop

% -------------------- Final Procedures -------------------------------

% Evaluate xmean and return best recent point in xmin
fmin = fitness.raw(1);
xmin = arxvalid(:, fitness.idx(1)); % Return best point of last generation.
if length(stopflag) > sum(strcmp(stopflag, 'stoptoresume')) % final stopping
  tmp = feval(fitfun, xintobounds(xmean, lbounds, ubounds), varargin{:});
  counteval = counteval + 1;
  if tmp < fitness.raw(1)
    fmin = tmp;
    xmin = xintobounds(xmean, lbounds, ubounds); % Return xmean as best point
  end
end

% ---------------------------------------------------------------  
% ---------------------------------------------------------------  
function [x, idx] = xintobounds(x, lbounds, ubounds)
%
% x can be a column vector or a matrix consisting of column vectors
%
  if ~isempty(lbounds)
    if length(lbounds) == 1
      idx = x < lbounds;
      x(idx) = lbounds;
    else
      arbounds = repmat(lbounds, 1, size(x,2));
      idx = x < arbounds;
      x(idx) = arbounds(idx);
    end
  else
    idx = 0;
  end
  if ~isempty(ubounds)
    if length(ubounds) == 1
      idx2 = x > ubounds;
      x(idx2) = ubounds;
    else
      arbounds = repmat(ubounds, 1, size(x,2));
      idx2 = x > arbounds;
      x(idx2) = arbounds(idx2);
    end
  else
    idx2 = 0;
  end
  idx = idx2-idx; 
  
% ---------------------------------------------------------------  
% ---------------------------------------------------------------  
function opts=getoptions(inopts, defopts)
if nargin < 2 || isempty(defopts) % no default options available
  opts=inopts;
  return;
elseif isempty(inopts) % empty inopts invoke default options
  opts = defopts;
  return;
elseif ~isstruct(defopts) % handle a single option value
  if isempty(inopts) 
    opts = defopts;
  elseif ~isstruct(inopts)
    opts = inopts;
  else
    error('Input options are a struct, while default options are not');
  end
  return;
elseif ~isstruct(inopts) % no valid input options
  error('The options need to be a struct or empty');
end

  opts = defopts; % start from defopts 
  % if necessary overwrite opts fields by inopts values
  defnames = fieldnames(defopts);
  idxmatched = []; % indices of defopts that already matched
  for name = fieldnames(inopts)'
    name = name{1}; % name of i-th inopts-field
    if isoctave
      for i = 1:size(defnames, 1)
	idx(i) = strncmp(lower(defnames(i)), lower(name), length(name));
      end
    else
	idx = strncmpi(defnames, name, length(name));
    end
    if sum(idx) > 1
      error(['option "' name '" is not an unambigous abbreviation. ' ...
	     'Use opts=RMFIELD(opts, ''' name, ...
	     ''') to remove the field from the struct.']);
    end
    if sum(idx) == 1
      defname  = defnames{find(idx)}; 
      if ismember(find(idx), idxmatched)
	error(['input options match more than ones with "' ...
	       defname '". ' ...
	       'Use opts=RMFIELD(opts, ''' name, ...
	       ''') to remove the field from the struct.']);
      end
      idxmatched = [idxmatched find(idx)];
      val = getfield(inopts, name);
      if isstruct(val) % valid syntax only from version 6.5.0
	opts = setfield(opts, defname, ...
	    getoptions(val, getfield(defopts, defname))); 
      elseif isstruct(getfield(defopts, defname)) 
	warning(['option "' name '" disregarded (must be struct)']); 
      elseif ~isempty(val) % empty value: do nothing, i.e. stick to default
	opts = setfield(opts, defnames{find(idx)}, val);
      end
    else
      warning(['option "' name '" disregarded (unknown field name)']);
    end
  end

% ---------------------------------------------------------------  
% ---------------------------------------------------------------  
function res=myeval(s)
  if ischar(s)
    res = evalin('caller', s);
  else
    res = s;
  end

% ---------------------------------------------------------------  
% ---------------------------------------------------------------  
function res = isoctave
% any hack to find out whether we are running octave
  s = version;
  res = 0;
  if exist('fflush', 'builtin') && eval(s(1)) < 7
    res = 1;
  end

% ---------------------------------------------------------------  
% ---------------------------------------------------------------  
% ----- replacements for statistic toolbox functions ------------
% ---------------------------------------------------------------  
% ---------------------------------------------------------------  
function res=myrange(x)
  res = max(x) - min(x);
