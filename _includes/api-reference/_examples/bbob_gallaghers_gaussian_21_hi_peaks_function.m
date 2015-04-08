% Load samples
X = dlmread('X.mat');
Y = dlmread('Y.mat');
Z = dlmread('Z.mat');

% Generate surface plot
surf(X, Y, Z);
shading interp;
title(['bbob Gallagher', char(39), 's Gaussian 21-hi peaks function - surface plot']);
xlabel('x_1');
ylabel('x_2');
zlabel('f(x_1, x_2)');
view(-35, 35);
colorbar;
print('bbob_gallaghers_gaussian_21_hi_peaks_function_surface','-dpng');

% Generate contour plot
contour(X, Y, Z);
title(['bbob Gallagher', char(39), 's Gaussian 21-hi peaks function - contour plot']);
xlabel('x_1');
ylabel('x_2');
colorbar;
print('bbob_gallaghers_gaussian_21_hi_peaks_function_contour','-dpng');