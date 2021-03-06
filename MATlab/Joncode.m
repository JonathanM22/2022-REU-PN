clc
close all
clear 

%{
I modifed Linky's code to automatically calculate varibales like Sampling
frequency and Length of Signal.

It handles creating the data arrays for each axis including time. But when
computing the graphs I just used the time vector fomurla (t). 

It also works with the files that the 
arduino outputs, so I don't have to turn everything to a excel file. 

Logs 1-4 are not good/viable
Log 5 is idle
Log 6 is 40hz

%}

factory = 12000; %Converts Acell Data to G's

% Read Data From Table and Assign Arrays
S = readtable('data\Log2_1.txt');
Sarray = table2array(S);
time = Sarray(:,1);                             % Miliseconds
ax = Sarray(:,2)/factory;                       % X-Axis Acceleration
ay = Sarray(:,3)/factory;                       % Y-Axis Acceleration
az = Sarray(:,4)/factory;                       % Z-Axis Acceleration

duration = (time(end,1) - time(1,1))/1000;      % Duration
Fs = length(time) / duration;                   % Sampling frequency
T = 1/Fs;                                       % Sampling period
L = length(time);                               % Length of signal
t = (0:L-1)*T;                                  % Time vector

X = az; %Just set what axis you want analyze equal to X

% Plot General Acceleration vs Time Graph
figure(1)
plot(t,X)
title('Acceleration vs Time (Log2_1)')
xlabel('Time (sec)')
ylabel('Acceleration (g)')


%FFT Analyzes 
Y = fft(X);
P2 = abs(Y/L);
P1 = P2(1:L/2+1);
P1(2:end-1) = 2*P1(2:end-1);

f = Fs*(0:(L/2))/L;

%Plot Single-Sided Amplitude Spectrum of X(t)
figure(2)
plot(f,P1) 
title('Single-Sided Amplitude Spectrum of X(t) (Log2_1)')
xlabel('f (Hz)')
ylabel('Gravity in gs')