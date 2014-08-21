% Samples expected values for each benchmark problem in the BBOB 2013
function expected
  rng(123456789, 'twister');

  for dim = [2, 40]
    x = rand(dim, 10000) * 10 - 5;
    save(['./data/parameters,', int2str(dim), '.mat'], 'x', '-ascii', '-double');
    
    xOpt = rand(dim, 1) * 4 - 4;
    save(['./data/parameterTranslation,', int2str(dim), '.mat'], 'xOpt', '-ascii', '-double');
    
    one = repmat(sign(rand - 0.5), dim, 1);
    save(['./data/one,', int2str(dim), '.mat'], 'one', '-ascii', '-double');
    
    R = getRandomRotation(dim);  
    save(['./data/rotationR,', int2str(dim), '.mat'], 'R', '-ascii', '-double');
    
    Q = getRandomRotation(dim);
    save(['./data/rotationQ,', int2str(dim), '.mat'], 'Q', '-ascii', '-double');
      
    C101 = getRandomC101(dim);
    save(['./data/delta101,', int2str(dim), '.mat'], 'C101', '-ascii', '-double');
      
    y101 = getRandomY101(dim);
    save(['./data/localOptima101,', int2str(dim), '.mat'], 'y101', '-ascii', '-double');
      
    C21 = getRandomC21(dim);
    save(['./data/delta21,', int2str(dim), '.mat'], 'C21', '-ascii', '-double');
      
    y21 = getRandomY21(dim);
    save(['./data/localOptima21,', int2str(dim), '.mat'], 'y21', '-ascii', '-double');

    for func = 1:24
      fx = nan(size(x, 2), 1);
      for k = 1:size(x, 2)
        fx(k) = f(func, x(:, k), xOpt, one, R, Q, C101, y101, C21, y21);
      end
        
      save(['./data/expectedF', int2str(func), ',', int2str(dim), '.mat'], 'fx', '-ascii', '-double');
    end
  end
end

function f = f(func, x, xOpt, one, R, Q, C101, y101, C21, y21)
  switch func
    case 1
      f = f1(x, xOpt);
    case 2
      f = f2(x, xOpt);
    case 3
      f = f3(x, xOpt);
    case 4
      f = f4(x, xOpt);
    case 5
      f = f5(x, one);
    case 6
      f = f6(x, xOpt, R, Q);
    case 7
      f = f7(x, xOpt, R, Q);
    case 8
      f = f8(x, xOpt);
    case 9
      f = f9(x, xOpt, R);
    case 10
      f = f10(x, xOpt, R);
    case 11
      f = f11(x, xOpt, R);
    case 12
      f = f12(x, xOpt, R);
    case 13
      f = f13(x, xOpt, R, Q);
    case 14
      f = f14(x, xOpt, R);
    case 15
      f = f15(x, xOpt, R, Q);
    case 16
      f = f16(x, xOpt, R, Q);
    case 17
      f = f17(x, xOpt, R, Q);
    case 18
      f = f18(x, xOpt, R, Q);
    case 19
      f = f19(x, R);
    case 20
      f = f20(x, one);
    case 21
      f = f21(x, R, C101, y101);
    case 22
      f = f22(x, R, C21, y21);
    case 23
      f = f23(x, xOpt, R, Q);
    case 24
      f = f24(x, one, R, Q);
  end
end

%%%%%%%%%%%%%%%%%%%% Bechmark problems %%%%%%%%%%%%%%%%%%%%
% ---------------
% Sphere function
% ---------------
function f = f1(x, xOpt)
  %----- Transformation in search space -----
  x = x - xOpt;  % shift optimum to zero 

  %----- Computation core -----
  f = norm(x)^2; 
end

% --------------------
% Ellipsoidal function
% --------------------
function f = f2(x, xOpt)
  %----- Initialisation -----
  dim = size(x, 1);
  scales = 1e6.^linspace(0, 1, dim)'; 

  %----- Transformation in search space -----
  x = x - xOpt;  % shift optimum to zero 

  %----- Computation core -----
  f = scales' * monotoneTFosc(x).^2; 
end

% ------------------
% Rastrigin function
% ------------------
function f = f3(x, xOpt)
  %----- Initialisation -----
  dim = size(x, 1);
  scales = sqrt(10).^linspace(0, 1, dim)'; 
  expo = 0.2 * linspace(0, 1, dim)';

  %----- Transformation in search space -----
  x = x - xOpt;  % shift optimum to zero 
  x = monotoneTFosc(x);
  idx = x > 0;
  x(idx) = x(idx).^(1 + expo(idx) .* sqrt(x(idx)));  % smooth also in zero
  x = scales .* x;
  
  %----- Computation core -----
  f = 10 * (dim - sum(cos(2*pi*x), 1)) + sum(x.^2, 1); 
end

% -------------------------
% Büche-Rastrigin function
% -------------------------
function f = f4(x, xOpt)
  %----- Initialisation -----
  dim = size(x, 1);
  xOpt(1:2:dim) = abs(xOpt(1:2:dim));
  scales = sqrt(10).^linspace(0, 1, dim)'; 

  %----- Boundary handling -----
  xoutside = max(0, abs(x) - 5) .* sign(x); 
  fPen = 1e2 * sum(xoutside.^2, 1);  % penalty

  %----- Transformation in search space -----
  x = x - xOpt;  % shift optimum to zero 
  x = monotoneTFosc(x); 
  idx = false(dim, 1); 
  idx(1:2:dim, :) = x(1:2:dim, :) > 0; 
  x(idx) = sqrt(100)*x(idx);
  x = scales .* x;  % scale while assuming that Xopt == 0 

  %----- Computation core -----
  f = 10 * (dim - sum(cos(2*pi*x), 1)) + sum(x.^2, 1);
  f = f + fPen;
end

% ------------
% Linear slope
% ------------
function f = f5(x, one)
  %----- Initialisation -----
  dim = size(x, 1);
  xOpt = 5 * one; 
  scales = -sign(xOpt) .* sqrt(100).^linspace(0, 1, dim)'; 

  %----- Boundary handling -----
  % move "too" good coordinates back into domain
  idx_out_of_bounds = x .* xOpt > 25;  % 25 == 5 * 5 
  x(idx_out_of_bounds) = sign(x(idx_out_of_bounds)) * 5; 

  %----- Computation core -----
  f = 5 * sum(abs(scales)) + scales' * x;
end

% --------------------------
% Attractive sector function
% --------------------------
function f = f6(x, xOpt, R, Q)
  %----- Initialisation -----
  dim = size(x, 1);
  scales = sqrt(10).^linspace(0, 1, dim)'; 
  % decouple scaling from function definition
  linearTF = Q * diag(scales) * R;

  %----- Transformation in search space -----
  x = x - xOpt;  % shift optimum to zero 
  x = linearTF * x;    % rotate/scale

  %----- Computation core -----
  idx = x.*xOpt > 0;
  x(idx) = 100*x(idx);
  f = monotoneTFosc(sum(x.^2,1)).^0.9; 
end

% -------------------------
% Step ellipsoidal function
% -------------------------
function f = f7(x, xOpt, R, Q)
  %----- Initialisation -----
  dim = size(x, 1);
  scales = 100.^linspace(0, 1, dim)'; 
  linearTF = diag(sqrt(10).^linspace(0, 1, dim)) * R;

  %----- Boundary handling -----
  xoutside = max(0, abs(x) - 5) .* sign(x); 
  fPen = 1 * sum(xoutside.^2, 1);  % penalty

  %----- Transformation in search space -----
  x = x - xOpt;  % shift optimum to zero 
  x = linearTF * x;  % rotate while assuming that Xopt == 0 
  x1 = x(1,:);
  idx = abs(x) > 0.5;
  x(idx) = round(x(idx));
  x(~idx) = round(10*x(~idx))/10; 
  x = Q * x;  % rotate while assuming that Xopt == 0 

  %----- Computation core -----
  f = 0.1 * max(1e-4*abs(x1), scales' * x.^2);
  f = f + fPen;
end

% -----------------------------
% Rosenbrock function, original
% -----------------------------
function f = f8(x, xOpt)
  %----- Initialisation -----
  dim = size(x, 1);
  xOpt = xOpt * 0.75; 
  scales = max(1, sqrt(dim) / 8.);  % only needed for rotated case

  %----- Transformation in search space -----
  x = x - xOpt;  % shift optimum to zero 
  x = scales * x;  % rotate/scale while assuming that Xopt == 0 
  x = x + 1;  % shift zero to factual optimum 1

  %----- Computation core -----
  f = 100 * sum((x(1:end-1, :).^2 - x(2:end, :)).^2, 1) + sum((x(1:end-1, :) - 1).^2, 1);
end

% -----------------------------
% Rosenbrock function, rotated
% -----------------------------
function f = f9(x, xOpt, R)
  %----- Initialisation -----
  dim = size(x, 1);
  scale = max(1, sqrt(dim) / 8.); 
  linearTF = scale * R;

  %----- Transformation in search space ----
  x = linearTF * x + 0.5; 

  %----- Computation core -----
  f = 100 * sum((x(1:end-1, :).^2 - x(2:end, :)).^2, 1) + sum((x(1:end-1, :) - 1).^2,1);
end

% --------------------
% Ellipsoidal function
% --------------------
function f = f10(x, xOpt, R)
  %----- Initialisation -----
  dim = size(x, 1);
  scales = 1e6.^linspace(0, 1, dim)';


  %----- Transformation in search space ----
  x = x - xOpt;  % shift optimum to zero 
  x = R * x;  % no scaling here, because it would go to the arrExpo
  x = monotoneTFosc(x);

  %----- Computation core -----
  f = scales' * x.^2; 
end

% ---------------
% Discus function
% ---------------
function f = f11(x, xOpt, R)
  %----- Transformation in search space ----
  x = x - xOpt;  % shift optimum to zero 
  x = R * x;  % no scaling here, because it would go to the arrExpo
  x = monotoneTFosc(x); 

  %----- Computation core -----
  f = sum(x.^2,1) + (1e6-1) * x(1,:).^2;
end

% -------------------
% Bent Cigar function
% -------------------
function f = f12(x, xOpt, R)
  %----- Initialisation -----
  dim = size(x, 1);
  expo = 0.5 * linspace(0, 1, dim)'; 

  %----- Transformation in search space ----
  x = x - xOpt;  % shift optimum to zero 
  x = R * x;  % no scaling here, because it would go to the arrExpo
  idx = x > 0;
  x(idx) = x(idx).^(1 + expo(idx) .* sqrt(x(idx)));  % smooth in zero
  x = R * x;  
  
  %----- Computation core -----
  f = 1e6 * sum(x.^2, 1) + (1-1e6) * x(1,:).^2;
end

% --------------------
% Sharp ridge function
% --------------------
function f = f13(x, xOpt, R, Q)
  %----- Initialisation -----
  dim = size(x, 1);
  scales = sqrt(10).^linspace(0, 1, dim)'; 
  % decouple scaling from function definition
  linearTF = Q * diag(scales) * R; 

  %----- Transformation in search space ----
  x = x - xOpt;  % shift optimum to zero 
  x = linearTF * x;    % rotate/scale

  %----- Computation core -----
  f = x(1,:).^2 + 100 * sqrt(sum(x(2:end,:).^2, 1));
end

% -------------------------
% Different powers function
% -------------------------
function f = f14(x, xOpt, R)
  %----- Initialisation -----
  dim = size(x, 1);
  expo = 2 + 4 * linspace(0, 1, dim)';

  %----- Transformation in search space ----
  x = x - xOpt;  % shift optimum to zero 
  x = R * x;    % rotate/scale

  %----- Computation core -----
  f = sqrt(sum(abs(x).^expo, 1)); 
end

% -------------------
% Rastrigin function
% -------------------
function f = f15(x, xOpt, R, Q)
  %----- Initialisation -----
  dim = size(x, 1);
  scales = sqrt(10).^linspace(0, 1, dim)'; 
  % decouple scaling from function definition
  linearTF = R * diag(scales) * Q;
  expo = 0.2 * linspace(0, 1, dim)';

  %----- Transformation in search space ----
  x = x - xOpt;  % shift optimum to zero 
  x = R * x;  % no scaling here, because it would go to the arrExpo
  x = monotoneTFosc(x); 
  idx = x > 0;
  x(idx) = x(idx).^(1 + expo(idx) .* sqrt(x(idx)));  % smooth in zero
  x = linearTF * x;     % rotate/scale

  %----- Computation core -----
  f = 10 * (dim - sum(cos(2*pi*x), 1)) + sum(x.^2, 1); 
end

% ---------------------
% Weierstrass function
% ---------------------
function f = f16(x, xOpt, R, Q)
  %----- Initialisation -----
  dim = size(x, 1);
  scales = (1/sqrt(100)).^linspace(0, 1, dim)';
  % decouple scaling from function definition
  linearTF = R * diag(scales) * Q;
  K = 0:11; % number of summands, 20 in CEC2005, 10/12 saves 30% of time
  aK = 0.5.^K;
  bK = 3.^K;
  F0 = sum(aK .* cos(2*pi*bK * 0.5)); % optimal value

  %----- Boundary handling -----
  xoutside = max(0, abs(x) - 5) .* sign(x); 
  fPen = (10/dim) * sum(xoutside.^2, 1);  % penalty

  %----- Transformation in search space ----
  x = x - xOpt;  % shift optimum to zero 
  x = R * x;  % no scaling here, because it would go to the arrExpo
  x = monotoneTFosc(x); 
  x = linearTF * x;    % rotate/scale

  %----- Computation core -----
  f = 10 * (sum(cos(2*pi * (x+0.5) * bK) * aK')/dim - F0).^3; 
  f = f + fPen;
end

% ----------------------
% Schaffers F7 Function
% ----------------------
function f = f17(x, xOpt, R, Q)
  %----- Initialisation -----
  dim = size(x, 1);
  scales = sqrt(10).^linspace(0, 1, dim)'; 
  linearTF = diag(scales) * Q; 
  expo = 0.5 * linspace(0, 1, dim)';

  %----- Boundary handling -----
  xoutside = max(0, abs(x) - 5) .* sign(x); 
  fPen = 1e1 * sum(xoutside.^2, 1);  % parallel to f18

  %----- Transformation in search space ----
  x = x - xOpt;  % shift optimum to zero 
  x = R * x;  % no scaling here, because it would go to the arrExpo
  idx = x > 0;
  x(idx) = x(idx).^(1 + expo(idx) .* sqrt(x(idx)));  % smooth in zero
  x = linearTF * x;    % rotate/scale

  %----- Computation core -----
  s = x(1:dim-1, :).^2 + x(2:dim, :).^2;
  f = mean(s.^0.25 .* (sin(50*s.^0.1).^2+1), 1).^2; % ^2 for ftarget
  f = f + fPen;
end

% -------------------------------------------------
% Schaffers F7 Function, moderately ill-conditioned
% -------------------------------------------------
function f = f18(x, xOpt, R, Q)
  %----- Initialisation -----
  dim = size(x, 1);
  scales = sqrt(1e3).^linspace(0, 1, dim)'; 
  linearTF = diag(scales) * Q; 
  expo = 0.5 * linspace(0, 1, dim)';

  %----- Boundary handling -----
  xoutside = max(0, abs(x) - 5) .* sign(x); 
  fPen = 1e1 * sum(xoutside.^2, 1);  % penalty

  %----- Transformation in search space ----
  x = x - xOpt;  % shift optimum to zero 
  x = R * x;  % no scaling here, because it would go to the arrExpo
  idx = x > 0;
  x(idx) = x(idx).^(1 + expo(idx) .* sqrt(x(idx)));  % smooth in zero
  x = linearTF * x;    % rotate/scale

  %----- Computation core -----
  s = x(1:dim-1, :).^2 + x(2:dim, :).^2;
  f = mean(s.^0.25 .* (sin(50*s.^0.1).^2+1), 1).^2; % ^2 for ftarget 
  f = f + fPen;
end

% -------------------------------------------
% Composite Griewank-Rosenbrock function F8F2
% -------------------------------------------
function f = f19(x, R)
  %----- Initialisation -----
  dim = size(x, 1);
  scale = max(1, sqrt(dim) / 8.); 
  linearTF = scale * R; 

  %----- Transformation in search space ----
  x = linearTF * x + 0.5;    % rotate/scale

  %----- Computation core -----
  F2 = 100 * (x(1:end-1, :).^2 - x(2:end, :)).^2 + (1 - x(1:end-1, :)).^2;
  f = 10 + 10 * sum(F2/4000 - cos(F2), 1) / (dim - 1); 
end

% -----------------
% Schwefel function
% -----------------
function f = f20(x, one) 
  %----- Initialisation -----
  dim = size(x, 1);
  xOpt = 0.5 * one * 4.2096874633;
  scales = sqrt(10).^linspace(0, 1, dim)'; 
  signs = sign(xOpt);
  xOpt = 2*abs(xOpt);

  %----- Transformation in search space ----
  x = 2 * signs .* x;  % makes the below boundary handling effective for coordinates
  x(2:end,:) = x(2:end,:) + 0.25 * (x(1:end-1,:) - xOpt(1:end-1,:)); 
  x = 100 * (scales .* (x - xOpt) + xOpt); 

  %----- Boundary handling -----
  xoutside = max(0, abs(x) - 500) .* sign(x);  % in [-500,500]
  fPen = 0.01 * sum(xoutside.^2, 1);  % penalty

  %----- Computation core -----
  % 0.01: values below one should be close to the global optimum 
  f = 0.01 * ((418.9828872724339) - mean(x.*sin(sqrt(abs(x))), 1));
  f = f + fPen;
end

% ------------------------------------------
% Gallagher's Gaussian 101-me peaks function
% ------------------------------------------
function f = f21(x, R, C101, y101)
  %----- Initialisation -----
  dim = size(x, 1);
  w = [10, linspace(1.1, 9.1, 100)];

  %----- Boundary handling -----
  xoutside = max(0, abs(x) - 5) .* sign(x); 
  fPen = 1 * sum(xoutside.^2, 1);  % penalty

  %----- Transformation in search space ----
  x = R * x;  

  %----- Computation core -----
  xx = repmat(x, 1, 101) - R * y101;
  f = w' .* exp(-0.5/dim * (sum(C101.*xx.^2, 1)))';
  % f is in [0,10], 10 is best
  f = monotoneTFosc(10 - max(f, [], 1)).^2;  
  f = f + fPen;
end

% -----------------------------------------
% Gallagher's Gaussian 21-hi peaks function
% -----------------------------------------
function f = f22(x, R, C21, y21)
  %----- Initialisation -----
  dim = size(x, 1);
  w = [10, linspace(1.1, 9.1, 20)]; 

  %----- Boundary handling -----
  xoutside = max(0, abs(x) - 5) .* sign(x); 
  fPen = 1 * sum(xoutside.^2, 1);  % penalty

  %----- Transformation in search space ----
  x = R * x;  

  %----- Computation core -----
  xx = repmat(x, 1, 21) - R * y21;
  f = w' .* exp(-0.5/dim * (sum(C21.*xx.^2, 1)))';
  % f is in [0,10], 10 is best
  f = monotoneTFosc(10 - max(f, [], 1)).^2;  
  f = f + fPen;
end

% -----------------
% Katsuura function
% -----------------
function f = f23(x, xOpt, R, Q)
  %----- Initialisation -----
  dim = size(x, 1);
  scales = sqrt(100).^linspace(0, 1, dim)'; 
  linearTF = Q * diag(scales) * R;

  %----- Boundary handling -----
  xoutside = max(0, abs(x) - 5) .* sign(x); 
  fPen = 1 * sum(xoutside.^2, 1);

  %----- Transformation in search space ----
  x = x - xOpt;  % shift optimum to zero 
  x = linearTF * x;    % rotate/scale

  %----- Computation core -----
  arr2k = 2.^(1:32);
  arr = x * arr2k;  % DIMxd array
  f = -10/dim^2 + 10/dim^2 * prod(1 + (1:dim)' .* (abs(arr-round(arr)) * arr2k'.^-1)).^(10/dim^1.2); 
  f = f + fPen;
end

% -----------------------------
% Lunacek bi-Rastrigin function
% -----------------------------
function f = f24(x, one, R, Q)
  %----- Initialisation -----
  dim = size(x, 1);
  d = 1;  % shift [1,3], smaller is more difficult
  s = 1 - 0.5/(sqrt(dim+20)-4.1);  % tested up to DIM = 160 p in [0.25,0.33] 
  mu0 = 2.5;  % optimum shift
  mu1 = -sqrt((mu0^2 - d) / s);  % TODO: re-check this
  xOpt = 0.5 * mu0 * one; 
  scales = sqrt(100).^linspace(0, 1, dim)'; 
  % decouple scaling from function definition
  linearTF = Q * diag(scales) * R;
  scales = 2 * sign(xOpt);

  %----- Boundary handling -----
  xoutside = max(0, abs(x) - 5) .* sign(x); 
  fPen = 1e4 * sum(xoutside.^2, 1);  % penalty

  %----- Transformation in search space ----
  x = scales .* x;  

  %----- Computation core -----
  f = min(sum((x-mu0).^2, 1), d*dim + s * sum((x-mu1).^2, 1)) + 10*(dim - sum(cos(2*pi*(linearTF * (x-mu0))), 1));  
  f = f + fPen;
end

%%%%%%%%%%%%%%%%%%%% Helper functions %%%%%%%%%%%%%%%%%%%%
function g = monotoneTFosc(f)
  % maps [-inf,inf] to [-inf,inf] with different constants
  % for positive and negative part
  a = 0.1;
  g = f; 
  idx = (f > 0);
  g(idx) = log(f(idx))/a;
  g(idx) = exp(g(idx) + 0.49*(sin(g(idx)) + sin(0.79*g(idx)))).^a;
  idx = (f < 0);
  g(idx) = log(-f(idx))/a;
  g(idx) = -exp(g(idx) + 0.49*(sin(0.55*g(idx)) + sin(0.31*g(idx)))).^a;
end

function R = getRandomRotation(dim)
  % computes an orthogonal basis
  R = randn(dim, dim);
  for i = 1:dim
      for j = 1:i-1
        R(:,i) = R(:,i) - R(:,i)'*R(:,j) * R(:,j);
      end
      R(:,i) = R(:,i) / sqrt(sum(R(:,i).^2));
  end
end

function C = getRandomC101(dim)
  % compute scaling for each optimum
  condition = 1000.^linspace(0, 1, 100);
  [~, idx] = sort(rand(100, 1));  % random permutation
  condition = [sqrt(1000) condition(idx)];
  C = zeros(dim, 101); 
  for i = 1:101  % generation of cov matrices 
    s = condition(i).^linspace(-0.5, 0.5, dim); 
    [~, idx] = sort(rand(dim, 1)); % permutation instead of rotation
    C(:, i) = s(idx); % this is inverse Cov
  end
end

function C = getRandomC21(dim)
  % compute scaling for each optimum
  condition = 1000.^linspace(0, 1, 20);
  [~, idx] = sort(rand(20, 1));  % random permutation
  condition = [sqrt(1000) condition(idx)];
  C = zeros(dim, 21); 
  for i = 1:21  % generation of cov matrices 
    s = condition(i).^linspace(-0.5, 0.5, dim); 
    [~, idx] = sort(rand(dim, 1)); % permutation instead of rotation
    C(:, i) = s(idx); % this is inverse Cov
  end
end

function y = getRandomY101(dim)
 y = rand(dim, 101) * 10.0 - 5.0; 
 % global optimum not too close to boundary
 y(:, 1) = 0.8 * y(:,1); 
end

function y = getRandomY21(dim)
 y = rand(dim, 21) * 9.8 - 4.9; 
 % global optimum not too close to boundary
 y(:, 1) = 0.8 * y(:,1); 
end