%[text] Save SOS IIR filter to header file
%[text] Strips out unnecessary values and combines gains to a single float32
function save_sos(SOS,g,filename,prefix)
% arguments (Input)
%     x
%     y
% end
    nstages = size(SOS,1);

    fid = fopen(filename,'w');
    
    % Include guard
    fprintf(fid,['#pragma once' char([13 10])]);
    fwrite(fid,char([13 10]),'uchar'); % Blank Line
    
    % define Number of SOS stages, numlength and denlength
    fprintf(fid,['#define %s_N_SOS %d' char([13 10])], prefix,nstages);
    fprintf(fid,['#define %s_N_NUM %d' char([13 10])], prefix,3);
    fprintf(fid,['#define %s_N_DEN %d' char([13 10])], prefix,2);
    
    % define sos gain
    fprintf(fid,['#define %s_G_SOS %.7ff' char([13 10])], prefix,prod(g));
    fwrite(fid,char([13 10]),'uchar');

    

    
    %%% Write numerator
    %% fprintf(fid,'const float32_t %s_num[%d][3] = { ',prefix,nstages);
    %%for ct = 1:nstages
    %%    fprintf(fid,'{%.7ff, %.7ff, %.7ff}, ', single(SOS(ct,1)), single(SOS(ct,2)), single(SOS(ct,3)) );
    %%end
    %%fwrite(fid,[' };' char([13 10])],'uchar');    
%%
    %%% Write denominator
    %%% 4th value is always 1, for y[n]
    %%fprintf(fid,'const float32_t %s_den[%d][2] = { ',prefix,nstages);
    %%for ct = 1:nstages
    %%    fprintf(fid,'{%.7ff, %.7ff}, ', single(SOS(ct,5)), single(SOS(ct,6)) );
    %%end
    %%fwrite(fid,[' };' char([13 10])],'uchar');
    %%fwrite(fid,char([13 10]),'uchar');

    % Write filter coeffs as vector
     fprintf(fid,'const float32_t %s_coeffs[%d] = { ',prefix,nstages*5);
    for ct = 1:nstages
        fprintf(fid,'%.7ff, %.7ff, %.7ff,%.7ff, %.7ff, ', single(SOS(ct,1)), single(SOS(ct,2)), single(SOS(ct,3)), single(SOS(ct,5)), single(SOS(ct,6)) );
    end
    fwrite(fid,[' };' char([13 10])],'uchar');    
    
    

    fclose(fid);

end

%[appendix]{"version":"1.0"}
%---
