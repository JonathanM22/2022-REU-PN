clc
close all
clear 

Fs = 339.9;        % Sampling frequency
T = 1/Fs;         % Sampling period
L = 3399;        % Length of signal
t = (0:L-1)*T;    % Time vector

%3185 
%S = readtable('data\Log3.1_excel.xlsx','Sheet','Sheet1','Range','D2:D3185');
%timedata = readtable('data\Log3.1_excel.xlsx','Sheet','Sheet1','Range','A2:A3185');

%3400
S = readtable('data\Log2.1_excel.xlsx','Sheet','Sheet1','Range','D2:D3400');
%timedata = readtable('data\Log2.1_excel.xlsx','Sheet','Sheet1','Range','A2:A3400');


X = table2array(S);
X = X/12000;
%t = table2array(timedata);

figure(1)
plot(t,X)
title('G-Force')
xlabel('Time (sec)')
ylabel('Acceleration (g)')

Y = fft(X);
P2 = abs(Y/L);
P1 = P2(1:L/2+1);
P1(2:end-1) = 2*P1(2:end-1);

f = Fs*(0:(L/2))/L;

figure(2)
plot(f,P1) 
title('Single-Sided Amplitude Spectrum of X(t)')
xlabel('f (Hz)')
ylabel('g')
%ylim([0.02 1])