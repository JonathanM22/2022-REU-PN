clc
close all
clear 

%3185 
%S = readtable('data\Log3.1_excel.xlsx','Sheet','Sheet1','Range','D2:D3185');
%timedata = readtable('data\Log3.1_excel.xlsx','Sheet','Sheet1','Range','A2:A3185');

S = readtable('data\LOG5_1.txt');
Sarray = table2array(S);
time = Sarray(:,1)/1000;
ax = Sarray(:,2);
ay = Sarray(:,3);
az = Sarray(:,4);

duration = (time(end,1) - time(1,1));
sample_rate = length(time) / duration;



