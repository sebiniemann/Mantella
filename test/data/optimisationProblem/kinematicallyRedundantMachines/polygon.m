function [X, Y] = polygon(N, rad)
theta = 0:2*pi/N:2*pi;
Pos = rad * exp(1i * theta);

X = real(Pos);
Y = imag(Pos);
