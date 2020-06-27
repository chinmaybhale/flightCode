%% Write to shared resource
% This program provides a potential method of implementing a locking system
% to prevent a resource shared between two instances of MATLAB (in this
% case, a MAT file) from becoming corrupted by simultaneous access. 
%
% This "writer.m" script shows a potential method of how locking can be 
% implemented to write data to a file while avoiding simultaneous access
% from the "reader.m" script.
%
% This implementation uses empty files to act as "locks" to prevent access
% to a data file. For example, when the "readlock" file exists, nothing can
% be written to the data file, and vice versa. 

function writer
i =1;
    fid = fopen('datafile.txt', 'w');
    formatSpec = '%.4f, \t %.4f, \t %.4f, \t %.4f, \t %.4f, \t %.4f, \t %.4f, \t %.4f, \t %.4f, \t %.4f \n';
Pressure = zeros(1, 10);
while true
    % Create lock to prevent 'reader' program from accessing data
    createWriteLock(); 

    if canWrite() 
        % The 'reader' program is not accessing the data
        for j = 1:10
            Pressure(1, j) = -5 + (5+5)*rand(1,1);
        end
        
        fprintf(fid, formatSpec, Pressure(1, 1), Pressure(1, 2), Pressure(1, 3),...
        Pressure(1, 4), Pressure(1, 5), Pressure(1, 6), Pressure(1, 7),...
        Pressure(1, 8), Pressure(1, 9), Pressure(1, 10));
        i = i + 1;
%         save('datafile.mat'); % Write data to file (in general, access resource)
        
        deleteWriteLock(); % Delete lock, so the 'reader' can access data
        
        pause(.5); % Wait before writing more data
    else
        % The 'reader' program is accessing the data
        
        deleteWriteLock(); % Delete lock, to prevent blocking the other script unnecessarily long
        
        % Wait briefly, to give 'reader' time to finish using file before 
        % trying again
        pause(.1); 
    end
    
end

end

% Creates the 'writelock' file 
function createWriteLock 
    fclose(fopen('writelock', 'a'));
end

% Deletes the 'writelock' file
function deleteWriteLock
    delete('writelock');
end

% Determines whether the 'writer' can write data to the file. The writer
% can only write data if the 'readlock' file does not currently exist.
function cw = canWrite
    fileExists = exist('readlock', 'file');
    if fileExists == 0
        cw = true; % 'readlock' file does not exist, can write
    else
        cw = false; % 'readlock' file does exist, can not write
    end
end