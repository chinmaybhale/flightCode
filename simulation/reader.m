%% Read from shared resource
% This program provides a potential method of implementing a locking system
% to prevent a resource shared between two instances of MATLAB (in this
% case, a MAT file) from becoming corrupted by simultaneous access.
%
% This "reader.m" script shows a potential method of how locking can be
% implemented to read data from a file while avoiding simultaneous access
% from the "writer.m" script.
%
% This implementation uses empty files to act as "locks" to prevent access
% to a data file. For example, when the "readlock" file exists, nothing can
% be written to the data file, and vice versa. 


fid = fopen('datafile.txt');
% tline = fgetl(fid);

% while ischar(tline)
%     disp(tline)
%     tline = fgetl(fid);
% end
% fclose(fid);
i = 1;

while true
    % Create lock to prevent 'writer' program from accessing data
    createReadLock();
    
    if canRead
        % The 'writer' program is not accessing the data
        
%         load('dataFile.mat'); % Read data from file
        tline = fgetl(fid);
        data{i} = tline;
        deleteReadLock(); % Delete lock, so the 'writer' can access data
        
        i = i +1;
        pause(0.5); % Wait before reading data again
    else
        % The 'writer' program is accessing the data
        
        deleteReadLock(); % Delete lock, to prevent blocking the other script unnecessarily long
        
        % Wait briefly, to give the 'writer' time to finish using file
        % before trying again
        pause(.1);
    end
    
end

% Create the 'readlock' file
function createReadLock 
    fclose(fopen('readlock', 'a'));
end

% Deletes the 'readlock' file
function deleteReadLock
    delete('readlock');
end

% Determines whether the 'reader' can write data to the file. The reader
% can only write data if the 'writelock' file does not currently exist.
function cr = canRead
    fileExists = exist('writelock', 'file');
    if fileExists == 0
        cr = true; % 'writelock' file does not exist, can read
    else
        cr = false; % 'writelock' file does exist, can not read
    end
end