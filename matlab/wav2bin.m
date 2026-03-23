%[text] Save signal to file
function wav2bin(wav,text_file)
% arguments (Input)
%     x
%     y
% end
    audio = audioread(wav);

    fid = fopen(text_file,'w');

    fwrite(fid,audio,'single',0,'ieee-le');

    fclose(fid);

end

%[appendix]{"version":"1.0"}
%---
