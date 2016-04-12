function Averaging
clear all

% number of stimulations
NumStimulation = 10;

% variables
PreTime = 30;
PostTime = 100;
SpikePerSti = 1;
EventTime = PreTime + PostTime;
TotalTime = NumStimulation*EventTime;

%preallocating the array
event = zeros(1,TotalTime+1);

average = 0;

% creating the array of event
for i = 1:EventTime:TotalTime+1

    % randomly generated noise and spike
    pre = rand(1,PreTime)/3.5;
    post = rand(1,PostTime)/2.6;
    spike = rand(1,SpikePerSti)/5+.8;
    
    if rand(1,1) <0.5
        
    pre(randperm(PreTime,1)) = spike;
    else
    post(randperm(PostTime,1)) = spike;
    end
    
    
    event(i:i+EventTime-1) = [pre post];
    
    % averaging the stimulations
    average =  (average + event(i:i+EventTime-1));
    average = average/NumStimulation;
    
end




subplot(2,1,1);
plot(average);
xlabel('Time');
ylabel('Voltage');
subplot(2,1,2);
plot(event);
xlabel('Time');
ylabel('Voltage');

end