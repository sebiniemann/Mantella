function f = bbob(x)
global fitnessFunction;
if(isempty(fitnessFunction) || fitnessFunction == 25)
   disp('global fitnessFunction in bbob.m undefined. Now 1.');
   fitnessFunction = 1;
end;
f = benchmarks(x,fitnessFunction);
%sphere	function							= 1
%separable ellipsoid with monotone transformation, condition 1e6	= 2
%Rastrigin with monotone transformation separable "condition" 10	= 3
%skew Rastrigin-Bueche, condition 10, skew-"condition" 100		= 4
%linear slope 								= 5
%attractive sector							= 6
%step ellipsoid, condition 100						= 7
%Rosenbrock, non-rotated						= 8
%Rosenbrock, rotated							= 9
%ellipsoid with monotone transformation, condition 1e6			= 10
%discus (tablet) with monotone transformation, condition 1e6		= 11
%bent cigar with asymmetric space distortion, condition 1e6		= 12
%sharp ridge								= 13
%sum of different powers, between x^2 and x^6				= 14
%Rastrigin with asymmetric non-linear distortion, "condition" 10	= 15
%Weierstrass, condition 100						= 16
%Schaffers F7 with asymmetric non-linear transformation, condition 10	= 17
%Schaffers F7 with asymmetric non-linear transformation, condition 1000	= 18
%F8F2 sum of Griewank-Rosenbrock 2-D blocks				= 19
%Schwefel with tridiagonal variable transformation			= 20
%Gallagher with 101 Gaussian peaks, condition up to 1000, one global rotation = 21
%Gallagher with 21 Gaussian peaks, condition up to 1000, one global rotation = 22
%Katsuura function							= 23
%Lunacek bi-Rastrigin, condition 100					= 24
