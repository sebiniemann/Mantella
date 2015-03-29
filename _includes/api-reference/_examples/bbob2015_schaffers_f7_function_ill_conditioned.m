% Load samples
X = dlmread('X.mat');
Y = dlmread('Y.mat');
Z = dlmread('Z.mat');

% Generate surface plot
surf(X, Y, Z);
shading interp;
title('BBOB2015 Schaffers f7 function, ill-conditioned - surface plot');
xlabel('x_1');
ylabel('x_2');
zlabel('f(x_1, x_2)');
view(-35, 35);
colorbar;
print('bbob2015_schaffers_f7_function_ill_conditioned_surface','-dpng');

% Generate contour plot
contour(X, Y, Z);
title('BBOB2015 Schaffers f7 function, ill-conditioned - contour plot');
xlabel('x_1');
ylabel('x_2');
colorbar;
print('bbob2015_schaffers_f7_function_ill_conditioned_contour','-dpng');