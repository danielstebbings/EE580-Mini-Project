%[text] Save signal to file
function save_sig(signal,fs,filename,prefix)
% arguments (Input)
%     x
%     y
% end

    fid = fopen(filename,'w');
    
    % Include guard
    fprintf(fid,['#pragma once' char([13 10])]);
    fwrite(fid,char([13 10]),'uchar'); % Blank Line
    
    % Length of Signal & sampling freq
    fprintf(fid,['#define %s_N  %d' char([13 10])], prefix,length(signal));
    fprintf(fid,['#define %s_FS %d' char([13 10])], prefix,fs);

    % Write signal coeffs
    fprintf(fid,'const float32_t %s_coeffs[%d] = { ',prefix,length(signal));
    for ct = 1:length(signal)
        fprintf(fid,'%.7ff,', single(signal(ct)));
    end
    fwrite(fid,[' };' char([13 10])],'uchar');    
    
    

    fclose(fid);

end

%[appendix]{"version":"1.0"}
%---
